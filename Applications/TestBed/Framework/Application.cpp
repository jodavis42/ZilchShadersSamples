#include "Application.hpp"

#include "OpenGlRenderer.hpp"
#include "Camera.hpp"
#include "MaterialUniformBufferHelper.hpp"

#include "ZilchShadersStandard.hpp"

//-------------------------------------------------------------------Application
Application::Application()
{
  mRenderer = new OpenGlRenderer();
  mRenderer->Initialize();
  mMeshLibrary = new MeshLibrary();
  mMaterialLibrary = new MaterialLibrary();
  mTextureLibrary = new TextureLibrary();
  mShaderLibrary = new ShaderLibrary();
  mZilchShaderManager = new ZilchShaderManager();

  mCamera = new Camera();
  mCamera->SetCameraMode(Camera::Fps);
  mCamera->mTranslation = Vector3(0, 0, 5);
  mCamera->mPhi = Math::cPi / 2.0f;
  mCamera->mTheta = -Math::cPi / 2.0f;
  
  mFrameData.mLogicTime = 0;
  mFrameData.mFrameTime = 0;
}

Application::~Application()
{
  delete mZilchShaderManager;
  delete mShaderLibrary;
  delete mTextureLibrary;
  delete mMaterialLibrary;
  delete mMeshLibrary;
  delete mCamera;
  delete mRenderer;

  Zero::ShaderSettingsLibrary::GetInstance().Destroy();
  delete Zilch::ExecutableState::CallingState;
}

void Application::Initialize()
{
  Zilch::ZilchSetup zilchSetup;

  Zilch::Module module;
  Zilch::ExecutableState::CallingState = module.Link();

  Zero::ShaderSettingsLibrary::InitializeInstance();
  Zero::ShaderSettingsLibrary::GetInstance().GetLibrary();

  InitializeMeshes();
  InitializeTextures();
  InitializeShadersAndMaterials();

  mFrameDataBuffer = new UniformBuffer();
  mFrameDataBuffer->mBufferName = "FrameData";
  mFrameDataBuffer->mBufferData.Set(mFrameData);
  mFrameDataBuffer->mId = 0;
  mRenderer->CreateBuffer(mFrameDataBuffer);

  mCameraDataBuffer = new UniformBuffer();
  mCameraDataBuffer->mBufferName = "CameraData";
  mCameraDataBuffer->mId = 1;
  mCameraDataBuffer->mBufferData.Set(mCameraData);
  mRenderer->CreateBuffer(mCameraDataBuffer);

  Model* model = new Model();
  model->mScale = Vector3(2);
  model->mRotation = Matrix3::cIdentity;
  model->mTranslation = Vector3(1, 0, 0);
  model->mMaterial = mMaterialLibrary->Find("Shader");
  model->mMesh = mMeshLibrary->GetDefault();
  mModels.PushBack(model);

  model = new Model();
  model->mScale = Vector3(1);
  model->mRotation = Matrix3::cIdentity;
  model->mTranslation = Vector3(-2, 0, 0);
  model->mMaterial = mMaterialLibrary->Find("Shader");
  model->mMesh = mMeshLibrary->GetDefault();
  mModels.PushBack(model);
}

void  Application::InitializeMeshes()
{
  Mesh* mesh = new Mesh();
  mesh->mVertices.PushBack(Vertex(Vector3(-1, -1, -1), Vector3::cZAxis, Vector2(0, 0), Vector4(1)));
  mesh->mVertices.PushBack(Vertex(Vector3(+1, -1, -1), Vector3::cZAxis, Vector2(1, 0), Vector4(1)));
  mesh->mVertices.PushBack(Vertex(Vector3(-1, +1, -1), Vector3::cZAxis, Vector2(0, 1), Vector4(1)));
  mesh->mVertices.PushBack(Vertex(Vector3(+1, +1, -1), Vector3::cZAxis, Vector2(1, 1), Vector4(1)));

  mesh->mIndices.PushBack(2);
  mesh->mIndices.PushBack(0);
  mesh->mIndices.PushBack(1);

  mesh->mIndices.PushBack(2);
  mesh->mIndices.PushBack(1);
  mesh->mIndices.PushBack(3);

  mesh->mElementType = MeshElementType::Triangles;
  mesh->mName = "Quad";

  mRenderer->CreateMesh(mesh);

  mMeshLibrary->Add(mesh->mName, mesh);
}

void Application::InitializeTextures()
{
  Texture* texture = new Texture();
  texture->mSizeX = 16;
  texture->mSizeY = 16;
  texture->mName = "Texture";
  const int size = texture->mSizeX *  texture->mSizeY * 3;

  texture->mTextureData.Resize(size);
  
  for(int y = 0; y < texture->mSizeY; ++y)
  {
    for(int x = 0; x < texture->mSizeX; ++x)
    {
      int index = (x + y * texture->mSizeX) * 3;
      float value = (x / (float)texture->mSizeX) * (y / (float)texture->mSizeY);
      texture->mTextureData[index + 0] = value;
      texture->mTextureData[index + 1] = value;
      texture->mTextureData[index + 2] = value;
    }
  }
  mRenderer->CreateTexture(texture);
  // Clear data (not needed since it's uploaded to the renderer now)
  texture->mTextureData.Clear();
  mTextureLibrary->Add(texture->mName, texture);
}

void Application::InitializeShadersAndMaterials()
{
  Zilch::JsonReader jsonReader;
  Zilch::CompilationErrors errors;
  Zilch::JsonValue* json = jsonReader.ReadIntoTreeFromFile(errors, "BuildConfig.data", nullptr);
  String shadersCoreDir = json->GetMember("ShaderCoreDir")->AsString();
  String testShadersDir = json->GetMember("TestShadersDir")->AsString();

  mZilchShaderManager->Initialize(mTextureLibrary, mMaterialLibrary, mShaderLibrary, mRenderer->CreateBackend(), shadersCoreDir);
  
  mZilchShaderManager->AddShaderFragmentProjectDirectory(testShadersDir);

  MaterialCreationData simpleMat;
  simpleMat.mMaterialName = "Shader";
  simpleMat.mFragmentNames.PushBack("Vertex");
  simpleMat.mFragmentNames.PushBack("Pixel");
  mZilchShaderManager->AddMaterialDefinition(simpleMat);

  mZilchShaderManager->CreateShadersAndMaterials(mRenderer);
}

void Application::Update(float frameTime)
{
  mFrameData.mFrameTime += frameTime;
  mFrameData.mLogicTime += frameTime;
}

void Application::Reshape(int width, int height, float aspectRatio)
{
  mCamera->mAspectRatio = aspectRatio;
  mRenderer->Reshape(width, height, aspectRatio);
}

void Application::Draw()
{
  mCamera->SetMatrix();

  mFrameDataBuffer->mBufferData.Set(mFrameData);
  mRenderer->UpdateBufferData(mFrameDataBuffer);

  float nearDistance = 0.1f;
  float farDistance = 1000.0f;
  float fov = Math::DegToRad(45);
  mCameraData.mFarPlane = farDistance;
  mCameraData.mNearPlane = nearDistance;
  
  TransformBufferData transformData;
  transformData.mViewToPerspective = mRenderer->BuildPerspectiveMatrix(fov, mCamera->mAspectRatio, nearDistance, farDistance);
  transformData.mWorldToView = mCamera->mWorldToView.Transposed();

  UniformBuffer materialBuffers[Zero::FragmentType::Size];

  mRenderer->ClearTarget();
  for(size_t modelIndex = 0; modelIndex < mModels.Size(); ++modelIndex)
  {
    Model* model = mModels[modelIndex];
    Draw(model, transformData);
  }
}

void Application::Draw(Model* model, TransformBufferData& transformData)
{
  Material* material = model->mMaterial;
  ObjectData objData;

  transformData.mLocalToWorld = Matrix4::GenerateTransform(model->mTranslation, model->mRotation, model->mScale);
  UniformBuffer transformBuffer;
  transformBuffer.mId = 2;
  transformBuffer.mBufferData.Set(transformData);

  MaterialRendererData materialData;
  ExtractMaterialData(model, mTextureLibrary, materialData);

  objData.mMesh = model->mMesh;
  objData.mShader = material->mShader;
  // Record all the buffers and textures to render with
  objData.mPreBoundBuffers.PushBack(mFrameDataBuffer);
  objData.mPreBoundBuffers.PushBack(mCameraDataBuffer);
  objData.mBuffersToBind.PushBack(&transformBuffer);
  for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
    objData.mBuffersToBind.PushBack(&materialData.mMaterialBuffers[fragIndex]);
  objData.mTextures = materialData.mTextures;

  mRenderer->Draw(objData);
}

void Application::OnKeyDown(unsigned int key)
{
  mCamera->ProcessKeyboardInput(key, 0, 0);
}

void Application::OnKeyUp(unsigned int key)
{
  mCamera->ProcessKeyUp(key, 0, 0);
}

void Application::OnMouseInput(unsigned int button, bool isDown, int x, int y)
{
  mCamera->ProcessMouseInput(button, isDown, x, y);
}

void Application::OnMouseMove(int x, int y)
{
  mCamera->ProcessMouseMovement(x, y);
}

void Application::OnMouseScroll(int x, int y)
{
  mCamera->OnMouseScroll(x, y);
}

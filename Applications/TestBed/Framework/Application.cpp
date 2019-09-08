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
  mImGui = nullptr;

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
  InitializeBuffers();
  InitializeModels();
}

void  Application::InitializeMeshes()
{
  // Create the quad mesh
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
    mMeshLibrary->mDefaultName = "Quad";
  }
 
  // Create the particle system point mesh (no actual data, just a binding format)
  {
    Mesh* pointMesh = new Mesh();
    pointMesh->mVertices.PushBack(Vertex(Vector3::cZero, Vector3::cZAxis, Vector2(0, 0), Vector4(1)));
    pointMesh->mIndexCount = 100;
    pointMesh->mElementType = MeshElementType::Points;
    pointMesh->mName = "Points";
  
    mRenderer->CreateMesh(pointMesh);
  
    mMeshLibrary->Add(pointMesh->mName, pointMesh);
  }
}

void Application::InitializeTextures()
{
  {
    Texture* texture = new Texture();
    texture->mSizeX = 16;
    texture->mSizeY = 16;
    texture->mName = "Texture";
    const int size = texture->mSizeX *  texture->mSizeY * 3;
    texture->mMinFilter = TextureFiltering::Bilinear;
    texture->mMagFilter = TextureFiltering::Bilinear;
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
  
  {
    Texture* texture = new Texture();
    texture->mSizeX = 16;
    texture->mSizeY = 16;
    texture->mName = "Texture2";
    texture->mMinFilter = TextureFiltering::Bilinear;
    texture->mMagFilter = TextureFiltering::Bilinear;
    const int size = texture->mSizeX *  texture->mSizeY * 3;

    texture->mTextureData.Resize(size);

    for(int y = 0; y < texture->mSizeY; ++y)
    {
      for(int x = 0; x < texture->mSizeX; ++x)
      {
        int index = (x + y * texture->mSizeX) * 3;
        texture->mTextureData[index + 0] = (x / (float)texture->mSizeX);
        texture->mTextureData[index + 1] = (y / (float)texture->mSizeY);
        texture->mTextureData[index + 2] = 0;
      }
    }
    mRenderer->CreateTexture(texture);
    // Clear data (not needed since it's uploaded to the renderer now)
    texture->mTextureData.Clear();
    mTextureLibrary->Add(texture->mName, texture);
  }
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

  MaterialCreationData computeMat;
  computeMat.mMaterialName = "Compute";
  computeMat.mFragmentNames.PushBack("SimpleParticleReader");
  computeMat.mFragmentNames.PushBack("ParticleWorldGravity");
  computeMat.mFragmentNames.PushBack("ParticleIntegration");
  computeMat.mFragmentNames.PushBack("SimpleParticleWriter");
  mZilchShaderManager->AddMaterialDefinition(computeMat);
  
  MaterialCreationData particleMat;
  particleMat.mMaterialName = "ParticleRender";
  particleMat.mFragmentNames.PushBack("ParticleVertex");
  particleMat.mFragmentNames.PushBack("ParticlePixel");
  mZilchShaderManager->AddMaterialDefinition(particleMat);

  mZilchShaderManager->CreateShadersAndMaterials(mRenderer);
}

void Application::InitializeBuffers()
{
  // Create the frame data buffer
  {
    ByteBuffer frameDataByteBuffer;
    frameDataByteBuffer.Set(mFrameData);
    BufferCreationData frameDataCreationBuffer;
    frameDataCreationBuffer.mBufferData = &frameDataByteBuffer;
    frameDataCreationBuffer.mName = "FrameData";
    mFrameDataBufferData = mRenderer->CreateBuffer(frameDataCreationBuffer, BufferType::Uniform);
    mFrameDataBufferData.mBindingIndex = 0;
  }
  
  // Create the camera data buffer
  {
    ByteBuffer cameraDataByteBuffer;
    cameraDataByteBuffer.Set(mCameraData);
    BufferCreationData cameraDataCreationBuffer;
    cameraDataCreationBuffer.mBufferData = &cameraDataByteBuffer;
    cameraDataCreationBuffer.mName = "CameraData";
    mCameraDataBufferData = mRenderer->CreateBuffer(cameraDataCreationBuffer, BufferType::Uniform);
    mCameraDataBufferData.mBindingIndex = 1;
  }
  
  // Create the particle system ssbo data
  {
    BufferCreationData creationData;
    creationData.mName = "Particles";
    mParticlesSsboBufferData = mRenderer->CreateBuffer(creationData, BufferType::StructuredStorage);
    BufferMappingType::Enum mappingType = (BufferMappingType::Enum)(BufferMappingType::Write | BufferMappingType::InvalidateBuffer);
    ParticleData* ssboData = (ParticleData*)mRenderer->MapBuffer(mParticlesSsboBufferData, 0, sizeof(ParticleData) * 100, mappingType);
    for(size_t y = 0; y < 10; ++y)
    {
      for(size_t x = 0; x < 10; ++x)
      {
        ParticleData& data = ssboData[x + y * 10];
        data.mPosition = Vector4(x / 10.0f, y / 10.0f, 0, 1);
        data.mVelocity = Vector4(0, 0, 0, 0);
      }
    }
    mRenderer->UnMapBuffer(mParticlesSsboBufferData);
  }
}

void Application::InitializeModels()
{
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

  // Update the per-frame data
  ByteBuffer frameData;
  frameData.Set(mFrameData);
  mRenderer->UploadBuffer(mFrameDataBufferData, frameData);

  float nearDistance = 0.1f;
  float farDistance = 1000.0f;
  float fov = Math::DegToRad(45);
  mCameraData.mFarPlane = farDistance;
  mCameraData.mNearPlane = nearDistance;
  
  // Recompute the non-changing part of the transform buffer (could be a separate uniform buffer)
  TransformBufferData transformData;
  transformData.mViewToPerspective = mRenderer->BuildPerspectiveMatrix(fov, mCamera->mAspectRatio, nearDistance, farDistance);
  transformData.mWorldToView = mCamera->mWorldToView.Transposed();
  transformData.mPerspectiveToApiPerspective.SetIdentity();

  mRenderer->ClearTarget();

  DrawCompute(transformData);
  for(size_t modelIndex = 0; modelIndex < mModels.Size(); ++modelIndex)
  {
    Model* model = mModels[modelIndex];
    Draw(model, transformData);
  }

  mImGui->NewFrame();
  mImGui->DrawMaterials(mMaterialLibrary);
  mImGui->EndFrame();
  mImGui->Render();
}

void Application::DrawCompute(TransformBufferData& transformData)
{
  ObjectData objData;

  // Setup the compute render data
  Material* computeMaterial = mMaterialLibrary->Find("Compute");
  MaterialRendererData materialData;
  ExtractMaterialData(computeMaterial, mTextureLibrary, materialData);
  objData.mShader = computeMaterial->mShader;
  mParticlesSsboBufferData.mBindingIndex = 0;
  objData.mBuffers.PushBack(mParticlesSsboBufferData);
  
  // Run the compute pass
  mRenderer->DispatchCompute(objData, 100, 1, 1);

  // Setup the render pass
  objData.mMesh = mMeshLibrary->Find("Points");
  Material* particleRenderMaterial = mMaterialLibrary->Find("ParticleRender");
  MaterialRendererData particleRenderMaterialData;
  ExtractMaterialData(particleRenderMaterial, mTextureLibrary, particleRenderMaterialData);
  objData.mShader = particleRenderMaterial->mShader;

  ByteBuffer transformByteBuffer;
  transformByteBuffer.Set(transformData);
  BufferCreationData transformCreationData;
  transformCreationData.mBufferData = &transformByteBuffer;

  EphemeralBuffer& ephemeralBuffer = objData.mEphemeralBuffers.PushBack();
  ephemeralBuffer.mBufferType = BufferType::Uniform;
  ephemeralBuffer.mCreationData.mBufferData = &transformByteBuffer;
  ephemeralBuffer.mRenderData.mBindingIndex = 2;
  objData.mEphemeralBuffers.PushBack(ephemeralBuffer);

  mRenderer->Draw(objData);
}

void Application::Draw(Model* model, TransformBufferData& transformData)
{
  Material* material = model->mMaterial;

  ByteBuffer transformByteBuffer;
  transformData.mLocalToWorld = Matrix4::GenerateTransform(model->mTranslation, model->mRotation, model->mScale);
  transformByteBuffer.Set(transformData);

  ObjectData objData;
  objData.mMesh = model->mMesh;
  objData.mShader = material->mShader;
  // Record all the buffers and textures to render with
  objData.mBuffers.PushBack(mFrameDataBufferData);
  objData.mBuffers.PushBack(mCameraDataBufferData);

  MaterialRendererData materialData;
  ExtractMaterialData(model, mTextureLibrary, materialData);

  {
    EphemeralBuffer& ephemeralTransformBuffer = objData.mEphemeralBuffers.PushBack();
    ephemeralTransformBuffer.mBufferType = BufferType::Uniform;
    ephemeralTransformBuffer.mCreationData.mBufferData = &transformByteBuffer;
    ephemeralTransformBuffer.mRenderData.mBindingIndex = 2;
  }
  
  for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
  {
    if(materialData.mMaterialBuffers[fragIndex].mBindingIndex != -1)
    {
      EphemeralBuffer& buffer = objData.mEphemeralBuffers.PushBack();
      MaterialBufferData& materialBufferData = materialData.mMaterialBuffers[fragIndex];
      buffer.mBufferType = materialBufferData.mBufferType;
      buffer.mCreationData.mBufferData = &materialBufferData.mBufferData;
      buffer.mRenderData.mBindingIndex = materialBufferData.mBindingIndex;
    }
  }
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

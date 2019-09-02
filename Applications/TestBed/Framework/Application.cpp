#include "Application.hpp"

#include "OpenGlRenderer.hpp"
#include "Camera.hpp"

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
    //Material* material = model->mMaterial;
    //
    //ObjectData objData;
    //objData.mMesh = model->mMesh;
    //transformData.mLocalToWorld = Matrix4::GenerateTransform(model->mTranslation, model->mRotation, model->mScale);
    //
    //UniformBuffer transformBuffer;
    //transformBuffer.mId = 2;
    //transformBuffer.mBufferData.Set(transformData);
    //
    //for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
    //{
    //  UniformBuffer& materialBuffer = materialBuffers[fragIndex];
    //  materialBuffer.mId = material->mMaterialStageBindingData[fragIndex].mReflectionData.mBinding;// mZilchShaderManager->mGenerator->mSettings->mDefaultUniformBufferDescription.mBindingId + fragIndex;
    //  materialBuffer.mBufferData.Resize(material->mMaterialStageBindingData[fragIndex].mReflectionData.mSizeInBytes);
    //}
    //
    //for(size_t blockIndex = 0; blockIndex < material->mMaterialBlocks.Size(); ++blockIndex)
    //{
    //  MaterialBlock* block = material->mMaterialBlocks[blockIndex];
    //  for(size_t propIndex = 0; propIndex < block->mPropertyList.Size(); ++propIndex)
    //  {
    //    MaterialProperty* materialProp = block->mPropertyList[propIndex];
    //    if(materialProp->mValidReflectionObject == false)
    //      continue;
    //
    //    if(materialProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::SampledImage)
    //    {
    //      MaterialTextureProperty* textureProp = (MaterialTextureProperty*)materialProp;
    //      Texture* texture = mTextureLibrary->Find(textureProp->mTextureName);
    //      for(size_t bindingIndex = 0; bindingIndex < textureProp->mBindingData.Size(); ++bindingIndex)
    //      {
    //        ShaderPropertyBindingData& bindingData = textureProp->mBindingData[bindingIndex];
    //
    //        TextureData textureData;
    //        textureData.mTexture = texture;
    //        textureData.mTextureSlot = bindingData.mReflectionData.mBinding;
    //        objData.mTextures.PushBack(textureData);
    //      }
    //    }
    //    else
    //    {
    //      MaterialDataProperty* dataProp = (MaterialDataProperty*)materialProp;
    //      Zero::ShaderResourceReflectionData& reflectionData = dataProp->mBindingData.mReflectionData;
    //
    //      UniformBuffer& materialBuffer = materialBuffers[dataProp->mFragmentType];
    //      memcpy(materialBuffer.mBufferData.Data() + reflectionData.mOffsetInBytes, dataProp->mPropertyData.Data(), reflectionData.mSizeInBytes);
    //    }
    //  }
    //}
    //
    //objData.mShader = material->mShader;
    //objData.mPreBoundBuffers.PushBack(mFrameDataBuffer);
    //objData.mPreBoundBuffers.PushBack(mCameraDataBuffer);
    //objData.mBuffersToBind.PushBack(&transformBuffer);
    //for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
    //  objData.mBuffersToBind.PushBack(&materialBuffers[fragIndex]);
    //
    //mRenderer->Draw(objData);
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

  /// Extract all of the uniform buffers from the material for this object
  UniformBuffer materialBuffers[Zero::FragmentType::Size];

  // First, get the buffer id's and sizes for each shader stage
  for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
  {
    UniformBuffer& materialBuffer = materialBuffers[fragIndex];
    Zero::ShaderResourceReflectionData& stageReflectionData = material->mMaterialStageBindingData[fragIndex].mReflectionData;
    materialBuffer.mId = stageReflectionData.mBinding;
    materialBuffer.mBufferData.Resize(stageReflectionData.mSizeInBytes);
  }

  // Then for each material block, copy each property to the uniform buffer in the right locations
  for(size_t blockIndex = 0; blockIndex < material->mMaterialBlocks.Size(); ++blockIndex)
  {
    MaterialBlock* block = material->mMaterialBlocks[blockIndex];
    for(size_t propIndex = 0; propIndex < block->mPropertyList.Size(); ++propIndex)
    {
      MaterialProperty* materialProp = block->mPropertyList[propIndex];
      // If this property isn't actually a shader property for this material (fulfilled by another input) then skip it.
      if(materialProp->mValidReflectionObject == false)
        continue;

      // Handle images/samplers specially
      if(materialProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::SampledImage)
      {
        MaterialTextureProperty* textureProp = (MaterialTextureProperty*)materialProp;
        Texture* texture = mTextureLibrary->Find(textureProp->mTextureName);
        // Bind each texture id that was used
        for(size_t bindingIndex = 0; bindingIndex < textureProp->mBindingData.Size(); ++bindingIndex)
        {
          ShaderPropertyBindingData& bindingData = textureProp->mBindingData[bindingIndex];

          TextureData textureData;
          textureData.mTexture = texture;
          textureData.mTextureSlot = bindingData.mReflectionData.mBinding;
          objData.mTextures.PushBack(textureData);
        }
      }
      // Otherwise this is generic data, copy it to the relevant uniform buffer location
      else
      {
        MaterialDataProperty* dataProp = (MaterialDataProperty*)materialProp;
        Zero::ShaderResourceReflectionData& reflectionData = dataProp->mBindingData.mReflectionData;
        
        UniformBuffer& materialBuffer = materialBuffers[dataProp->mFragmentType];
        WriteProperty(materialBuffer.mBufferData, dataProp, dataProp->mBindingData);
        //memcpy(materialBuffer.mBufferData.Data() + reflectionData.mOffsetInBytes, dataProp->mPropertyData.Data(), reflectionData.mSizeInBytes);
      }
    }
  }

  objData.mMesh = model->mMesh;
  objData.mShader = material->mShader;
  // Record all the buffers and textures to render with
  objData.mPreBoundBuffers.PushBack(mFrameDataBuffer);
  objData.mPreBoundBuffers.PushBack(mCameraDataBuffer);
  objData.mBuffersToBind.PushBack(&transformBuffer);
  for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
    objData.mBuffersToBind.PushBack(&materialBuffers[fragIndex]);

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

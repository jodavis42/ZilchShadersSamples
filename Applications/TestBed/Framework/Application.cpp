#include "Application.hpp"

#include "OpenGlRenderer.hpp"
#include "Camera.hpp"

#include "ZilchShadersStandard.hpp"


Application::Application()
{
  mRenderer = new OpenGlRenderer();
  mRenderer->Initialize();

  mCamera = new Camera();
  mCamera->SetCameraMode(Camera::Fps);
  mCamera->mTranslation = Vector3(0, 0, 5);
  mCamera->mPhi = Math::cPi / 2.0f;
  mCamera->mTheta = -Math::cPi / 2.0f;
  mFrameTime = 0;
}

Application::~Application()
{
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

  InitializeShaders();
  InitializeTextures();
  InitializeMeshes();

  mBuffer = new UniformBuffer();
  mBuffer->mBufferName = "Shader_PixelFrame_Data";
  mBuffer->mData.Resize(8);
  mRenderer->CreateBuffer(mBuffer);
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

  mMeshes.PushBack(mesh);
}

void Application::InitializeTextures()
{
  Texture* texture = new Texture();
  texture->mSizeX = 16;
  texture->mSizeY = 16;
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
  // Clear data (not needed since it's uploaded)
  texture->mTextureData.Clear();
  mTextures.PushBack(texture);
}

typedef Zilch::Ref<Zero::ShaderTranslationPassResult> TranslationPassResultRef;

void CreateShader(Zero::SimpleZilchShaderIRGenerator& generator, Zero::ShaderPipelineDescription& pipeline, Material& material, Shader& shader, TranslationPassResultRef& passResult)
{
  Zero::ZilchShaderIRCompositor::ShaderDefinition shaderDef;
  shaderDef.mShaderName = material.mMaterialName;
  for(size_t i = 0; i < material.mFragmentNames.Size(); ++i)
  {
    Zero::ZilchShaderIRType* shaderType = generator.FindFragmentType(material.mFragmentNames[i]);
    shaderDef.mFragments.PushBack(shaderType);
  }

  Zero::ShaderCapabilities capabilities;
  generator.ComposeShader(shaderDef, capabilities);

  // Add all of the composited shader stages together into the shader library.
  for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
  {
    Zero::ZilchShaderIRCompositor::ShaderStageDescription& shaderInfo = shaderDef.mResults[i];
    if(shaderInfo.mShaderCode.Empty())
      continue;

    generator.AddShaderCode(shaderInfo.mShaderCode, shaderInfo.mClassName, nullptr);
  }
  generator.CompileAndTranslateShaders();
  generator.CompilePipeline(pipeline);

  
  
  for(size_t i = 0; i < Zero::FragmentType::Size; ++i)
  {
    Zero::ZilchShaderIRCompositor::ShaderStageDescription& shaderInfo = shaderDef.mResults[i];
    if(shaderInfo.mShaderCode.Empty())
      continue;
  
    Zero::ZilchShaderIRType* shaderType = generator.FindShaderType(shaderDef.mResults[i].mClassName);
    Zero::ShaderTranslationPassResult* passResult = generator.FindTranslationResult(shaderType);
    auto* reflection = generator.FindSimplifiedReflectionResult(shaderType);
    Array<TranslationPassResultRef> pipelineResults, debugResults;
    generator.CompilePipeline(shaderType, pipeline, pipelineResults, debugResults);
    String debug = debugResults[0]->ToString();
  
    passResult = pipelineResults.Back();
  
    shader.mShaderSource[i] = passResult->ToString();

    for(size_t i = 0; i < reflection->mReflection.mUniforms.Size(); ++i)
    {
      Zero::ShaderStageResource& uniformData = passResult->mReflectionData.mUniforms[i];
      UniformBuffer buffer;
      buffer.mData.Resize(uniformData.mReflectionData.mSizeInBytes);


    }


  }

  //Zero::SimplifiedShaderReflectionData reflectionData;
  //reflectionData.CreateReflectionData(generator.mShaderLibraryRef, )
}

void  Application::InitializeShaders()
{
  Zilch::JsonReader jsonReader;
  Zilch::CompilationErrors errors;
  Zilch::JsonValue* json = jsonReader.ReadIntoTreeFromFile(errors, "BuildConfig.data", nullptr);
  String shadersCoreDir = json->GetMember("ShaderCoreDir")->AsString();
  String testShadersDir = json->GetMember("TestShadersDir")->AsString();

  

  Zero::SpirVNameSettings nameSettings;
  SampleZilchShaderIRGenerator::LoadNameSettings(nameSettings);
  Zero::ZilchShaderSpirVSettings* settings = SampleZilchShaderIRGenerator::CreateZilchShaderSettings(nameSettings);
  SampleZilchShaderIRGenerator generator(settings);
  Zero::ShaderPipelineDescription pipeline;
  pipeline.mDebugPasses.PushBack(new Zero::ZilchSpirVDisassemblerBackend());
  //pipeline.mToolPasses.PushBack(new Zero::SpirVOptimizerPass());
  pipeline.mBackend = mRenderer->CreateBackend();

  generator.SetupDependencies(shadersCoreDir);
  generator.RecursivelyLoadDirectory(testShadersDir, generator.mFragmentProject);
  generator.CompileAndTranslateFragments();

  Material simpleMat;
  simpleMat.mMaterialName = "Shader";
  simpleMat.mFragmentNames.PushBack("Vertex");
  simpleMat.mFragmentNames.PushBack("Pixel");

  Shader* shader = new Shader();
  TranslationPassResultRef passResult;
  CreateShader(generator, pipeline, simpleMat, *shader, passResult);

  //for(size_t i = 0; i < passResult->mReflectionData.mUniforms.Size(); ++i)
  //{
  //  Zero::ShaderStageResource& uniformData = passResult->mReflectionData.mUniforms[i];
  //  UniformBuffer buffer;
  //  buffer.mData.Resize(uniformData.mReflectionData.mSizeInBytes);
  //
  //
  //}
  

  mRenderer->CreateShader(shader);

  mShaders.PushBack(shader);
}

void Application::Update(float frameTime)
{
  mFrameTime += frameTime;
}

void Application::Reshape(int width, int height, float aspectRatio)
{
  mCamera->mAspectRatio = aspectRatio;
  mRenderer->Reshape(width, height, aspectRatio);
}

void Application::Draw()
{
  memcpy(mBuffer->mData.Data(), &mFrameTime, sizeof(mFrameTime));
  memcpy(mBuffer->mData.Data() + sizeof(mFrameTime), &mFrameTime, sizeof(mFrameTime));
  mBuffer->mId = 0;
  mRenderer->UpdateBufferData(mBuffer);

  float nearDistance = 0.1f;
  float farDistance = 1000.0f;
  float fov = Math::DegToRad(45);
  CameraData cameraData;
  cameraData.mViewToPerspective = mRenderer->BuildPerspectiveMatrix(fov, mCamera->mAspectRatio, nearDistance, farDistance);
  //renderData.mCameraData.mWorldToView.BuildTransform(Vector3(0, 0, -5), Matrix3::cIdentity, Vector3(1, 1, 1));
  mCamera->SetMatrix();
  cameraData.mWorldToView = mCamera->mWorldToView;

  ObjectData objData;
  objData.mMesh = mMeshes[0];
  objData.mLocalToWorld = Matrix4::GenerateTransform(Vector3(1, 0, 0), Matrix3::cIdentity, Vector3(2));

  UniformBuffer transformBuffer;
  transformBuffer.mId = 2;
  transformBuffer.mBufferName = "Shader_VertexTransformData";
  transformBuffer.mData.Resize(256);

  TextureData textureData;
  textureData.mTexture = mTextures[0];
  textureData.mTextureSlot = 0;

  Matrix4 viewToPerspective = cameraData.mViewToPerspective;
  Matrix4 worldToView = cameraData.mWorldToView.Transposed();
  Matrix4 localToWorld = objData.mLocalToWorld;
  memcpy(transformBuffer.mData.Data(), localToWorld.array, 64);
  memcpy(transformBuffer.mData.Data() + 64, worldToView.array, 64);
  memcpy(transformBuffer.mData.Data() + 128, viewToPerspective.array, 64);
  
  objData.mShader = mShaders[0];
  objData.mPreBoundBuffers.PushBack(mBuffer);
  objData.mBuffersToBind.PushBack(&transformBuffer);
  objData.mTextures.PushBack(textureData);

  RenderData renderData;
  renderData.mObjects.PushBack(objData);
  renderData.mCameraData = cameraData;

  mRenderer->ClearTarget();
  mRenderer->Draw(renderData);
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

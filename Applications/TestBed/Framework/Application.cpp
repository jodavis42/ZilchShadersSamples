#include "Application.hpp"

#include "OpenGlRenderer.hpp"
#include "Camera.hpp"
#include "MaterialUniformBufferHelper.hpp"

#include "ZilchShadersStandard.hpp"

#include "JsonSerializer.hpp"
#include "GraphicsSpace.hpp"

#include "Renderer.hpp"
#include "Model.hpp"
#include "ComputePass.hpp"

//-------------------------------------------------------------------Application
Application::Application()
{
  mRenderer = new OpenGlRenderer();
  mRenderer->Initialize();
  mResourceSystem = new ResourceSystem();
  mResourceSystem->RegisterResourceLibrary(MeshLibrary);
  mResourceSystem->RegisterResourceLibrary(MaterialLibrary);
  mResourceSystem->RegisterResourceLibrary(TextureLibrary);
  mResourceSystem->RegisterResourceLibrary(ShaderLibrary);
  mResourceSystem->RegisterResourceLibrary(LevelLibrary);
  mResourceSystem->RegisterResourceLibrary(ProjectLibrary);
  mZilchShaderManager = new ZilchShaderManager();


  mSpace = nullptr;
  mCurrentLevel = nullptr;
  mCurrentProject = nullptr;
  mImGui = nullptr;

  mCamera = new Camera();
  mCamera->SetCameraMode(Camera::Fps);
  mCamera->mTranslation = Vector3(0, 0, 5);
  mCamera->mPhi = Math::cPi / 2.0f;
  mCamera->mTheta = -Math::cPi / 2.0f;
}

Application::~Application()
{
  DestroyProject();
  delete mCamera;
  delete mRenderer;

  Zero::ShaderSettingsLibrary::GetInstance().Destroy();
  delete Zilch::ExecutableState::CallingState;
}

void Application::Initialize()
{
  Zilch::ZilchSetup* zilchSetup = new Zilch::ZilchSetup();

  Zilch::Module module;
  Zilch::ExecutableState::CallingState = module.Link();

  Zero::ShaderSettingsLibrary::InitializeInstance();
  Zero::ShaderSettingsLibrary::GetInstance().GetLibrary();

  Zilch::JsonReader jsonReader;
  Zilch::CompilationErrors errors;
  Zilch::JsonValue* json = jsonReader.ReadIntoTreeFromFile(errors, "BuildConfig.data", nullptr);
  String shadersCoreDir = json->GetMember("ShaderCoreDir")->AsString();
  mZilchShaderManager->Initialize(mResourceSystem, mRenderer->CreateBackend(), shadersCoreDir);

  InitializeTextures();

  LoadProjects();
  LevelLibrary* levelLibrary = mResourceSystem->HasResourceLibrary(LevelLibrary);
  LoadLevel(levelLibrary->GetDefault());
}

void Application::InitializeTextures()
{
  TextureLibrary* textureLibrary = mResourceSystem->HasResourceLibrary(TextureLibrary);
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
    textureLibrary->Add(texture->mName, texture);
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
    textureLibrary->Add(texture->mName, texture);
  }
}

void Application::LoadProjects()
{
  ProjectLibrary* projectLibrary = mResourceSystem->HasResourceLibrary(ProjectLibrary);
  // Load the projects directory from the config
  JsonSerializer serializer;
  serializer.mResourceSystem = mResourceSystem;
  serializer.Load("BuildConfig.data");
  String ProjectsDir;
  serializer.SerializeField(ProjectsDir);

  // Add each folder as a project
  Zero::FileRange range(ProjectsDir);
  for(; !range.Empty(); range.PopFront())
  {
    auto entry = range.FrontEntry();
    String dirPath = entry.GetFullPath();
    if(!Zero::DirectoryExists(dirPath))
      continue;

    Project* project = new Project();
    project->mFilePath = dirPath;
    project->mName = entry.mFileName;
    projectLibrary->Add(project->mName, project);
  }
  projectLibrary->mDefaultName = "Quads";

  LoadProject(projectLibrary->GetDefault());
}

void Application::LoadProject(Project* project)
{
  DestroyProject();

  mCurrentProject = project;

  // Load all of the resources
  Zero::FileRange range(project->mFilePath);
  for(; !range.Empty(); range.PopFront())
  {
    auto entry = range.FrontEntry();
    String ext = Zero::FilePath::GetExtension(entry.mFileName);
    if(ext == "material")
      LoadMaterial(entry.GetFullPath());
    else if(ext == "mesh")
      LoadMesh(entry.GetFullPath());
    else if(ext == "level")
      LoadLevel(entry.GetFullPath());
    else if(ext == "zilchFrag")
      LoadZilchFragment(entry.GetFullPath());
  }

  MeshLibrary* meshLibrary = mResourceSystem->HasResourceLibrary(MeshLibrary);
  LevelLibrary* levelLibrary = mResourceSystem->HasResourceLibrary(LevelLibrary);
  // Create all of the resources now that their loaded. This is done in a second pass to control the order.
  for(auto range = meshLibrary->Values(); !range.Empty(); range.PopFront())
    mRenderer->CreateMesh(range.Front());
  mZilchShaderManager->CreateShadersAndMaterials(mRenderer);

  LoadLevel(levelLibrary->GetDefault());
}

void Application::DestroyProject()
{
  DestroyLevel();

  LevelLibrary* levelLibrary = mResourceSystem->HasResourceLibrary(LevelLibrary);
  MeshLibrary* meshLibrary = mResourceSystem->HasResourceLibrary(MeshLibrary);
  MaterialLibrary* materialLibrary = mResourceSystem->HasResourceLibrary(MaterialLibrary);
  ShaderLibrary* shaderLibrary = mResourceSystem->HasResourceLibrary(ShaderLibrary);
  // Destroy all of the resources (making sure to unload them from the renderer where appropriate)
  for(auto range = meshLibrary->Values(); !range.Empty(); range.PopFront())
    mRenderer->DestroyMesh(range.Front());
  meshLibrary->Destroy();

  for(auto range = materialLibrary->Values(); !range.Empty(); range.PopFront())
    mRenderer->DestroyShader(range.Front()->mShader);
  materialLibrary->Destroy();
  shaderLibrary->Destroy();

  levelLibrary->Destroy();
  mZilchShaderManager->ClearAll(mRenderer);
  mCurrentProject = nullptr;
}


void Application::LoadLevel(const String& filePath)
{
  LevelLibrary* levelLibrary = mResourceSystem->HasResourceLibrary(LevelLibrary);
  JsonSerializer serializer;
  serializer.mResourceSystem = mResourceSystem;
  serializer.Load(filePath);
  Level* level = levelLibrary->SerializeResource(serializer);
  level->mFilePath = filePath;
}

void Application::LoadLevel(Level* level)
{
  DestroyLevel();
  if(level == nullptr)
    return;

  mCurrentLevel = level;
  mSpace = new Space();

  GraphicsSpace* graphicsSpace = new GraphicsSpace();
  graphicsSpace->mRenderer = mRenderer;
  graphicsSpace->mResourceSystem = mResourceSystem;
  graphicsSpace->mCamera = mCamera;
  mSpace->AddComponent("GraphicsSpace", graphicsSpace);
  mSpace->Initialize(Engine::CompositionInitializer());

  JsonSerializer serializer;
  serializer.mResourceSystem = mResourceSystem;
  serializer.Load(level->mFilePath);

  size_t objCount = serializer.BeginArray("Objects");
  if(objCount != 0)
  {
    for(size_t i = 0; i < objCount; ++i)
    {
      serializer.BeginArrayItem(i);

      Cog* cog = new Cog();
      serializer.SerializeNamedField("Name", cog->mName);

      if(serializer.BeginObject("Transform"))
      {
        Transform* transform = new Transform();
        transform->Serialize(serializer);
        cog->AddComponent("Transform", transform);
        serializer.End();
      }
      if(serializer.BeginObject("Model"))
      {
        String meshName, materialName;
        Model* model = new Model();
        model->Serialize(serializer);
        cog->AddComponent("Model", model);
        serializer.End();
      }
      if(serializer.BeginObject("ComputePass"))
      {
        String materialName;
        ComputePass* computePass = new ComputePass();
        computePass->Serialize(serializer);
        cog->AddComponent("ComputePass", computePass);
        serializer.End();
      }

      mSpace->Add(cog);
      serializer.End();
    }
    serializer.End();
  }
}

void Application::DestroyLevel()
{
  delete mSpace;
  mSpace = nullptr;
  mCurrentLevel = nullptr;
}

void Application::LoadMesh(const String& filePath)
{
  JsonSerializer serializer;
  serializer.mResourceSystem = mResourceSystem;
  serializer.Load(filePath);

  MeshLibrary* meshLibrary = mResourceSystem->HasResourceLibrary(MeshLibrary);
  meshLibrary->SerializeResource(serializer);
}

void Application::LoadMaterial(const String& filePath)
{
  JsonSerializer serializer;
  serializer.mResourceSystem = mResourceSystem;
  serializer.Load(filePath);

  MaterialCreationData material;
  material.Serialize(serializer);
  mZilchShaderManager->AddMaterialDefinition(material);
}

void Application::LoadZilchFragment(const String& filePath)
{
  mZilchShaderManager->AddFragmentFile(filePath);
}

void Application::Update(float frameTime)
{
}

void Application::Reshape(int width, int height, float aspectRatio)
{
  mCamera->mAspectRatio = aspectRatio;
  mRenderer->Reshape(width, height, aspectRatio);
}

void Application::Draw()
{
  mSpace->Update(0.016f);

  ProjectLibrary* projectLibrary = mResourceSystem->HasResourceLibrary(ProjectLibrary);
  LevelLibrary* levelLibrary = mResourceSystem->HasResourceLibrary(LevelLibrary);
  MaterialLibrary* materialLibrary = mResourceSystem->HasResourceLibrary(MaterialLibrary);

  mImGui->NewFrame();
  mImGui->DrawResources(mResourceSystem);
  mImGui->EndFrame();
  mImGui->Render();
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

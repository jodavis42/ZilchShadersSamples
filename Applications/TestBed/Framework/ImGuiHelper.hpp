#pragma once

class SdlApp;
typedef union SDL_Event SDL_Event;

#include "ResourceSystem.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Project.hpp"
#include "Level.hpp"

using ResourceSystem = Engine::ResourceSystem;
using Material = Graphics::Material;
using MaterialBlock = Graphics::MaterialBlock;
using MaterialLibrary = Graphics::MaterialLibrary;
using MaterialProperty = Graphics::MaterialProperty;
using TextureLibrary = Graphics::TextureLibrary;
using Zero::String;

class ImGuiHelper
{
public:
  ImGuiHelper();
  ~ImGuiHelper();

  bool Init(SdlApp* sdlApp);
  void Shutdown();

  void ProcessEvent(SDL_Event& event);
  void NewFrame();
  void EndFrame();
  void Render();

  void DrawResources(ResourceSystem* resourceSystem);
  void DrawProjects(ProjectLibrary* projectLibrary);
  void DrawLevels(LevelLibrary* levelLibrary);
  void DrawMaterials(MaterialLibrary* materialLibrary);
  void DrawMaterial(Material* material);
  void DrawMaterialBlock(MaterialBlock* materialBlock);
  void DrawMaterialProperty(MaterialProperty* materialProp);
  bool DrawResourceProperty(String& currentValue, Engine::BaseResourceLibrary* library);

private:
  SdlApp* mSdlApp;
  void* glContext;
};

#pragma once

class SdlApp;
typedef union SDL_Event SDL_Event;

#include "Material.hpp"
#include "Texture.hpp"

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

  void DrawMaterials(MaterialLibrary* materialLibrary);
  void DrawMaterial(Material* material);
  void DrawMaterialBlock(MaterialBlock* materialBlock);
  void DrawMaterialProperty(MaterialProperty* materialProp);
  void DrawResourceProperty(String& currentValue, Graphics::BaseResourceLibrary* library);

private:
  SdlApp* mSdlApp;
  void* glContext;
};

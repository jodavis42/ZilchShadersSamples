#include "ImGuiHelper.hpp"

#include "SDL.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "ResourceLibrary.hpp"

ImGuiHelper::ImGuiHelper()
{
  mSdlApp = nullptr;
  glContext = nullptr;
}

ImGuiHelper::~ImGuiHelper()
{

}

bool ImGuiHelper::Init(SdlApp* sdlApp)
{
  mSdlApp = sdlApp;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplSDL2_InitForOpenGL(mSdlApp->window, mSdlApp->glContext);
  ImGui_ImplOpenGL3_Init("#version 150");
  return true;
}

void ImGuiHelper::Shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiHelper::ProcessEvent(SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void ImGuiHelper::NewFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(mSdlApp->window);
  ImGui::NewFrame();
}

void ImGuiHelper::EndFrame()
{
  ImGui::EndFrame();
}

void ImGuiHelper::Render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHelper::DrawMaterials(MaterialLibrary* materialLibrary)
{
  //bool p_open;
  //ImGui::ShowDemoWindow(&p_open);

  bool open;
  ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
  if(ImGui::Begin("Materials", &open))
  {
    for(auto range = materialLibrary->Values(); !range.Empty(); range.PopFront())
      DrawMaterial(range.Front());
  }
  ImGui::End();
}

void ImGuiHelper::DrawMaterial(Material* material)
{
  if(ImGui::CollapsingHeader(material->mMaterialName.c_str()))
  {
    for(size_t i = 0; i < material->mMaterialBlocks.Size(); ++i)
      DrawMaterialBlock(material->mMaterialBlocks[i]);
  }
}

void ImGuiHelper::DrawMaterialBlock(MaterialBlock* materialBlock)
{
  if(ImGui::TreeNode(materialBlock->mBlockName.c_str()))
  {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    ImGui::Columns(2);
    for(size_t i = 0; i < materialBlock->mPropertyList.Size(); ++i)
    {
      DrawMaterialProperty(materialBlock->mPropertyList[i]);
    }
    ImGui::Columns(1);
    ImGui::TreePop();
    ImGui::PopStyleVar();
  }
}

void ImGuiHelper::DrawMaterialProperty(MaterialProperty* materialProp)
{
  ImGui::PushID(materialProp);
  ImGui::AlignTextToFramePadding();

  ImGui::Text(materialProp->mPropertyName.c_str());
  ImGui::NextColumn();

  if(materialProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::SampledImage)
  {
    Graphics::MaterialResourceProperty* resourceProp = (Graphics::MaterialResourceProperty*)materialProp;
    DrawResourceProperty(resourceProp->mResourceName, resourceProp->mLibrary);
  }
  else
  {
    Graphics::MaterialDataProperty* dataProp = (Graphics::MaterialDataProperty*)materialProp;
    if(materialProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::Float || materialProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::Vector)
      ImGui::InputScalarN("##", ImGuiDataType_Float, dataProp->mPropertyData.Data(), materialProp->mShaderType->mComponents, NULL, NULL, "%.3f", 0);
  }
  ImGui::NextColumn();

  ImGui::PopID();
}

void ImGuiHelper::DrawResourceProperty(String& currentValue, Graphics::BaseResourceLibrary* library)
{
  Zero::HashMap<String, int> indexMap;
  Zero::Array<String> resources;
  for(auto range = library->mResourceMap.All(); !range.Empty(); range.PopFront())
  {
    String resourceName = range.Front().first;
    indexMap[resourceName] = resources.Size();
    resources.PushBack(resourceName);
  }

  auto lookupFn = [](void* data, int idx, const char** out_str)
  {
    Zero::Array<String>& resources = *(Zero::Array<String>*)data;
    *out_str = resources[idx].c_str();
    return true;
  };
  static int currentItem = indexMap.FindValue(currentValue, 0);
  if(ImGui::Combo("##", &currentItem, lookupFn, &resources, resources.Size()))
  {
    currentValue = resources[currentItem];
  }
}


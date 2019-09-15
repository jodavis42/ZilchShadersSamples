#pragma once

#include "FrameworkStandard.hpp"
#include "Engine/Component.hpp"
#include "Engine/Composition.hpp"
#include "Engine/Cog.hpp"
#include "Engine/Space.hpp"
#include "Engine/Transform.hpp"
#include "Engine/ResourceSystem.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Level.hpp"
#include "Project.hpp"
#include "ZilchShaderManager.hpp"
#include "AppBuiltInTypes.hpp"
#include "ImGuiHelper.hpp"

using Engine::Transform;
using Engine::Component;
using Engine::Cog;
using Engine::Space;

//-------------------------------------------------------------------Application
class Application
{
public:
  Application();
  ~Application();

  void Initialize();
  void InitializeTextures();

  void LoadProjects();
  void LoadProject(Project* project);
  void DestroyProject();
  void LoadLevel(const String& filePath);
  void LoadLevel(Level* level);
  void DestroyLevel();
  void LoadMesh(const String& filePath);
  void LoadMaterial(const String& filePath);
  void LoadZilchFragment(const String& filePath);

  void Update(float frameTime);
  // Reshape our projection matrix since the window size changed
  void Reshape(int width, int height, float aspectRatio);

  void Draw();

  void OnKeyDown(unsigned int key);
  void OnKeyUp(unsigned int key);
  void OnMouseInput(unsigned int button, bool isDown, int x, int y);
  void OnMouseMove(int x, int y);
  void OnMouseScroll(int x, int y);

  Renderer* mRenderer;
  Camera* mCamera;
  Space* mSpace;

  Project* mCurrentProject;
  Level* mCurrentLevel;

  ResourceSystem* mResourceSystem;

  ZilchShaderManager* mZilchShaderManager;
  ImGuiHelper* mImGui;
};
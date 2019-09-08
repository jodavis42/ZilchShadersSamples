#pragma once

#include "FrameworkStandard.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "ZilchShaderManager.hpp"
#include "AppBuiltInTypes.hpp"
#include "ImGuiHelper.hpp"

//-------------------------------------------------------------------Application
class Application
{
public:
  Application();
  ~Application();

  void Initialize();
  void InitializeMeshes();
  void InitializeTextures();
  void InitializeShadersAndMaterials();
  void InitializeBuffers();
  void InitializeModels();

  void Update(float frameTime);
  // Reshape our projection matrix since the window size changed
  void Reshape(int width, int height, float aspectRatio);

  void Draw();
  void DrawCompute(TransformBufferData& transformData);
  void Draw(Model* model, TransformBufferData& transformData);

  void OnKeyDown(unsigned int key);
  void OnKeyUp(unsigned int key);
  void OnMouseInput(unsigned int button, bool isDown, int x, int y);
  void OnMouseMove(int x, int y);
  void OnMouseScroll(int x, int y);

  FrameBufferData mFrameData;
  CameraBufferData mCameraData;

  BufferRenderData mFrameDataBufferData;
  BufferRenderData mCameraDataBufferData;
  BufferRenderData mParticlesSsboBufferData;

  Renderer* mRenderer;
  Camera* mCamera;
  MeshLibrary* mMeshLibrary;
  MaterialLibrary* mMaterialLibrary;
  TextureLibrary* mTextureLibrary;
  ShaderLibrary* mShaderLibrary;
  ZilchShaderManager* mZilchShaderManager;
  Array<Model*> mModels;
  ImGuiHelper* mImGui;
};
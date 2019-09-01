#pragma once

#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Material.hpp"

class Application
{
public:
  Application();
  ~Application();

  void Initialize();
  void InitializeMeshes();
  void InitializeShaders();

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
  Array<Mesh*> mMeshes;
  Array<Shader*> mShaders;
  UniformBuffer* mBuffer;
  float mFrameTime;
};
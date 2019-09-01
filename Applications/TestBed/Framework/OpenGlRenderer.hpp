#pragma once

#include "FrameworkStandard.hpp"

#include "Renderer.hpp"

class GlMeshData;
class GlShaderData;
class GlUniformBufferData;

class OpenGlRenderer : public Renderer
{
public:
  OpenGlRenderer();
  virtual ~OpenGlRenderer();

  void Initialize() override;

  void CreateMesh(Mesh* mesh) override;
  void DestroyMesh(Mesh* mesh) override;

  void CreateShader(Shader* shader) override;
  void DestroyShader(Shader* shader) override;

  void CreateBuffer(UniformBuffer* buffer) override;
  void UpdateBufferData(UniformBuffer* buffer) override;
  void DestroyBuffer(UniformBuffer* buffer) override;

  void ClearTarget() override;
  void Draw(ObjectData& objData) override;
  void Draw(RenderData& renderData) override;
  
  void Reshape(int width, int height, float aspectRatio) override;
  Matrix4 BuildPerspectiveMatrix(float verticalFov, float aspectRatio, float nearDistance, float farDistance) override;
  ZilchShaderIRBackend* CreateBackend() override;

  bool CompileShaderInternal(const String& shaderSource, int shaderType, int& shaderId);
  bool CompileShaderInternal(const char* shaderSource, size_t sourceLength, int shaderType, int& shaderId);
  bool LinkInternal(const Array<int>& shaderIds, int& programId);

  void BindBufferInternal(GlShaderData* glShader, UniformBuffer* buffer, GlUniformBufferData* glBuffer);
  void UpdateBufferDataInternal(UniformBuffer* buffer, GlUniformBufferData* glBuffer);

  HashMap<Mesh*, GlMeshData*> mGlMeshMap;
  HashMap<Shader*, GlShaderData*> mShaderMap;
  HashMap<UniformBuffer*, GlUniformBufferData*> mUniformBufferMap;
};


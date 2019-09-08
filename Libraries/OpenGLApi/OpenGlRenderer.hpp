///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "OpenGlStandard.hpp"

#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

namespace Graphics
{

class GlMeshData;
class GlTextureData;
class GlShaderData;
class GlBufferData;

//-------------------------------------------------------------------OpenGlRenderer
class OpenGlRenderer : public Renderer
{
public:
  OpenGlRenderer();
  virtual ~OpenGlRenderer();

  void Initialize() override;

  void CreateMesh(Mesh* mesh) override;
  void DestroyMesh(Mesh* mesh) override;

  void CreateTexture(Texture* texture) override;
  void DestroyTexture(Texture* texture) override;

  void CreateShader(Shader* shader) override;
  void DestroyShader(Shader* shader) override;

  BufferRenderData CreateBuffer(BufferCreationData& creationData, BufferType::Enum bufferType) override;
  void UploadBuffer(BufferRenderData& renderData, ByteBuffer& data) override;
  void* MapBuffer(BufferRenderData& renderData, size_t offset, size_t sizeInBytes, BufferMappingType::Enum mappingTypes) override;
  void UnMapBuffer(BufferRenderData& renderData) override;
  void DestroyBuffer(BufferRenderData& renderData) override;

  void ClearTarget() override;
  void Draw(ObjectData& objData) override;
  void DispatchCompute(ObjectData& objData, int x, int y, int z) override;

  void Reshape(int width, int height, float aspectRatio) override;
  Matrix4 BuildPerspectiveMatrix(float verticalFov, float aspectRatio, float nearDistance, float farDistance) override;
  ZilchShaderIRBackend* CreateBackend() override;

  bool CompileShaderInternal(const String& shaderSource, int shaderType, int& shaderId);
  bool CompileShaderInternal(const char* shaderSource, size_t sourceLength, int shaderType, int& shaderId);
  bool LinkInternal(const Array<int>& shaderIds, int& programId);

  void BindInternal(GlShaderData* glShader, ObjectData& objData);
  void UnBindInternal(GlShaderData* glShader, ObjectData& objData);
  void DestroyTemporaryBindingsInternal(GlShaderData* glShader, ObjectData& objData);
  void BindTextureInternal(TextureData* textureData, GlTextureData* glTexture);
  void BindInternal(GlBufferData* glBufferData, BufferRenderData* renderData);
  void UnBindInternal(GlBufferData* glBufferData, BufferRenderData* renderData);

  int GetElementType(MeshElementType::Enum elementType);
  int GetTextureType(TextureType::Enum type);
  int GetTextureAddressing(TextureAddressing::Enum addressing);
  int GetTextureMinFiltering(TextureFiltering::Enum filtering);
  int GetTextureMagFiltering(TextureFiltering::Enum filtering);
  int GetBufferType(BufferType::Enum bufferType);

  HashMap<Mesh*, GlMeshData*> mGlMeshMap;
  HashMap<Texture*, GlTextureData*> mTextureMap;
  HashMap<Shader*, GlShaderData*> mShaderMap;
};

}//namespace Graphics

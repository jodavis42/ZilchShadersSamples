///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "Buffers.hpp"

namespace Graphics
{

class Mesh;
class Texture;
class Shader;

//-------------------------------------------------------------------TextureData
class TextureData
{
public:
  TextureData();
  TextureData(Texture* texture, int textureSlot);

  Texture* mTexture;
  int mTextureSlot;
};

//-------------------------------------------------------------------ObjectData
class ObjectData
{
public:
  Mesh* mMesh;
  Shader* mShader;
  Array<TextureData> mTextures;

  Array<BufferRenderData> mBuffers;
  Array<EphemeralBuffer> mEphemeralBuffers;
};

//-------------------------------------------------------------------Renderer
class Renderer
{
public:
  virtual ~Renderer() {}

  virtual void Initialize() {};

  virtual void CreateMesh(Mesh* mesh) {};
  virtual void DestroyMesh(Mesh* mesh) {};

  virtual void CreateTexture(Texture* texture) {};
  virtual void DestroyTexture(Texture* texture) {};

  virtual void CreateShader(Shader* shader) {};
  virtual void DestroyShader(Shader* shader) {};

  virtual BufferRenderData CreateBuffer(BufferCreationData& creationData, BufferType::Enum bufferType) { return BufferRenderData(); };
  virtual void UploadBuffer(BufferRenderData& renderData, ByteBuffer& data) {};
  virtual void* MapBuffer(BufferRenderData& renderData, size_t offset, size_t sizeInBytes, BufferMappingType::Enum mappingTypes) { return nullptr; };
  virtual void UnMapBuffer(BufferRenderData& renderData) {};
  virtual void DestroyBuffer(BufferRenderData& renderData) {};

  virtual void ClearTarget() {};
  virtual void Draw(ObjectData& objData) {};
  virtual void DispatchCompute(ObjectData& objData, int x, int y, int z) {};

  virtual void Reshape(int width, int height, float aspectRatio) {};
  virtual Matrix4 BuildPerspectiveMatrix(float verticalFov, float aspectRatio, float nearDistance, float farDistance) abstract;
  virtual ZilchShaderIRBackend* CreateBackend() abstract;
};

}//namespace Graphics

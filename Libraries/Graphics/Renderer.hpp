///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

class Mesh;
class Texture;
class Shader;
class UniformBuffer;


class TextureData
{
public:
  TextureData();
  TextureData(Texture* texture, int textureSlot);

  Texture* mTexture;
  int mTextureSlot;
};

class ObjectData
{
public:
  //ObjectData();

  //void Load(Model* model);

  Matrix4 mLocalToWorld;
  Mesh* mMesh;
  Shader* mShader;
  Array<UniformBuffer*> mPreBoundBuffers;
  Array<UniformBuffer*> mBuffersToBind;
  Array<TextureData> mTextures;
};

class CameraData
{
public:
//  CameraData();

  Matrix4 mWorldToView;
  Matrix4 mViewToPerspective;

  //float mNearDistance;
  //float mFarDistance;
  //float mAspectRatio;
  //float mFov;
};

class RenderData
{
public:

  Array<ObjectData> mObjects;
  CameraData mCameraData;
};

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

  virtual void CreateBuffer(UniformBuffer* buffer) {};
  virtual void UpdateBufferData(UniformBuffer* buffer) {};
  virtual void DestroyBuffer(UniformBuffer* buffer) {};

  virtual void ClearTarget() {};
  virtual void Draw(ObjectData& objData) {};
  virtual void Draw(RenderData& renderData) {};

  virtual void Reshape(int width, int height, float aspectRatio) {};
  virtual Matrix4 BuildPerspectiveMatrix(float verticalFov, float aspectRatio, float nearDistance, float farDistance) abstract;
  virtual ZilchShaderIRBackend* CreateBackend() abstract;
};

}//namespace Graphics

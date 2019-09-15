///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "Component.hpp"

#include "Buffers.hpp"
#include "AppBuiltInTypes.hpp"

namespace Graphics
{

class Mesh;
class Material;
class TransformBufferData;
class ComputePass;
class Model;
class Renderer;
class Camera;
class ObjectData;
class MaterialRendererData;

//-------------------------------------------------------------------GraphicsSpace
class GraphicsSpace : public Engine::Component
{
public:
  GraphicsSpace();

  void Initialize(const Engine::CompositionInitializer& initializer) override;
  void Serialize(Serializer& serializer) override;
  void Update(float dt) override;

  void Render();
  void Draw(Composition* composition, TransformBufferData& transformData);
  void Draw(ComputePass* computePass, TransformBufferData& transformData);
  void Draw(Model* model, TransformBufferData& transformData);
  void ExtractMaterialRenderDataIntoObjectData(MaterialRendererData& materialData, ObjectData& objData);

  Renderer* mRenderer;
  Camera* mCamera;
  ResourceSystem* mResourceSystem;
  FrameBufferData mFrameData;
  CameraBufferData mCameraData;

  BufferRenderData mFrameDataBufferData;
  BufferRenderData mCameraDataBufferData;
  BufferRenderData mParticlesSsboBufferData;
};

}//namespace Graphics

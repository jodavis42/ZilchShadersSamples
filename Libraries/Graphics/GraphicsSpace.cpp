///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "GraphicsSpace.hpp"

#include "Space.hpp"
#include "Cog.hpp"
#include "Transform.hpp"
#include "AppBuiltInTypes.hpp"
#include "Renderer.hpp"
#include "MaterialUniformBufferHelper.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "ComputePass.hpp"

namespace Graphics
{

//-------------------------------------------------------------------GraphicsSpace
GraphicsSpace::GraphicsSpace()
{
  mRenderer = nullptr;
  mResourceSystem = nullptr;
  mCamera = nullptr;

  mFrameData.mLogicTime = 0;
  mFrameData.mFrameTime = 0;
}

void GraphicsSpace::Initialize(const Engine::CompositionInitializer& initializer)
{
  // Create the frame data buffer
  {
    ByteBuffer frameDataByteBuffer;
    frameDataByteBuffer.Set(mFrameData);
    BufferCreationData frameDataCreationBuffer;
    frameDataCreationBuffer.mBufferData = &frameDataByteBuffer;
    frameDataCreationBuffer.mName = "FrameData";
    mFrameDataBufferData = mRenderer->CreateBuffer(frameDataCreationBuffer, BufferType::Uniform);
    mFrameDataBufferData.mBindingIndex = 0;
  }

  // Create the camera data buffer
  {
    ByteBuffer cameraDataByteBuffer;
    cameraDataByteBuffer.Set(mCameraData);
    BufferCreationData cameraDataCreationBuffer;
    cameraDataCreationBuffer.mBufferData = &cameraDataByteBuffer;
    cameraDataCreationBuffer.mName = "CameraData";
    mCameraDataBufferData = mRenderer->CreateBuffer(cameraDataCreationBuffer, BufferType::Uniform);
    mCameraDataBufferData.mBindingIndex = 1;
  }

  // Create the particle system ssbo data
  {
    BufferCreationData creationData;
    creationData.mName = "Particles";
    mParticlesSsboBufferData = mRenderer->CreateBuffer(creationData, BufferType::StructuredStorage);
    BufferMappingType::Enum mappingType = (BufferMappingType::Enum)(BufferMappingType::Write | BufferMappingType::InvalidateBuffer);
    ParticleData* ssboData = (ParticleData*)mRenderer->MapBuffer(mParticlesSsboBufferData, 0, sizeof(ParticleData) * 100, mappingType);
    for(size_t y = 0; y < 10; ++y)
    {
      for(size_t x = 0; x < 10; ++x)
      {
        ParticleData& data = ssboData[x + y * 10];
        data.mPosition = Vector4(x / 10.0f, y / 10.0f, 0, 1);
        data.mVelocity = Vector4(0, 0, 0, 0);
      }
    }
    mRenderer->UnMapBuffer(mParticlesSsboBufferData);
  }
}

void GraphicsSpace::Serialize(Serializer& serializer)
{

}

void GraphicsSpace::Update(float dt)
{
  mFrameData.mFrameTime += dt;
  mFrameData.mLogicTime += dt;
  Render();
}

void GraphicsSpace::Render()
{
  mCamera->SetMatrix();

  // Update the per-frame data
  ByteBuffer frameData;
  frameData.Set(mFrameData);
  mRenderer->UploadBuffer(mFrameDataBufferData, frameData);
  
  float nearDistance = 0.1f;
  float farDistance = 1000.0f;
  float fov = Math::DegToRad(45);
  mCameraData.mFarPlane = farDistance;
  mCameraData.mNearPlane = nearDistance;
  
  // Recompute the non-changing part of the transform buffer (could be a separate uniform buffer)
  TransformBufferData transformData;
  transformData.mViewToPerspective = mRenderer->BuildPerspectiveMatrix(fov, mCamera->mAspectRatio, nearDistance, farDistance);
  transformData.mWorldToView = mCamera->mWorldToView.Transposed();
  transformData.mPerspectiveToApiPerspective.SetIdentity();
  mRenderer->ClearTarget();
  Space* space = GetOwner()->GetSpace();

  for(size_t i = 0; i < space->mCogs.Size(); ++i)
  {
    Draw(space->mCogs[i], transformData);
  }
}

void GraphicsSpace::Draw(Composition* composition, TransformBufferData& transformData)
{
  using Engine::Transform;
  Transform* transform = composition->Has(Transform);
  if(transform != nullptr)
  {
    transformData.mLocalToWorld = transform->GetWorldMatrix();
  }

  ComputePass* computePass = composition->Has(ComputePass);
  if(computePass != nullptr)
  {
    Draw(computePass, transformData);
  }

  Model* model = composition->Has(Model);
  if(model != nullptr)
  {
    Draw(model, transformData);
  }
}

void GraphicsSpace::Draw(ComputePass* computePass, TransformBufferData& transformData)
{
  ObjectData objData;

  // Setup the compute render data
  Material* computeMaterial = computePass->mMaterial;
  objData.mShader = computeMaterial->mShader;
  mParticlesSsboBufferData.mBindingIndex = 0;
  objData.mBuffers.PushBack(mParticlesSsboBufferData);

  MaterialRendererData materialData;
  ExtractMaterialData(computeMaterial, mResourceSystem, materialData);
  ExtractMaterialRenderDataIntoObjectData(materialData, objData);

  // Run the compute pass
  mRenderer->DispatchCompute(objData, computePass->mDispatchSize.x, computePass->mDispatchSize.y, computePass->mDispatchSize.z);
}

void GraphicsSpace::Draw(Model* model, TransformBufferData& transformData)
{
  Material* material = model->mMaterial;

  ByteBuffer transformByteBuffer;
  transformByteBuffer.Set(transformData);

  ObjectData objData;
  objData.mMesh = model->mMesh;
  objData.mShader = material->mShader;
  MaterialRendererData materialData;
  ExtractMaterialData(model->mMaterial, mResourceSystem, materialData);
  ExtractMaterialRenderDataIntoObjectData(materialData, objData);
  {
    EphemeralBuffer& ephemeralTransformBuffer = objData.mEphemeralBuffers.PushBack();
    ephemeralTransformBuffer.mBufferType = BufferType::Uniform;
    ephemeralTransformBuffer.mCreationData.mBufferData = &transformByteBuffer;
    ephemeralTransformBuffer.mRenderData.mBindingIndex = 2;
  }

  mRenderer->Draw(objData);
}

void GraphicsSpace::ExtractMaterialRenderDataIntoObjectData(MaterialRendererData& materialData, ObjectData& objData)
{
   // Record all the buffers and textures to render with
  objData.mBuffers.PushBack(mFrameDataBufferData);
  objData.mBuffers.PushBack(mCameraDataBufferData);

  for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
  {
    if(materialData.mMaterialBuffers[fragIndex].mBindingIndex != -1)
    {
      EphemeralBuffer& buffer = objData.mEphemeralBuffers.PushBack();
      MaterialBufferData& materialBufferData = materialData.mMaterialBuffers[fragIndex];
      buffer.mBufferType = materialBufferData.mBufferType;
      buffer.mCreationData.mBufferData = &materialBufferData.mBufferData;
      buffer.mRenderData.mBindingIndex = materialBufferData.mBindingIndex;
    }
  }
  objData.mTextures = materialData.mTextures;
}

}//namespace Graphics

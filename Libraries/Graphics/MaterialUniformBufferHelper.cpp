///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "MaterialUniformBufferHelper.hpp"

#include "Material.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "AppBuiltInTypes.hpp"
#include "Texture.hpp"

namespace Graphics
{

MaterialBufferData::MaterialBufferData()
{
  mBindingIndex = -1;
  mBufferType = BufferType::None;
}

void ExtractMaterialData(Model* model, TextureLibrary* textureLibrary, MaterialRendererData& outputData)
{
  ExtractMaterialData(model->mMaterial, textureLibrary, outputData);
}

void ExtractMaterialData(Material* material, TextureLibrary* textureLibrary, MaterialRendererData& outputData)
{
  /// Extract all of the uniform buffers from the material for this object
  outputData.mMaterialBuffers.Resize(Zero::FragmentType::Size);

  // First, get the buffer id's and sizes for each shader stage
  for(size_t fragIndex = 0; fragIndex < Zero::FragmentType::Size; ++fragIndex)
  {
    MaterialBufferData& materialBuffer = outputData.mMaterialBuffers[fragIndex];
    Zero::ShaderResourceReflectionData& stageReflectionData = material->mMaterialStageBindingData[fragIndex].mReflectionData;
    materialBuffer.mBufferType = BufferType::Uniform;
    materialBuffer.mBindingIndex = stageReflectionData.mBinding;
    materialBuffer.mBufferData.Resize(stageReflectionData.mSizeInBytes);
  }

  // Then for each material block, copy each property to the uniform buffer in the right locations
  for(size_t blockIndex = 0; blockIndex < material->mMaterialBlocks.Size(); ++blockIndex)
  {
    MaterialBlock* block = material->mMaterialBlocks[blockIndex];
    for(size_t propIndex = 0; propIndex < block->mPropertyList.Size(); ++propIndex)
    {
      MaterialProperty* materialProp = block->mPropertyList[propIndex];
      // If this property isn't actually a shader property for this material (fulfilled by another input) then skip it.
      if(materialProp->mValidReflectionObject == false)
        continue;

      // Handle images/samplers specially
      if(materialProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::SampledImage)
      {
        MaterialTextureProperty* textureProp = (MaterialTextureProperty*)materialProp;
        Texture* texture = textureLibrary->Find(textureProp->mResourceName);
        // Bind each texture id that was used
        for(size_t bindingIndex = 0; bindingIndex < textureProp->mBindingData.Size(); ++bindingIndex)
        {
          ShaderPropertyBindingData& bindingData = textureProp->mBindingData[bindingIndex];

          TextureData textureData;
          textureData.mTexture = texture;
          textureData.mTextureSlot = bindingData.mReflectionData.mBinding;
          outputData.mTextures.PushBack(textureData);
        }
        continue;
      }

      Zero::ZilchShaderIRRuntimeArrayType runtimeArrayType;
      if(runtimeArrayType.Load(materialProp->mShaderType))
      {
        MaterialSsboProperty* ssboProp = (MaterialSsboProperty*)materialProp;
        Zero::ShaderResourceReflectionData& reflectionData = ssboProp->mBindingData.mReflectionData;
        continue;
      }
      // Otherwise this is generic data, copy it to the relevant uniform buffer location
      else
      {
        MaterialDataProperty* dataProp = (MaterialDataProperty*)materialProp;
        Zero::ShaderResourceReflectionData& reflectionData = dataProp->mBindingData.mReflectionData;

        MaterialBufferData& materialBuffer = outputData.mMaterialBuffers[dataProp->mFragmentType];
        WriteProperty(materialBuffer.mBufferData, dataProp, dataProp->mBindingData);
      }
    }
  }
}

}//namespace Graphics

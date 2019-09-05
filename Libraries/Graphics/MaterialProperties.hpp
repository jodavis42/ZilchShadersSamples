///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ShaderProperties.hpp"
#include "ResourceLibrary.hpp"

namespace Graphics
{

//-------------------------------------------------------------------MaterialProperty
class MaterialProperty
{
public:
  MaterialProperty();
  virtual ~MaterialProperty() {};

  virtual MaterialProperty* Clone() const abstract;

  Zero::ZilchShaderIRType* mShaderType;
  Zero::FragmentType::Enum mFragmentType;
  String mPropertyName;
  bool mValidReflectionObject;
};

//-------------------------------------------------------------------MaterialDataProperty
class MaterialDataProperty : public MaterialProperty
{
public:
  MaterialProperty* Clone() const override;

  ShaderProperty mPropertyData;
  ShaderPropertyBindingData mBindingData;
};

//-------------------------------------------------------------------MaterialResourceProperty
class MaterialResourceProperty : public MaterialProperty
{
public:
  BaseResourceLibrary* mLibrary;
  String mResourceName;
};

//-------------------------------------------------------------------MaterialTextureProperty
class MaterialTextureProperty : public MaterialResourceProperty
{
public:
  virtual MaterialProperty* Clone() const;

  Array<ShaderPropertyBindingData> mBindingData;
};

template <typename PropertyType>
void WritePropertyData(ByteBuffer& buffer, const PropertyType& prop, const ShaderPropertyBindingData& bindingData)
{
  const Zero::ShaderResourceReflectionData& reflectionData = bindingData.mReflectionData;
  memcpy(buffer.Data() + reflectionData.mOffsetInBytes, &prop, reflectionData.mSizeInBytes);
}

inline void WritePropertyData(ByteBuffer& buffer, const byte* data, const ShaderPropertyBindingData& bindingData)
{
  const Zero::ShaderResourceReflectionData& reflectionData = bindingData.mReflectionData;
  memcpy(buffer.Data() + reflectionData.mOffsetInBytes, data, reflectionData.mSizeInBytes);
}

inline void WriteMatrixPropertyData(ByteBuffer& buffer, const MaterialDataProperty* dataProp, const ShaderPropertyBindingData& bindingData)
{
  const Zero::ShaderResourceReflectionData& reflectionData = bindingData.mReflectionData;
  Zero::ZilchShaderIRType* dataType = dataProp->mShaderType;
  size_t componentTypeSize = dataType->mComponentType->GetByteSize();

  // Copy each vector element out according to it's actual size and the stride of each vector. (e.g. Matrix3x3 is actually Matrix3x4)
  byte* destData = buffer.Data() + bindingData.mReflectionData.mOffsetInBytes;
  const byte* sourceData = dataProp->mPropertyData.Data();
  for(size_t i = 0; i < dataType->mComponents; ++i)
  {
    memcpy(destData + i * reflectionData.mStride, sourceData + i * componentTypeSize, componentTypeSize);
  }
}

/// Handles writing properties with varying sizes/strides according to the layout required by the shader.
inline void WriteProperty(ByteBuffer& buffer, const MaterialDataProperty* dataProp, const ShaderPropertyBindingData& bindingData)
{
  if(dataProp->mShaderType->mBaseType == Zero::ShaderIRTypeBaseType::Matrix)
    WriteMatrixPropertyData(buffer, dataProp, bindingData);
  else
    WritePropertyData(buffer, dataProp->mPropertyData.Data(), bindingData);
}

}//namespace Graphics

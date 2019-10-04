///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ShaderSerialization.hpp"

namespace Demo
{

//-------------------------------------------------------------------ShaderSerialization
ShaderSerialization::ShaderSerialization(JsonBuilder* serializer, SimpleZilchShaderIRGenerator* generator)
{
  mSerializer = serializer;
  mGenerator = generator;
}

void ShaderSerialization::SaveShaderReflection(ZilchShaderIRType* shaderType)
{
  SimplifiedShaderReflectionData* reflectionData = mGenerator->FindSimplifiedReflectionResult(shaderType);
  if(reflectionData == nullptr)
    return;

  mSerializer->Begin(Zilch::JsonType::Object);
  for(auto fragmentNames = reflectionData->mFragmentLookup.Keys(); !fragmentNames.Empty(); fragmentNames.PopFront())
  {
    String fragmentName = fragmentNames.Front();
    ZilchShaderIRType* fragmentType = mGenerator->FindFragmentType(fragmentName);

    SaveFragmentReflection(fragmentType, reflectionData);
  }
  mSerializer->End();
}

void ShaderSerialization::SaveFragmentReflection(ZilchShaderIRType* fragmentType, SimplifiedShaderReflectionData* reflectionData)
{
  ShaderIRTypeMeta* fragmentMeta = fragmentType->mMeta;
  mSerializer->Key(fragmentMeta->mZilchName);
  mSerializer->Begin(Zilch::JsonType::Object);
  for(auto fieldRange = fragmentMeta->mFields.All(); !fieldRange.Empty(); fieldRange.PopFront())
  {
    ShaderIRFieldMeta* fieldMeta = fieldRange.Front();
    SaveFragmentProperty(fragmentType, fieldMeta, reflectionData);
  }
  mSerializer->End();
}

void ShaderSerialization::SaveFragmentProperty(ZilchShaderIRType* fragmentType, ShaderIRFieldMeta* fieldMeta, SimplifiedShaderReflectionData* reflectionData)
{
  String propertyInputAttribute = mGenerator->mSettings->mNameSettings.mPropertyInputAttribute;
  Zero::ShaderIRAttribute* propertyAttribute = fieldMeta->mAttributes.FindFirstAttribute(propertyInputAttribute);
  if(propertyAttribute == nullptr)
    return;

  ZilchShaderIRType* propertyType = mGenerator->mShaderLibraryRef->FindType(fieldMeta->mZilchType->Name);

  // Make sure to get the actual property name (can be overridden in the attributes)
  String propertyName = fieldMeta->GetFieldAttributeName(propertyAttribute);
  Zero::ZilchShaderIRRuntimeArrayType runtimeArrayType;
  ZilchShaderIRImageType imageType;
  if(runtimeArrayType.Load(propertyType))
  {
    ShaderResourceReflectionData* resourceData = reflectionData->FindStructedStorageBuffer(fragmentType, propertyName);
    SavePropertyReflection(fieldMeta, resourceData);
  }
  else if(propertyType->mBaseType == Zero::ShaderIRTypeBaseType::SampledImage)
  {
    Array<ShaderResourceReflectionData*> resources;
    reflectionData->FindSampledImageReflectionData(fragmentType, propertyName, resources);
    SavePropertyReflection(fieldMeta, resources);
  }
  else if(imageType.Load(propertyType))
  {
    if(imageType.IsStorageImage())
    {
      ShaderResourceReflectionData* resourceData = reflectionData->FindStorageImage(fragmentType, propertyName);
      SavePropertyReflection(fieldMeta, resourceData);
    }
    else
    {
      Array<ShaderResourceReflectionData*> resources;
      reflectionData->FindImageReflectionData(fragmentType, propertyName, resources);
      SavePropertyReflection(fieldMeta, resources);
    }
  }
  else if(propertyType->mBaseType == Zero::ShaderIRTypeBaseType::Sampler)
  {
    Array<ShaderResourceReflectionData*> resources;
    reflectionData->FindSamplerReflectionData(fragmentType, propertyName, resources);
    SavePropertyReflection(fieldMeta, resources);
  }
  else
  {
    ShaderResourceReflectionData* resourceData = reflectionData->FindUniformReflectionData(fragmentType, propertyName);
    SavePropertyReflection(fieldMeta, resourceData);
  }
}

void ShaderSerialization::SavePropertyReflection(ShaderIRFieldMeta* propertyMeta, ShaderResourceReflectionData* resourceData)
{
  if(resourceData == nullptr)
    return;

  mSerializer->Key(propertyMeta->mZilchName);
  SavePropertyReflection(resourceData);
}

void ShaderSerialization::SavePropertyReflection(ShaderIRFieldMeta* propertyMeta, Array<ShaderResourceReflectionData*>& resourceData)
{
  if(resourceData.Empty())
    return;

  mSerializer->Key(propertyMeta->mZilchName);
  mSerializer->Begin(Zilch::JsonType::ArrayMultiLine);
  for(size_t i = 0; i < resourceData.Size(); ++i)
  {
    SavePropertyReflection(resourceData[i]);
  }
  mSerializer->End();
}

void ShaderSerialization::SavePropertyReflection(ShaderResourceReflectionData* resourceData)
{
  if(resourceData == nullptr)
    return;

  mSerializer->Begin(Zilch::JsonType::Object);

  mSerializer->Key("InstanceName");
  mSerializer->Value(resourceData->mInstanceName);

  mSerializer->Key("TypeName");
  mSerializer->Value(resourceData->mTypeName);

  mSerializer->Key("OffsetInBytes");
  mSerializer->Value(resourceData->mOffsetInBytes);

  mSerializer->Key("SizeInBytes");
  mSerializer->Value(resourceData->mSizeInBytes);

  mSerializer->Key("DescriptorSet");
  mSerializer->Value(resourceData->mDescriptorSet);

  mSerializer->Key("Binding");
  mSerializer->Value(resourceData->mBinding);

  mSerializer->Key("Location");
  mSerializer->Value(resourceData->mLocation);

  mSerializer->Key("Stride");
  mSerializer->Value(resourceData->mStride);

  mSerializer->End();
}

}//namespace Demo

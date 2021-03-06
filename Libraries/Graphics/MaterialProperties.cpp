///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Material.hpp"
#include "ShaderProperties.hpp"

namespace Graphics
{

//-------------------------------------------------------------------MaterialBlock
MaterialProperty::MaterialProperty()
{
  mFragmentType = Zero::FragmentType::None;
  mValidReflectionObject = false;
}

//-------------------------------------------------------------------MaterialDataProperty
MaterialProperty* MaterialDataProperty::Clone() const
{
  MaterialDataProperty* result = new MaterialDataProperty();
  result->mShaderType = mShaderType;
  result->mPropertyName = mPropertyName;
  result->mValidReflectionObject = mValidReflectionObject;;
  result->mBindingData = mBindingData;
  result->mPropertyData.CopyFrom(mPropertyData);
  return result;
}

//-------------------------------------------------------------------MaterialTextureProperty
MaterialProperty* MaterialTextureProperty::Clone() const
{
  MaterialTextureProperty* result = new MaterialTextureProperty();
  result->mShaderType = mShaderType;
  result->mPropertyName = mPropertyName;
  result->mLibrary = mLibrary;
  result->mValidReflectionObject = mValidReflectionObject;
  result->mBindingData = mBindingData;
  result->mResourceName = mResourceName;
  return result;
}

//-------------------------------------------------------------------MaterialSsboProperty
MaterialProperty* MaterialSsboProperty::Clone() const
{
  MaterialSsboProperty* result = new MaterialSsboProperty();
  *result = *this;
  return result;
}

}//namespace Graphics

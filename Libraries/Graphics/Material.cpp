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

MaterialProperty* MaterialTextureProperty::Clone() const
{
  MaterialTextureProperty* result = new MaterialTextureProperty();
  result->mShaderType = mShaderType;
  result->mPropertyName = mPropertyName;
  result->mValidReflectionObject = mValidReflectionObject;;
  result->mBindingData = mBindingData;
  result->mTextureName = mTextureName;
  return result;
}

//-------------------------------------------------------------------MaterialBlock
MaterialBlock::~MaterialBlock()
{
  Destroy();
}

void MaterialBlock::AddProperty(MaterialProperty* prop)
{
  mPropertyMap[prop->mPropertyName] = prop;
  mPropertyList.PushBack(prop);
}

MaterialProperty* MaterialBlock::FindProperty(const String& propertyName)
{
  return mPropertyMap.FindValue(propertyName, nullptr);
}

void MaterialBlock::CopyFrom(const MaterialBlock& rhs)
{
  Destroy();
  
  for(size_t i = 0; i < rhs.mPropertyList.Size(); ++i)
  {
    MaterialProperty* rhsProp = rhs.mPropertyList[i];
    MaterialProperty* prop = rhsProp->Clone();
    AddProperty(prop);
  }
}

void MaterialBlock::Destroy()
{
  for(size_t i = 0; i < mPropertyList.Size(); ++i)
    delete mPropertyList[i];
  mPropertyList.Clear();
  mPropertyMap.Clear();
}

//-------------------------------------------------------------------Material
Material::Material()
{
  mShader = nullptr;
}

Material::~Material()
{
  Destroy();
}

void Material::Destroy()
{
  for(size_t i = 0; i < mMaterialBlocks.Size(); ++i)
    delete mMaterialBlocks[i];
  mMaterialBlocks.Clear();
}

//-------------------------------------------------------------------MaterialLibrary
void MaterialLibrary::Destroy()
{
  __super::Destroy();
  for(auto range = mFragmentBlockTemplates.Values(); !range.Empty(); range.PopFront())
  {
    MaterialBlock* block = range.Front();
    delete block;
  }
  mFragmentBlockTemplates.Clear();
}

}//namespace Graphics

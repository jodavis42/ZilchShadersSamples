///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Material.hpp"
#include "ShaderProperties.hpp"

namespace Graphics
{

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

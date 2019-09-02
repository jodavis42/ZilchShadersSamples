///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ShaderProperties.hpp"

namespace Graphics
{

//-------------------------------------------------------------------ShaderProperty
ShaderProperty::ShaderProperty()
{
  mPropertyType = ShaderPropertyType::None;
}

//-------------------------------------------------------------------ShaderPropertyList
ShaderPropertyList::ShaderPropertyList()
{

}

ShaderPropertyList::ShaderPropertyList(const ShaderPropertyList& rhs)
{
  CopyFrom(rhs);
}

ShaderPropertyList::ShaderPropertyList(ShaderPropertyList&& rhs)
{
  mProperties.Resize(rhs.mProperties.Size());
  for(size_t i = 0; i < mProperties.Size(); ++i)
    mProperties[i] = rhs.mProperties[i];
  rhs.mProperties.Clear();
}

ShaderPropertyList::~ShaderPropertyList()
{
  Clear();
}

void ShaderPropertyList::CopyFrom(const ShaderPropertyList& rhs)
{
  Clear();
  mProperties.Reserve(rhs.mProperties.Size());
  for(size_t i = 0; i < rhs.mProperties.Size(); ++i)
  {
    ShaderProperty* rhsProp = rhs.mProperties[i];
    ShaderProperty* prop = new ShaderProperty();
    prop->CopyFrom(*rhsProp);
    mProperties[i] = prop;
  }
}

void ShaderPropertyList::Clear()
{
  for(size_t i = 0; i < mProperties.Size(); ++i)
    delete mProperties[i];
  mProperties.Clear();
}

//-------------------------------------------------------------------ShaderPropertyList
ShaderPropertyBindingData::ShaderPropertyBindingData()
{
  
}

}//namespace Graphics

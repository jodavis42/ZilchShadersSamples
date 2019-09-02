///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ByteBuffer.hpp"

namespace Graphics
{

//-------------------------------------------------------------------ShaderPropertyType
struct ShaderPropertyType
{
  enum Enum
  {
    None,
    Bool, Integer, Float,
    Vector2, Vector3, Vector4,
    Matrix2x2, Matrix3x3, Matrix4x4,
    SampledImage2D
  };
};

//-------------------------------------------------------------------ShaderProperty
class ShaderProperty : public ByteBuffer
{
public:
  ShaderProperty();
  ShaderPropertyType::Enum mPropertyType;
  String mPropertyName;
};

//-------------------------------------------------------------------ShaderPropertyList
class ShaderPropertyList
{
public:
  ShaderPropertyList();
  ShaderPropertyList(const ShaderPropertyList& rhs);
  ShaderPropertyList(ShaderPropertyList&& rhs);
  ~ShaderPropertyList();

  void CopyFrom(const ShaderPropertyList& rhs);
  void Clear();

  Array<ShaderProperty*> mProperties;
};

//-------------------------------------------------------------------ShaderPropertyList
class ShaderPropertyBindingData
{
public:
  ShaderPropertyBindingData();

  
  Zero::ShaderResourceReflectionData mReflectionData;
};

}//namespace Graphics

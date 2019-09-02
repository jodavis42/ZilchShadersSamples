///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ShaderProperties.hpp"

namespace Graphics
{

class Mesh;
class Material;

//-------------------------------------------------------------------Model
class Model
{
public:
  Model();

  Vector3 mScale;
  Matrix3 mRotation;
  Vector3 mTranslation;

  Mesh* mMesh;
  Material* mMaterial;
  ShaderPropertyList mPropertyOverrides;
  String mName;
};

}//namespace Graphics

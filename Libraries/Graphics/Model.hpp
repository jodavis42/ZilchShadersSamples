///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ShaderProperties.hpp"
#include "Component.hpp"

namespace Graphics
{

class Mesh;
class Material;

//-------------------------------------------------------------------Model
class Model : public Engine::Component
{
public:
  Model();

  void Serialize(Serializer& serializer) override;

  Mesh* mMesh;
  Material* mMaterial;
  ShaderPropertyList mPropertyOverrides;
};

//-------------------------------------------------------------------ComputePass
class ComputePass : public Engine::Component
{
public:
  ComputePass();

  void Serialize(Serializer& serializer) override;

  Material* mMaterial;
  Integer3 mDispatchSize;
};

}//namespace Graphics

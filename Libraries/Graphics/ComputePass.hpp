///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

#include "ShaderProperties.hpp"
#include "Component.hpp"

namespace Graphics
{

class Material;

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

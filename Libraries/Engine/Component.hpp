///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

#include "CompositionInitializer.hpp"

namespace Engine
{

class Composition;
class Cog;
class Serializer;

//-------------------------------------------------------------------Component
class Component
{
public:
  Component();
  virtual ~Component() {}

  virtual void Initialize(const CompositionInitializer& initializer) {}
  virtual void Serialize(Serializer& serializer) {}
  virtual void Update(float dt) {}

  Cog* GetOwner() const;

  Cog* mOwner;
};

}//namespace Engine

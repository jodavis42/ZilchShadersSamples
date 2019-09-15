///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

#include "Composition.hpp"

namespace Engine
{

class Space;

//-------------------------------------------------------------------Cog
class Cog : public Composition
{
public:
  Cog();
  virtual ~Cog();

  void AddComponent(const String& componentName, Component* component) override;

  Space* GetSpace();

  Space* mSpace;
};

}//namespace Engine

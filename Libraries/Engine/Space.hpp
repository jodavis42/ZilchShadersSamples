///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

#include "Cog.hpp"

namespace Engine
{

class Cog;

//-------------------------------------------------------------------Space
class Space : public Cog
{
public:
  Space();
  virtual ~Space();

  void Space::AddComponent(const String& componentName, Component* component) override;

  void Add(Cog* cog);
  void DestroyCogs();

  Array<Cog*> mCogs;
};

}//namespace Engine

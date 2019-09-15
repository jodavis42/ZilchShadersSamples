///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Cog.hpp"

namespace Engine
{

//-------------------------------------------------------------------Cog
Cog::Cog()
{
  mSpace = nullptr;
}

Cog::~Cog()
{
}

void Cog::AddComponent(const String& componentName, Component* component)
{
  __super::AddComponent(componentName, component);
  component->mOwner = this;
}

Space* Cog::GetSpace()
{
  return mSpace;
}

}//namespace Engine

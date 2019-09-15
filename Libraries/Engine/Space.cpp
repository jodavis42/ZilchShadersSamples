///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Space.hpp"

#include "Cog.hpp"

namespace Engine
{

//-------------------------------------------------------------------Space
Space::Space()
{
  mSpace = this;
}

Space::~Space()
{
  DestroyCogs();
}

void Space::AddComponent(const String& componentName, Component* component)
{
  __super::AddComponent(componentName, component);
  component->mOwner = this;
}

void Space::Add(Cog* cog)
{
  cog->mSpace = this;
  mCogs.PushBack(cog);
}

void Space::DestroyCogs()
{
  for(auto range = mCogs.All(); !range.Empty(); range.PopFront())
    delete range.Front();
  mCogs.Clear();
}

}//namespace Engine

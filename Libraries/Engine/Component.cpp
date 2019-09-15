///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Component.hpp"

namespace Engine
{

//-------------------------------------------------------------------Component
Component::Component()
{
  mOwner = nullptr;
}

Cog* Component::GetOwner() const
{
  return mOwner;
}

}//namespace Engine

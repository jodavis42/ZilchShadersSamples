///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

//-------------------------------------------------------------------ResourceLibrary
template <typename ResourceType>
class ResourceLibrary
{
public:
  virtual ~ResourceLibrary();

  virtual void Add(const String& resourceName, ResourceType* resource);
  virtual ResourceType* Find(const String& resourceName);
  virtual ResourceType* GetDefault();
  virtual void Destroy();

  HashMap<String, ResourceType*> mResourceMap;
};

}//namespace Graphics

#include "ResourceLibrary.inl"

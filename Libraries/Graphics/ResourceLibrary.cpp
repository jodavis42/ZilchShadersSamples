///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ResourceLibrary.hpp"

namespace Graphics
{

//-------------------------------------------------------------------BaseResourceLibrary
BaseResourceLibrary::~BaseResourceLibrary()
{
  Destroy();
}

void BaseResourceLibrary::Destroy()
{
  for(auto range = mResourceMap.Values(); !range.Empty(); range.PopFront())
  {
    Resource* resource = range.Front();
    delete resource;
  }
  mResourceMap.Clear();
}

void BaseResourceLibrary::AddInternal(const String& resourceName, Resource* resource)
{
  mResourceMap.InsertOrError(resourceName, resource);
}

Resource* BaseResourceLibrary::FindInternal(const String& resourceName)
{
  return mResourceMap.FindValue(resourceName, nullptr);
}

Resource* BaseResourceLibrary::GetDefaultInternal()
{
  if(mResourceMap.Empty())
    return nullptr;

  Resource* result = mResourceMap.FindValue(mDefaultName, nullptr);
  if(result == nullptr)
    result = mResourceMap.All().Front().second;
  return result;
}

}//namespace Graphics

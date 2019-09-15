///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Resource.hpp"
#include "Serializer.hpp"

namespace Engine
{

//-------------------------------------------------------------------BaseResourceLibrary
void Resource::Serialize(Serializer& serializer)
{
  serializer.SerializeField(mName);
}

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

String BaseResourceLibrary::VirtualGetLibraryName()
{
  return String();
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

void BaseResourceLibrary::SerializeResourceInternal(Serializer& serializer, Resource* resource)
{
  resource->Serialize(serializer);

  bool isDefault = false;
  serializer.SerializeNamedField("Default", isDefault);
  if(isDefault)
    mDefaultName = resource->mName;
}

}//namespace Engine

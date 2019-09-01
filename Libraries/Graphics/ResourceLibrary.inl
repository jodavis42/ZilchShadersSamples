///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////

namespace Graphics
{

//-------------------------------------------------------------------ResourceLibrary
template <typename ResourceType>
ResourceLibrary<ResourceType>::~ResourceLibrary()
{
  Destroy();
}

template <typename ResourceType>
void ResourceLibrary<ResourceType>::Add(const String& resourceName, ResourceType* resource)
{
  mResourceMap.InsertOrError(resourceName, resource);
}

template <typename ResourceType>
ResourceType* ResourceLibrary<ResourceType>::Find(const String& resourceName)
{
  return mResourceMap.FindValue(resourceName, nullptr);
}

template <typename ResourceType>
ResourceType* ResourceLibrary<ResourceType>::GetDefault()
{
  if(mResourceMap.Empty())
    return nullptr;
  return mResourceMap.All().Front().second;
}

template <typename ResourceType>
void ResourceLibrary<ResourceType>::Destroy()
{
  for(auto range = mResourceMap.Values(); !range.Empty(); range.PopFront())
  {
    ResourceType* resource = range.Front();
    delete resource;
  }
  mResourceMap.Clear();
}

}//namespace Graphics

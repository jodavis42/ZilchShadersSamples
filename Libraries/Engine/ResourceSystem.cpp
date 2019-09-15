///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "ResourceSystem.hpp"

namespace Engine
{

//-------------------------------------------------------------------ResourceSystem
void ResourceSystem::RegisterLibrary(const String& libraryName, BaseResourceLibrary* library)
{
  mLibraryMap.InsertOrError(libraryName, library);
}

BaseResourceLibrary* ResourceSystem::FindLibrary(const String& libraryName) const
{
  return mLibraryMap.FindValue(libraryName, nullptr);
}

void ResourceSystem::Destroy()
{
  for(auto range = mLibraryMap.Values(); !range.Empty(); range.PopFront())
    delete range.Front();
  mLibraryMap.Clear();
}

}//namespace Engine

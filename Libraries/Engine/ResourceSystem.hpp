///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

#include "Resource.hpp"

namespace Engine
{

//-------------------------------------------------------------------ResourceSystem
/// Simple system to store all available resource libraries to make it easier to fetch.
class ResourceSystem
{
public:
  void RegisterLibrary(const String& libraryName, BaseResourceLibrary* library);
  BaseResourceLibrary* FindLibrary(const String& libraryName) const;

  template <typename ResourceLibraryType>
  ResourceLibraryType* FindTypedResourceLibrary(const String& libraryName) const
  {
    return (ResourceLibraryType*)FindLibrary(libraryName);
  }

  void Destroy();

  HashMap<String, BaseResourceLibrary*> mLibraryMap;
};

#define RegisterResourceLibrary(ResourceLibraryType) RegisterLibrary(#ResourceLibraryType, new ResourceLibraryType())
#define HasResourceLibrary(ResourceLibraryType) FindTypedResourceLibrary<ResourceLibraryType>(#ResourceLibraryType)

}//namespace Engine

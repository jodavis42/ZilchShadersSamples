///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "FrameworkStandard.hpp"
#include "Resource.hpp"

//-------------------------------------------------------------------Level
class Project : public Engine::Resource
{
public:

  String mFilePath;
};

//-------------------------------------------------------------------LevelLibrary
class ProjectLibrary : public Engine::ResourceLibrary<Project>
{
public:
  DeclareResourceLibraryType(ProjectLibrary);
};

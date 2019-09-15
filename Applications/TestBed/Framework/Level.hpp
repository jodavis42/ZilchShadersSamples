///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once


#include "FrameworkStandard.hpp"
#include "Resource.hpp"

//-------------------------------------------------------------------Level
class Level : public Engine::Resource
{
public:

  String mFilePath;
};

//-------------------------------------------------------------------LevelLibrary
class LevelLibrary : public Engine::ResourceLibrary<Level>
{
public:
  DeclareResourceLibraryType(LevelLibrary);
};

///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"
#include "Resource.hpp"

namespace Graphics
{

//-------------------------------------------------------------------Shader
class Shader : public Engine::Resource
{
public:
  String mShaderSource[Zero::FragmentType::Size];
};

//-------------------------------------------------------------------ShaderLibrary
class ShaderLibrary : public Engine::ResourceLibrary<Shader>
{

};

}//namespace Graphics

///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"
#include "ResourceLibrary.hpp"

namespace Graphics
{

//-------------------------------------------------------------------Shader
class Shader : public Resource
{
public:
  String mName;
  String mShaderSource[Zero::FragmentType::Size];
};

//-------------------------------------------------------------------ShaderLibrary
class ShaderLibrary : public ResourceLibrary<Shader>
{

};

}//namespace Graphics

///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "DemoUtilitiesStandard.hpp"


namespace Demo
{

using namespace Zero;
using namespace Zilch;

//-------------------------------------------------------------------ShaderSerialization
class ShaderSerialization
{
public:
  ShaderSerialization(JsonBuilder* serializer, SimpleZilchShaderIRGenerator* generator);

  void SaveShaderReflection(ZilchShaderIRType* shaderType);
  void SaveFragmentReflection(ZilchShaderIRType* fragmentType, SimplifiedShaderReflectionData* reflectionData);
  void SaveFragmentProperty(ZilchShaderIRType* fragmentType, ShaderIRFieldMeta* fieldMeta, SimplifiedShaderReflectionData* reflectionData);
  void SavePropertyReflection(ShaderIRFieldMeta* propertyMeta, ShaderResourceReflectionData* resourceData);
  void SavePropertyReflection(ShaderIRFieldMeta* propertyMeta, Array<ShaderResourceReflectionData*>& resourceData);
  void SavePropertyReflection(ShaderResourceReflectionData* resourceData);

  JsonBuilder* mSerializer;
  SimpleZilchShaderIRGenerator* mGenerator;
};

}//namespace Demo

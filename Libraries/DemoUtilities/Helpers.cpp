///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Callbacks.hpp"
#include "JsonSerializer.hpp"

namespace Demo
{

void LoadShaderDef(const String& filePath, String& shaderName, Array<String>& fragmentNames)
{
  Engine::JsonSerializer serializer;
  serializer.Load(filePath);

  serializer.SerializeNamedField("Name", shaderName);
  size_t blockCount = serializer.BeginArray("MaterialBlocks");
  if(blockCount != 0)
  {
    for(size_t i = 0; i < blockCount; ++i)
    {
      serializer.BeginArrayItem(i);
      String& fragmentName = fragmentNames.PushBack();
      serializer.SerializeNamedField("Name", fragmentName);
      serializer.End();
    }
    serializer.End();
  }
}

}//namespace Demo

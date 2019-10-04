///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Serializer.hpp"

namespace Engine
{

//-------------------------------------------------------------------StreamDirection
StreamDirection::Enum StreamDirection::FromString(const String& enumName)
{
  if(enumName == "Loading")
    return Loading;
  else if(enumName == "Saving")
    return Saving;
  return None;
}

String StreamDirection::ToString(Enum enumValue)
{
  if(enumValue == Loading)
    return "Loading";
  else if(enumValue == Saving)
    return "Saving";
  return "None";
}

//-------------------------------------------------------------------Serializer
Serializer::Serializer()
{
  mDirection = StreamDirection::None;
  mResourceSystem = nullptr;
}

String Serializer::CleanupName(const String& name)
{
  if(name.StartsWith("m"))
    return name.SubStringFromByteIndices(1, name.SizeInBytes());
  return name;
}

}//namespace Engine

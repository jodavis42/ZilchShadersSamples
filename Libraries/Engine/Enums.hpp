///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

namespace Engine
{

#define DeclareEnumType(EnumType)                     \
EnumType() { mEnumValue = None;  }                    \
explicit EnumType(EnumType::Enum value) { mEnumValue = value; } \
void operator=(EnumType::Enum value) { mEnumValue = value; }    \
operator EnumType::Enum() { return mEnumValue; }                \
EnumType::Enum mEnumValue;

}//namespace Engine

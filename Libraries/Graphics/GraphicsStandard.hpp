///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix3.hpp"
#include "Math/Matrix4.hpp"
#include "Common/CommonStandard.hpp"
#include "String/String.hpp"
#include "Zilch.hpp"
#include "ZilchShadersStandard.hpp"
#include "EngineStandard.hpp"
#include "Engine/Enums.hpp"

namespace Engine
{
class Component;
class Composition;
class Serializer;
class Space;
class Cog;
class ResourceSystem;
}

namespace Graphics
{
using Math::Vector2;
using Math::Vector3;
using Math::Vector4;
using Integer2 = Math::IntVec2;
using Integer3 = Math::IntVec3;
using Integer4 = Math::IntVec4;
using Math::Matrix2;
using Math::Matrix3;
using Math::Matrix4;
using Math::Quaternion;
using Zero::String;
using Zero::Array;
using Zero::HashMap;

using Zero::ZilchShaderIRType;
using Zero::ZilchShaderIRBackend;
using Engine::Component;
using Engine::Composition;
using Engine::Serializer;
using Engine::Cog;
using Engine::Space;
using Engine::ResourceSystem;
}//namespace Graphics

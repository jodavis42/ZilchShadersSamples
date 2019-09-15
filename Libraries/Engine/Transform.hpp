///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineStandard.hpp"

#include "Component.hpp"

namespace Engine
{

//-------------------------------------------------------------------Transform
class Transform : public Component
{
public:
  Transform();

  void Serialize(Serializer& serializer) override;

  Matrix4 GetWorldMatrix() const;

  Vector3 mScale;
  Quaternion mRotation;
  Vector3 mTranslation;
};

}//namespace Engine

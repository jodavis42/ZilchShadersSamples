///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

#include "Transform.hpp"
#include "Serializer.hpp"

namespace Engine
{

//-------------------------------------------------------------------Transform
Transform::Transform()
{
  mScale = Vector3(1, 1, 1);
  mRotation.SetIdentity();
  mTranslation = Vector3::cZero;
}

void Transform::Serialize(Serializer& serializer)
{
  __super::Serialize(serializer);
  serializer.SerializeField(mScale);
  serializer.SerializeField(mRotation);
  serializer.SerializeField(mTranslation);
}

Matrix4 Transform::GetWorldMatrix() const
{
  return Matrix4::GenerateTransform(mTranslation, mRotation, mScale);
}

}//namespace Engine

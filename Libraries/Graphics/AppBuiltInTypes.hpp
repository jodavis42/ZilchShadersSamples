///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

//-------------------------------------------------------------------FrameBufferData
class FrameBufferData
{
public:
  float mLogicTime;
  float mFrameTime;
};

//-------------------------------------------------------------------CameraBufferData
class CameraBufferData
{
public:
  float mNearPlane;
  float mFarPlane;
  Vector2 mViewportSize;
};

//-------------------------------------------------------------------TransformBufferData
class TransformBufferData
{
public:
  Matrix4 mLocalToWorld;
  Matrix4 mWorldToView;
  Matrix4 mViewToPerspective;
  Matrix4 mPerspectiveToApiPerspective;
};

//-------------------------------------------------------------------TransformBufferData
/// Used for particle system compute shaders
struct ParticleData
{
  Vector4 mPosition;
  Vector4 mVelocity;
};

}//namespace Graphics

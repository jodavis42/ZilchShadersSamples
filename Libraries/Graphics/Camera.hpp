///////////////////////////////////////////////////////////////////////////////
/// Authors: Joshua Davis
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GraphicsStandard.hpp"

namespace Graphics
{

//-------------------------------------------------------------------Camera
class Camera
{
public:
  Camera();

  // Change the camera mode between Fps and Orbit
  int GetCameraMode();
  void SetCameraMode(const int& mode);

  Matrix4 BuildCameraMatrix(const Vector3& forward, const Vector3& up, const Vector3& right, const Vector3& translation);

  void SetMatrix();
  void SetMatrixOrbit();
  void SetMatrixFps();
  Vector3 GetDirection();

  void ProcessMouseInput(int button, bool isDown, int x, int y);
  void ProcessMouseMovement(int x, int y);
  void OnMouseScroll(int x, int y);
  void ProcessKeyboardInput(unsigned int key, int x, int y);
  void ProcessKeyUp(unsigned int key, int x, int y);

  bool mRightMouseIsDown;
  float mPhi;
  float mTheta;
  float mRadius;
  int mLastX;
  int mLastY;

  enum Mode { Orbit, Fps };
  Mode mMode;
  enum MovementDir { Left, Right, Down, Up, PanGlobalUp, PanGlobalDown, NumKeys };
  bool mKeyStates[6];

  Vector3 mTarget;
  Vector3 mTranslation;

  float mNearPlane;
  float mFarPlane;
  float mAspectRatio;
  Matrix4 mWorldToView;
  Matrix4 mViewToPerspective;
};

}//namespace Graphics

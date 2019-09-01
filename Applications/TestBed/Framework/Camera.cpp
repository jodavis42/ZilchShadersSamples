///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
//#include "Precompiled.hpp"
#include "Camera.hpp"

#include "Math/Math.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
//#include "SimplePropertyBinding.hpp"

Camera::Camera()
{
  mTarget = Vector3::cZero;
  mTranslation = Vector3::cZero;
  mRightMouseIsDown = false;
  mPhi = Math::cPi * 0.5f;
  mTheta = Math::cPi * 0.5f;
  mRadius = 5;
  mLastX = mLastY = 0;
  mMode = Orbit;

  for(int i = 0;  i < NumKeys; ++i)
    mKeyStates[i] = false;
}

int Camera::GetCameraMode()
{
  return mMode;
}

void Camera::SetCameraMode(const int& mode)
{
  int newMode = mode;
  if(newMode == mMode)
    return;

  mMode = (Mode)newMode;

  mPhi = Math::cPi - mPhi;
  mTheta = Math::cPi - mTheta;
  if(newMode == Orbit)
  {
    mTarget = mTranslation - GetDirection();
  }
}

Matrix4 Camera::BuildCameraMatrix(const Vector3& forward, const Vector3& up, const Vector3& right, const Vector3& translation)
{
  Vector3 f = -forward.Normalized();
  Vector3 u = up.Normalized();
  Vector3 r = right.Normalized();

  Matrix4 transform;
  transform.SetBasis(0, r.x, r.y, r.z, 0);
  transform.SetBasis(1, u.x, u.y, u.z, 0);
  transform.SetBasis(2, f.x, f.y, f.z, 0);
  transform.SetBasis(3, 0, 0, 0, 1);

  Vector3 t = Math::TransposedTransform(Math::ToMatrix3(transform), -translation);
  transform.SetCross(3, t.x, t.y, t.z, 1);
  return transform;
}


float Cot(float angle)
{
  return Math::Tan(Math::cPi * 0.5f - angle);
}

void BuildPerspectiveTransformGl(Matrix4& matrix, float verticalFov, float aspectRatio, float nearDistance, float farDistance)
{
  matrix = Matrix4::cIdentity;

  // Near and far distances are expected to be positive
  float depth = farDistance - nearDistance;
  if(depth < 0.0001f || aspectRatio < 0.0001f)
    return;

  // horizontal+ (fixed vertical fov)
  //
  //            /|  |
  //          /  | top
  // fov/2__/    |  |
  //      /_|____|  |
  //      --near--

  // tan(fov/2) = t/n
  // n/t = Math::Cot(fov/2)
  float n_t = Cot(verticalFov * 0.5f);

  // r = t*(r/t) = t*aspect
  // n/r = n/(t*aspect) = (n/t)/aspect
  float n_r = n_t / aspectRatio;

  matrix.m00 = n_r;
  matrix.m11 = n_t;
  matrix.m22 = -(farDistance + nearDistance) / depth;
  matrix.m33 = 0.0f;
  matrix.m23 = -2.0f * farDistance * nearDistance / depth;
  matrix.m32 = -1.0f;
}

void Camera::SetMatrix()
{
  if(mMode == Orbit)
    SetMatrixOrbit();
  else
    SetMatrixFps();
  
  Matrix4 perspective;
  float fov = Math::DegToRad(45);
  BuildPerspectiveTransformGl(perspective, fov, mAspectRatio, mNearPlane, mFarPlane);
  mViewToPerspective = perspective;
}

void Camera::SetMatrixOrbit()
{
  Vector3 worldUp = Vector3(0, 1, 0);
  
  Vector3 cameraDir;
  cameraDir.x = mRadius * Math::Cos(mTheta) * Math::Sin(mPhi);
  cameraDir.y = mRadius * Math::Cos(mPhi);
  cameraDir.z = mRadius * Math::Sin(mTheta) * Math::Sin(mPhi);

  // Since we're looking at our target, the forward vector is just the
  // opposite of our position vector on the unit sphere
  Vector3 forward = -cameraDir;
  
  // Compute the forward and right vector of the camera in world space
  Vector3 movementForward = forward;
  movementForward.y = 0;
  movementForward = Math::Normalized(movementForward);
  Vector3 movementRight = Vector3(-movementForward.z, 0.0f, movementForward.x);

  // Build up the total world space movement of the camera
  Vector3 movementW = Vector3::cZero;
  // Move the target point along the camera's forward and right projected onto the camera's x-z plane 
  if(mKeyStates[Up])
  {
    movementW += movementForward;
  }
  else if(mKeyStates[Down])
  {
    movementW -= movementForward;
  }
  if(mKeyStates[Left])
  {
    movementW -= movementRight;
  }
  else if(mKeyStates[Right])
  {
    movementW += movementRight;
  }
  if(mKeyStates[PanGlobalDown])
    movementW -= worldUp;
  else if(mKeyStates[PanGlobalUp])
    movementW += worldUp;

  // Scale the movement by sensitivity
  movementW = movementW * 0.1f;//this.PanSensitivity;

  // Compute the basis of the camera
  Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
  // Use the world up and camera's forward to get the right vector
  Vector3 right = Math::Cross(forward, up);
  right = Math::Normalized(right);
  // Now use the camera forward and right to get the camera's actual up vector
  up = Math::Cross(right, forward);
  up = Math::Normalized(up);

  mTarget += movementW;
  Vector3 eye = cameraDir + mTarget;
  mTranslation = eye;

  Matrix4 transform = BuildCameraMatrix(forward, up, right, mTranslation);
  mWorldToView = transform;
}

void Camera::SetMatrixFps()
{
  Vector3 worldUp = Vector3(0, 1, 0);

  Vector3 cameraDir;
  cameraDir.x = mRadius * Math::Cos(mTheta) * Math::Sin(mPhi);
  cameraDir.y = mRadius * Math::Cos(mPhi);
  cameraDir.z = mRadius * Math::Sin(mTheta) * Math::Sin(mPhi);
  
  Vector3 forward = cameraDir;
  forward = Math::Normalized(forward);

  // Compute the forward and right vector of the camera in world space
  Vector3 movementForward = forward;
  Vector3 movementRight = Math::Cross(forward, worldUp);
  movementRight.Normalize();

  Vector3 movementW = Vector3::cZero;
  // Move the target point along the camera's forward and right projected onto the camera's x-z plane 
  if(mKeyStates[Up])
  {
    movementW += movementForward;
  }
  else if(mKeyStates[Down])
  {
    movementW -= movementForward;
  }
  if(mKeyStates[Left])
  {
    movementW -= movementRight;
  }
  else if(mKeyStates[Right])
  {
    movementW += movementRight;
  }
  if(mKeyStates[PanGlobalDown])
    movementW -= worldUp;
  else if(mKeyStates[PanGlobalUp])
    movementW += worldUp;

  // Scale the movement by sensitivity
  movementW = movementW * 0.1f;//this.PanSensitivity;
  mTranslation += movementW;

  // Now use the camera forward and right to get the camera's actual up vector
  Vector3 up = Math::Cross(movementRight, forward);
  up = Math::Normalized(up);

  Vector3 eye = mTranslation;
  mTarget = eye + cameraDir;
  Vector3 right = movementRight;

  Matrix4 transform = BuildCameraMatrix(forward, up, right, mTranslation);
  mWorldToView = transform;
}

Vector3 Camera::GetDirection()
{
  Vector3 cameraDir;
  cameraDir.x = mRadius * Math::Cos(mTheta) * Math::Sin(mPhi);
  cameraDir.y = mRadius * Math::Cos(mPhi);
  cameraDir.z = mRadius * Math::Sin(mTheta) * Math::Sin(mPhi);
  return cameraDir;
}

void Camera::ProcessMouseInput(int button, bool isDown, int x, int y)
{
  if(button == MouseButtons::Right)
  {
    mRightMouseIsDown = isDown;
  }
}

void Camera::ProcessMouseMovement(int x, int y)
{
  if(mRightMouseIsDown == true)
  {
    int deltaX = x - mLastX;
    int deltaY = y - mLastY;
    if(mMode == Orbit)
      deltaY *= -1;

    mPhi = Math::Clamp(mPhi + deltaY * 0.01f, Math::DegToRad(5), Math::DegToRad(175));
    mTheta += deltaX * 0.01f;
  }

  mLastX = x;
  mLastY = y;
}

void Camera::OnMouseScroll(int x, int y)
{
  float radius = mRadius - y;
  mRadius = Math::Clamp(radius, 0.01f, 100.0f);
}

void Camera::ProcessKeyboardInput(unsigned int key, int x, int y)
{
  if(key == Keys::A)
    mKeyStates[Left] = true;
  if(key == Keys::D)
    mKeyStates[Right] = true;
  if(key == Keys::W)
    mKeyStates[Up] = true;
  if(key == Keys::S)
    mKeyStates[Down] = true;
  if(key == Keys::Q)
    mKeyStates[PanGlobalDown] = true;
  if(key == Keys::E)
    mKeyStates[PanGlobalUp] = true;
}

void Camera::ProcessKeyUp(unsigned int key, int x, int y)
{
  if(key == Keys::A)
    mKeyStates[Left] = false;
  if(key == Keys::D)
    mKeyStates[Right] = false;
  if(key == Keys::W)
    mKeyStates[Up] = false;
  if(key == Keys::S)
    mKeyStates[Down] = false;
  if(key == Keys::Q)
    mKeyStates[PanGlobalDown] = false;
  if(key == Keys::E)
    mKeyStates[PanGlobalUp] = false;
}

#pragma once

#include "FrameworkStandard.hpp"

namespace MouseButtons { enum Enum { Left, Middle, Right, Count }; };
namespace MouseButtonStates {enum Enum { Down, Up };};

class Mouse;

//-----------------------------------------------------------------------------MouseButton
class MouseButton
{
public:
  MouseButton();
  MouseButtonStates::Enum mState;
};

//-----------------------------------------------------------------------------MouseEvent
class MouseEvent
{
public:
  ZilchDeclareType(MouseEvent, Zilch::TypeCopyMode::ReferenceType);
  MouseEvent();

  /// The button that triggered this event.
  MouseButtons::Enum GetButton() const;
  /// The current state of the button
  bool GetIsDown() const;

  MouseButtons::Enum mButton;
  MouseButtonStates::Enum mState;
  Mouse* mMouse;
};

//-----------------------------------------------------------------------------Mouse
class Mouse
{
public:
  ZilchDeclareType(Mouse, Zilch::TypeCopyMode::ReferenceType);

  Mouse();
  ~Mouse();

  bool IsButtonDown(MouseButtonStates::Enum button);

  void ProcessButton(int button, bool isDown);

  MouseButton mButtons[MouseButtons::Count];
  Vector2 mPosition;
  Vector2 mMovement;
  Vector2 mScroll;
};

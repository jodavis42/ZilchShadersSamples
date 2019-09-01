//#include "Precompiled.hpp"

#include "Mouse.hpp"

//-----------------------------------------------------------------------------MouseButton
MouseButton::MouseButton()
{
  mState = MouseButtonStates::Up;
}

//-----------------------------------------------------------------------------MouseEvent
ZilchDefineType(MouseEvent, builder, type)
{
  ZilchBindDefaultConstructor();
  ZilchBindDestructor();

  ZilchBindGetter(Button);
  ZilchBindGetter(IsDown);
}

MouseEvent::MouseEvent()
{
  mButton = MouseButtons::Left;
  mMouse = nullptr;
}

MouseButtons::Enum MouseEvent::GetButton() const
{
  return mButton;
}

bool MouseEvent::GetIsDown() const
{
  return mState == MouseButtonStates::Down;
}

//-----------------------------------------------------------------------------Mouse
ZilchDefineType(Mouse, builder, type)
{
  type->HandleManager = ZilchManagerId(Zilch::PointerManager);
  ZilchBindDefaultConstructor();
  ZilchBindDestructor();

  ZilchBindMethod(IsButtonDown);

  ZilchBindFieldGetter(mPosition);
  ZilchBindFieldGetter(mScroll);
  ZilchBindFieldGetter(mMovement);
}

Mouse::Mouse()
{
  mPosition = Vector2::cZero;
  mScroll = Vector2::cZero;
  mMovement = Vector2::cZero;
}

Mouse::~Mouse()
{

}

bool Mouse::IsButtonDown(MouseButtonStates::Enum button)
{
  MouseButton& buttonState = mButtons[(int)button];
  return buttonState.mState == MouseButtonStates::Down;
}

void Mouse::ProcessButton(int button, bool isDown)
{
  MouseButton& mouseButton = mButtons[button];
  MouseButtonStates::Enum buttonState = isDown ? MouseButtonStates::Down : MouseButtonStates::Up;

  if(mouseButton.mState != buttonState)
  {
    mouseButton.mState = buttonState;

    MouseEvent toSend;
    toSend.mMouse = this;
    toSend.mButton = (MouseButtons::Enum)button;
    toSend.mState = buttonState;

    String eventName = "MouseDown";
    if(!isDown)
      eventName = "MouseUp";

    //Zilch::EventSend(this, eventName, &toSend);
  }
}


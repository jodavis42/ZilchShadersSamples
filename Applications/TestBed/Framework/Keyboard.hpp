#pragma once

#include "FrameworkStandard.hpp"

class Keyboard;

namespace Keys
{
enum Enum
{
  Num0 = '0',
  Num1 = '1',
  Num2 = '2',
  Num3 = '3',
  Num4 = '4',
  Num5 = '5',
  Num6 = '6',
  Num7 = '7',
  Num8 = '8',
  Num9 = '9',

  A = 'A',
  B = 'B',
  C = 'C',
  D = 'D',
  E = 'E',
  F = 'F',
  G = 'G',
  H = 'H',
  I = 'I',
  J = 'J',
  K = 'K',
  L = 'L',
  M = 'M',
  N = 'N',
  O = 'O',
  P = 'P',
  Q = 'Q',
  R = 'R',
  S = 'S',
  T = 'T',
  U = 'U',
  V = 'V',
  W = 'W',
  X = 'X',
  Y = 'Y',
  Z = 'Z',

  Tilde = '`',
  Comma = ',',
  Period = '.',
  LeftBracket = '[',
  RightBracket = ']',
  Semicolon = ';',

  Up = 128,
  Down,
  Left,
  Right,

  Space = ' ',
  Tab,
  Enter,
  Shift,
  Control,
  Alt
};
};


//-----------------------------------------------------------------------------KeyState
class KeyState
{
public:
  KeyState();

  bool IsDown() const;
  bool IsUp() const;

  bool mIsDown;
};

//-----------------------------------------------------------------------------KeyboardEvent
class KeyboardEvent : public Zilch::EventData
{
public:
  ZilchDeclareType(KeyboardEvent, Zilch::TypeCopyMode::ReferenceType);
  KeyboardEvent();

  /// The key that triggered this event.
  Keys::Enum GetKey() const;
  /// The current state of the key
  bool GetIsDown() const;

  int mKey;
  KeyState mKeyState;
  Keyboard* mKeyboard;
};

//-----------------------------------------------------------------------------Keyboard
class Keyboard : public Zilch::EventHandler
{
public:
  ZilchDeclareType(Keyboard, Zilch::TypeCopyMode::ReferenceType);

  Keyboard();
  ~Keyboard();

  bool IsKeyDown(Keys::Enum key);
  bool IsKeyUp(Keys::Enum key);

  KeyState& GetKeyState(Keys::Enum key);

  void ProcessKey(int key, bool isDown);

  KeyState mKeys[255];
};

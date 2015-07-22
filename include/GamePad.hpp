#pragma once
#include "gamepad.h"

class GamePad
{
public:
  GamePad();
  void update();
  float getLeftStickVal() const;
  float getRightStickVal() const;
};

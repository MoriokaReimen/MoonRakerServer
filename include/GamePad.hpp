#pragma once
#include "gamepad.h"
#include <cmath>

class GamePad
{
public:
  GamePad();
  virtual ~GamePad();
  void update();
  float getLeftStickVal() const;
  float getRightStickVal() const;
};

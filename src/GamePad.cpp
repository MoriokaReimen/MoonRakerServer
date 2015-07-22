#include "GamePad.hpp"

GamePad::GamePad()
{
  GamepadInit();
  return;
}

void GamePad::update()
{
  GamepadUpdate();
  return;
}

float GamePad::getLeftStickVal() const
{
  float lx,ly;
  GamepadStickNormXY(GAMEPAD_0, STICK_LEFT, &lx, &ly);
  return ly;
}

float GamePad::getRightStickVal() const
{
  float rx,ry;
  GamepadStickNormXY(GAMEPAD_0, STICK_RIGHT, &rx, &ry);
  return ry;
}

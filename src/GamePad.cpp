#include "GamePad.hpp"

GamePad::GamePad()
{
  GamepadInit();
  GamepadSetRumble(GAMEPAD_0, 0.25f, 0.25f);
  return;
}

GamePad::~GamePad()
{
  GamepadShutdown();
  return;
}

void GamePad::update()
{
  GamepadUpdate();
  return;
}

float GamePad::getLeftStickVal() const
{
  float x,y;
  GamepadStickNormXY(GAMEPAD_0, STICK_LEFT, &x, &y);
  return std::floor(y + 0.5);
}

float GamePad::getRightStickVal() const
{
  float x,y;
  GamepadStickNormXY(GAMEPAD_0, STICK_RIGHT, &x, &y);
  return std::floor(y + 0.5);
}

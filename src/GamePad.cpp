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
  float length;
  length = GamepadStickLength(GAMEPAD_0, STICK_LEFT);
  return std::round(length);
}

float GamePad::getRightStickVal() const
{
  float length;
  length = GamepadStickLength(GAMEPAD_0, STICK_RIGHT);
  return std::round(length);
}

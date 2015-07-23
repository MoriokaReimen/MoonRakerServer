/*!
-----------------------------------------------------------------------------
@file    GamePad.cpp
----------------------------------------------------------------------------
         @@
       @@@@@@
      @```@@@@
     @`  `@@@@@@
   @@`   `@@@@@@@@
  @@`    `@@@@@@@@@           Tohoku University
  @` `   `@@@@@@@@@       SPACE ROBOTICS LABORATORY
  @`` ## `@@@@@@@@@    http://www.astro.mech.tohoku.ac.jp/
  @` #..#`@@@@@@@@@        Planetary Robotics Group
  @` #..#`@@@@@@@@@
  @` ### `@@@@@@@@@          Professor Kazuya Yoshida
  @` ###``@@@@@@@@@      Associate Professor Keiji Nagatani
   @### ``@@@@@@@@
   ###  ` @@@@@@@
  ###  @  @@@@@                 Creation Date:
 ###    @@@@@               @date July. 24. 2015
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%             @author Kei Nakata
   #### %%%                  menschenjager.mark.neun@gmail.com
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------
@brief gamepad wrapper class
-----------------------------------------------------------------------------
*/
#include "GamePad.hpp"

/*!
 * @brief Constructor for MotorData class
 */
GamePad::GamePad()
{
  GamepadInit();
  GamepadSetRumble(GAMEPAD_0, 0.25f, 0.25f);
  return;
}

/*!
 * @brief Destructor for MotorData class
 */
GamePad::~GamePad()
{
  GamepadShutdown();
  return;
}

/*!
 * @brief call this function before every extractin gamepad state
 */
void GamePad::update()
{
  GamepadUpdate();
  return;
}

/*!
 * @brief get left stick state
 * @return value 1 or 0 or -1
 */
float GamePad::getLeftStickVal() const
{
  float x,y;
  GamepadStickNormXY(GAMEPAD_0, STICK_LEFT, &x, &y);
  return std::round(y);
}

/*!
 * @brief get right stick state
 * @return value 1 or 0 or -1
 */
float GamePad::getRightStickVal() const
{
  float x,y;
  GamepadStickNormXY(GAMEPAD_0, STICK_RIGHT, &x, &y);
  return std::round(y);
}

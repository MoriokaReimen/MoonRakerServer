/*!
-----------------------------------------------------------------------------
@file    MoonRaker.hpp
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
 ###    @@@@@               @date Jan. 1. 2015
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
@brief MoonRaker facade class
-----------------------------------------------------------------------------
*/
#pragma once
#include "Motor.hpp"
#include "Command.hpp"
#include "Data.hpp"

class WheelState
{
public:
  rpm;
  torque;
};
/*! @class Motor
 *  @brief handles serial communication between motor
*/
class RoverState
{
public:
  start;
  now;
  WheelState left_front;
  WheelState left_rear;
  WheelState right_front;
  WheelState right_rear;
  //Quaternion quat; //! implement someday
};

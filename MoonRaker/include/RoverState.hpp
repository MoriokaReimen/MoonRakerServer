/*!
-----------------------------------------------------------------------------
@file    RoverState.hpp
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
#include "Data.hpp"
#include <chrono>

class WheelState
{
public:
    double rpm {0};
    double torque {0};
};

/*! @class Motor
 *  @brief handles serial communication between motor
*/
class RoverState
{
    const double GEAR_RATIO
    {
        690.0f
    };
    const double GEAR_EFFICIENCY
    {
        0.49f
    };
    const double TORQUE_CONSTANT
    {
        0.00902f * GEAR_RATIO * GEAR_EFFICIENCY
    }; //! in [mNm/mA]
    const double PI
    {
        3.14159265359
    };
public:
    void set(const MotorData& left, const MotorData& right);
    long long time {0};
    WheelState left_front;
    WheelState left_rear;
    WheelState right_front;
    WheelState right_rear;
    //Quaternion quat; implement someday
};

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
#include "Data.hpp"
#include <chrono>

class WheelState
{
public:
  double rpm{0};
  double current{0};
};

/*! @class Motor
 *  @brief handles serial communication between motor
*/
class RoverState
{
  const double GEAR_RATIO = 690.0f;
  const double GEAR_EFFICIENCY = 0.49f;
  const double TORQUE_CONSTANT = 0.00902f * GEAR_RATIO * GEAR_EFFICIENCY; //! in [mNm/mA]
  const double PI = 3.14159265359;
public:
  void set(const MotorData& left, const MotorData& right);
  long long now{0};
  WheelState left_front;
  WheelState left_rear;
  WheelState right_front;
  WheelState right_rear;
  //Quaternion quat; implement someday
};

void set(const MotorData& left, const MotorData& right)
{
  /* set rpm */
  this->left_front.rpm = left.front_rpm / GEAR_RATIO;
  this->left_rear.rpm = left.rear_rpm / GEAR_RATIO;
  this->right_front.rpm = right.front_rpm / GEAR_RATIO;
  this->right_rear.rpm = right.rear_rpm / GEAR_RATIO;

  /* set torque */
  this->left_front.torque = left.front_current * TORQUE_CONSTANT;
  this->left_rear.torque = left.rear_current * TORQUE_CONSTANT;
  this->right_front.torque = right.front_current * TORQUE_CONSTANT;
  this->right_rear.torque = right.rear_current * TORQUE_CONSTANT;

  /* set time */
  this->now = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();

  return;
}

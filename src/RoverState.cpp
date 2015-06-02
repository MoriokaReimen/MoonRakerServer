/*!
-----------------------------------------------------------------------------
@file    RoverState.cpp
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
@brief hold rover state class
-----------------------------------------------------------------------------
*/
#include "RoverState.hpp"

/*! @function set
 *  @brief set Rover state with Motordata
*/
void RoverState::set(const MotorData& left, const MotorData& right)
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
    this->time = std::chrono::duration_cast<std::chrono::milliseconds>
                 (std::chrono::system_clock::now().time_since_epoch()).count();

    return;
}

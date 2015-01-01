/*!
-----------------------------------------------------------------------------
@file    data.cpp
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
@brief data manipulation class
-----------------------------------------------------------------------------
*/
#include "data.hpp"

/*!
 * @brief Constructor for MotorData class
 * @param[in] raw bits array from serial port.
 */
MotorData::MotorData(const DataBytes& data)
{
  if(data.device == 0x11) this->device = "L";
  if(data.device == 0x12) this->device = "R";
  this -> rear_current  = static_cast<signed short>(data.rear_current);
  this -> front_current = static_cast<signed short>(data.front_current);
  this -> rear_rpm  = static_cast<signed short>(data.rear_rpm);
  this -> front_rpm  = static_cast<signed short>(data.front_rpm);
  this -> battery_v  = static_cast<signed short>(data.front_rpm);
  this -> time  = static_cast<uint32_t>(data.time);

  return;
}


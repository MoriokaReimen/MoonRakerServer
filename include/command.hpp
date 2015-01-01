#pragma once
/*!
-----------------------------------------------------------------------------
@file    command.hpp
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
 ###    @@@@@               @date Dec. 29. 2014
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
@brief command manipulation class
-----------------------------------------------------------------------------
*/
#include <stdexcept>
#include <cmath>
#include <cstdint>

struct CommandBytes
{
    const uint8_t head_base = 0x75; //! Header/Footer base byte
    const uint8_t header = 0xAA; //! Header byte
    const uint8_t device = 0x13; //! Source device 0x13 = PCBoard
    //(0x11 = left, 0x12 = right)
    const int16_t boost_flag = 0x00;
    int16_t left_rpm = 0;        //! Left Motor RPM Byte
    int16_t right_rpm = 0;        //! Right Motor RPM Byte
    const uint8_t foot_base = 0x75; //! Header/Footer base byte
    const uint8_t footer = 0xFF; //! Footer byte
} __attribute__((__packed__));

class MotorCommand
{
  signed short left_rpm{0};
  signed short right_rpm{0};
  signed short max_rpm_{4000};

public:
  MotorCommand(const signed short& left, const signed short& right);
  CommandBytes toByteArray();
};

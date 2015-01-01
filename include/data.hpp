/*!
-----------------------------------------------------------------------------
@file    data.hpp
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
#pragma once
#include <cstdint>
#include <string>
using std::string;

struct DataBytes
{
    const uint8_t head_base = 0x75; //! Header/Footer base byte
    const uint8_t header = 0xAA; //! Header byte
    uint8_t device = 0x00; //! Source device (0x11 = left, 0x12 = right)
    int16_t rear_current = 0; //! rear Current
    int16_t front_current = 0; //! front Current
    int16_t rear_rpm = 0; //! rear Motor RPM
    int16_t front_rpm = 0; //! front Motor RPM
    uint16_t battery_v = 0;        //! Battery Level milliVolts
    uint32_t time = 0;         //! Time stamp Byte
    const uint8_t foot_base = 0x75; //! Header/Footer base byte
    const uint8_t footer = 0xFF; //! Footer byte
} __attribute__((__packed__));

class MotorData
{
public:
  int rear_current{0};
  int front_current{0};
  int front_rpm{0};
  int rear_rpm{0};
  int battery_v{0};
  long time{0};
  string device{"O"};
  MotorData(const DataBytes& data);
};


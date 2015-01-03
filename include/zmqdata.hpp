/*!
-----------------------------------------------------------------------------
@file    zmqdata.hpp
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
 ###    @@@@@               @date Jan. 2. 2015
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
@brief published/subscribe data manipulation class
-----------------------------------------------------------------------------
*/
#pragma once
#include <cstdint>
#include <portable_endian.h>

/*!
 * @struct ZMQBytes
 * @brief wrapper for data excheanged through zmq socket bytes array
*/
#pragma pack(1)
struct ZMQBytes
{
    const uint16_t header = 0xAA75; //! Header byte
    uint32_t time = 0;         //! Time stamp Byte
    int16_t left_front_rpm = 0; //! front Motor RPM
    int16_t left_rear_rpm = 0; //! rear Motor RPM
    int16_t right_front_rpm = 0; //! front Motor RPM
    int16_t right_rear_rpm = 0; //! rear Motor RPM
    int16_t left_front_current = 0; //! front Current
    int16_t left_rear_current = 0; //! rear Current
    int16_t right_front_current = 0; //! front Current
    int16_t right_rear_current = 0; //! rear Current
    uint16_t battery_v = 0;        //! Battery Level milliVolts
    const uint16_t footer = 0xFF75; //! Footer byte
} __attribute__((__packed__));
#pragma pack()

/*!
 * @class ZMQData
 * @brief wrapper class for data exchanged through zmq socket
*/
class ZMQData
{
public:
  long time = 0;         //! Time stamp Byte
  int left_rear_rpm = 0; //! rear Motor RPM
  int left_front_rpm = 0; //! front Motor RPM
  int right_rear_rpm = 0; //! rear Motor RPM
  int right_front_rpm = 0; //! front Motor RPM
  int left_rear_current = 0; //! rear Current
  int left_front_current = 0; //! front Current
  int right_rear_current = 0; //! rear Current
  int right_front_current = 0; //! front Current
  int battery_v = 0;        //! Battery Level milliVolts
  ZMQData() = default;
  ZMQData(const ZMQBytes& data);
  void set(const ZMQBytes& data);
  ZMQBytes toByteArray() const;
};

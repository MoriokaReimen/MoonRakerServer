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
#include "Math3D/Math3D.hpp"
#include "Data.hpp"
#include <chrono>
#include <cstring>

/*!
 * @struct StateBytes
 * @brief wrapper for StateBytes bytes array
*/
#if __BYTE_ORDER == __BIG_ENDIAN
#pragma pack(1)
struct StateBytes {
    const uint16_t header = 0x75AA; //! Header bytes
    uint64_t time{0};
    float roll{0};
    float pitch{0};
    float yaw{0};
    float left_front_rpm{0};
    float left_rear_rpm{0};
    float right_front_rpm{0};
    float right_rear_rpm{0};
    float left_front_torque{0};
    float left_rear_torque{0};
    float right_front_torque{0};
    float right_rear_torque{0};
    const uint16_t footter = 0x75FF; //! Footer byte
} __attribute__((__packed__));
#pragma pack()
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#pragma pack(1)
struct StateBytes {
    const uint16_t header = 0xAA75; //! Header bytes
    uint64_t time{0};
    float roll{0};
    float pitch{0};
    float yaw{0};
    float left_front_rpm{0};
    float left_rear_rpm{0};
    float right_front_rpm{0};
    float right_rear_rpm{0};
    float left_front_torque{0};
    float left_rear_torque{0};
    float right_front_torque{0};
    float right_rear_torque{0};
    const uint16_t footter = 0xFF75; //! Footer byte
} __attribute__((__packed__));
#pragma pack()
#endif

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
    static constexpr double GEAR_RATIO{690.0f};
    static constexpr double GEAR_EFFICIENCY{0.49f};
    static constexpr double TORQUE_CONSTANT{0.00902f * GEAR_RATIO * GEAR_EFFICIENCY}; //! in [mNm/mA]
public:
    inline RoverState() {};
    RoverState(const StateBytes& bytes);
    void set(const MotorData& left, const MotorData& right);
    void set(const MotorData& left, const MotorData& right, const Math3D::Quaternion& quat);
    void set(const StateBytes& bytes);
    StateBytes toByteArray() const;
    long long time {0};
    WheelState left_front;
    WheelState left_rear;
    WheelState right_front;
    WheelState right_rear;
    Math3D::Quaternion quat;
};

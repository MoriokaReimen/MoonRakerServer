/*!
-----------------------------------------------------------------------------
@file    Motor.hpp
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
@brief motor serial communication manipulation class
-----------------------------------------------------------------------------
*/
#pragma once
#include "BoostComPort.hpp"
#include "Command.hpp"
#include "Data.hpp"
#include <stdexcept>
#include <cstring>
#include <thread>
#include <chrono>
using std::runtime_error;
using bit8 = unsigned char;

/*! @class Motor
 *  @brief handles serial communication between motor
*/
class Motor
{
    BoostComPort serial;

public:
    Motor();
    ~Motor() = default;
    bool work(const MotorCommand& command, MotorData& left, MotorData& right);
    void halt();
    int sendCommand(const MotorCommand& command);
    MotorData getData();
};

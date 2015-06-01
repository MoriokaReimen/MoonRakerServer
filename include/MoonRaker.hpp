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

/*! @class Motor
 *  @brief handles serial communication between motor
*/
class MoonRaker
{
    Motor motor_;
    Logger logger_;
public:
    MoonRaker(std::string filename)
      : logger_(filename)
    {
      return;
    }

    ~MoonRaker()
    {
      motor_.halt();
      return;
    }

    run(const Command& command)
    {
      try{
        motor_.work(command, left_data, right_data);
      }
    }
};

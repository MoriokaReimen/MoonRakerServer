/*
-----------------------------------------------------------------------------
Filename:    Logger.hpp
-----------------------------------------------------------------------------
         @@
       @@@@@@
      @```@@@@
     @`  `@@@@@@
   @@`   `@@@@@@@@
  @@`    `@@@@@@@@@           Tohoku University
  @` `   `@@@@@@@@@       SPACE ROBOTICS LABORATORY
  @`` ## `@@@@@@@@@    http://www.astro.mech.tohoku.ac.jp/
  @` #..#`@@@@@@@@@        Planetary Robotics Group @` #..#`@@@@@@@@@
  @` ### `@@@@@@@@@          Professor Kazuya Yoshida
  @` ###``@@@@@@@@@      Associate Professor Keiji Nagatani
   @### ``@@@@@@@@
   ###  ` @@@@@@@
  ###  @  @@@@@                 Creation Date:
 ###    @@@@@                    Spring 2013
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%                   Kei Nakata
   #### %%%                 nakata@astro.mech.tohoku.ac.jp
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------
@brief class for logging MotorData to file
-----------------------------------------------------------------------------
*/
#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include "RoverState.hpp"
#include "Math3D/Math3D.hpp"

/*! @class Logger
 *  @brief handles log routine
*/
class Logger
{
    std::ofstream file_;
public:
    Logger(const std::string& filne_name);
    ~Logger();
    void log(const RoverState& data);
};

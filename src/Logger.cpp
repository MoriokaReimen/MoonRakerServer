/*
-----------------------------------------------------------------------------
Filename:    Logger.cpp
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

-----------------------------------------------------------------------------
*/
#include "Logger.hpp"


/*CONSTRUCTOR
*/
Logger::Logger(const std::string& file_name)
{
  file_.open(file_name);
  return;
}


/*DESTRUCTOR
~MotorUnit:
    Clean up memory by deleting all objects in memory that were
assigned through a pointer.
*/
Logger::~Logger()
{
  file_.close();

  return;
}

bool Logger::log(const MotorData& data)
{
  file_ << data.device << ",";
  file_ << data.time << ",";
  file_ << data.front_rpm << ",";
  file_ << data.rear_rpm << ",";
  file_ << data.front_current << ",";
  file_ << data.rear_current << ",";
  file_ << data.battery_v << "\n";
  return true;
}

/*!
-----------------------------------------------------------------------------
@file    Remote.hpp
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
@brief Simple Console for controling MoonRaker
-----------------------------------------------------------------------------
*/
#pragma once
#include "Command.hpp"
#include "RoverState.hpp"
#include "Socket/UDP.hpp"
#include <cstring>
#include <algorithm>
#include <exception>
#include <thread>
#include <mutex>
#include <cereal/archives/portable_binary.hpp>

class Remote : private UDP
{
  std::mutex command_mutex_;
  std::mutex socket_mutex_;
  std::thread worker_thread_;
  void doTask_();
  MotorCommand command_;
  bool isEnd_{false};
public:
  Remote(const std::string& address);
  ~Remote();
  void sendCommand(const MotorCommand& command);
  void sendData(const RoverState& data);
  MotorCommand getCommand();
  RoverState getData();
};

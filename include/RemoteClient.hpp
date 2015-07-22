/*!
-----------------------------------------------------------------------------
@file    RemoteClient.hpp
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

class RemoteClient : public UDP
{
  std::mutex state_mutex_;
  std::mutex socket_mutex_;
  std::thread worker_thread_;
  void doTask_();
  RoverState state_;
  bool isEnd_{false};
public:
  RemoteClient(const std::string& address);
  ~RemoteClient();
  void sendCommand(const MotorCommand& command);
  RoverState getData();
};

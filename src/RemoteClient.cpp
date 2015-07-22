/*!
-----------------------------------------------------------------------------
@file    RemoteClient.cpp
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
#include "RemoteClient.hpp"

RemoteClient::RemoteClient(const std::string& address)
  : UDP(2015)
{
  this->setTarget(address, 2015);
  worker_thread_ = std::thread(&RemoteClient::doTask_, this);
  return;
}

RemoteClient::~RemoteClient()
{
  return;
}

void RemoteClient::sendCommand(const MotorCommand& command)
{
  std::string serialized = command.serialize();
  this->write(serialized);
  return;
}

RoverState RemoteClient::getData()
{
  RoverState state;
  std::lock_guard<std::mutex> lock(state_mutex_);
  state = this->state_;
  return state;
}

void RemoteClient::doTask_()
{
  while(true)
  {
    /*! read from udp */
    std::string message = this->read();

    /*! Detect Headers '$' and footers ';' */
    auto first = message.find_last_of('$');
    auto last = message.find_last_of(';');

    /* check received data */
    if(first == std::string::npos || last == std::string::npos)
        continue;

    if(first > last) continue;

    auto serialized = message.substr(first, last - first);

    RoverState state;
    state.deserialize(serialized);

    {
      std::lock_guard<std::mutex> lock(state_mutex_);
      this->state_ = state;
    }

  }
}

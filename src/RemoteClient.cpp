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
  //worker_thread_ = std::thread(&RemoteClient::doTask_, this);
  return;
}

RemoteClient::~RemoteClient()
{
  this->isEnd_ = true;
  worker_thread_.join();
  return;
}

void RemoteClient::sendCommand(const MotorCommand& command)
{
  std::string serialized = command.serialize();
  this->socket_mutex_.lock();
  this->write(serialized);
  this->socket_mutex_.unlock();
  return;
}

RoverState RemoteClient::getData()
{
  RoverState state;
  this->state_mutex_.lock();
  state = this->state_;
  this->state_mutex_.unlock();
  return state;
}

void RemoteClient::doTask_()
{
  while(true)
  {
    /*! read from udp */
    this->socket_mutex_.lock();
    std::string message = this->read();
    this->socket_mutex_.unlock();

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

    this->state_mutex_.lock();
    this->state_ = state;
    this->state_mutex_.unlock();

    if(isEnd_) return;
  }
}

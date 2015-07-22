/*!
-----------------------------------------------------------------------------
@file    RemoteServer.cpp
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
#include "RemoteServer.hpp"

RemoteServer::RemoteServer(const std::string& address)
  : UDP(2015)
{
  this->setTarget(address, 2015);
  worker_thread_ = std::thread(&RemoteServer::doTask_, this);
  return;
}

RemoteServer::~RemoteServer()
{
  this->worker_thread_.detach();
  this->close();
  return;
}

void RemoteServer::sendData(const RoverState& data)
{
  auto serialized = data.serialize();
  this->write(serialized);
  return;
}

MotorCommand RemoteServer::getCommand()
{
  MotorCommand command;
  std::lock_guard<std::mutex> lock(command_mutex_);
  command = this->command_;

  return command;
}

void RemoteServer::doTask_()
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

    MotorCommand command;
    command.deserialize(serialized);

    {
      std::lock_guard<std::mutex> lock(command_mutex_);
      this->command_ = command;
    }
  }
}

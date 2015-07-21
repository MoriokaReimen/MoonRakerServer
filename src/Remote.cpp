/*!
-----------------------------------------------------------------------------
@file    Remote.cpp
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
#include "Remote.hpp"

Remote::Remote(const std::string& address)
  : UDP(2015)
{
  this->setTarget(address, 2015);
  worker_thread_ = std::thread(&Remote::doTask_, this);
  return;
}

Remote::~Remote()
{
  this->isEnd_ = true;
  worker_thread_.join();
  return;
}

void Remote::sendCommand(const MotorCommand& command)
{
  std::stringstream ss;
  cereal::PortableBinaryOutputArchive oarchive(ss);
  oarchive(command);
  std::string serialized;
  ss >> serialized;
  serialized = "$" + serialized +";";
  this->socket_mutex_.lock();
  this->write(serialized);
  this->socket_mutex_.unlock();
  return;
}

void Remote::sendData(const RoverState& data)
{
  auto bytes = data.toByteArray();
  unsigned char buffer[50];
  memcpy(buffer, &bytes, sizeof(bytes));
  this->write(buffer, sizeof(bytes) + 2);
  return;
}

MotorCommand Remote::getCommand()
{
  MotorCommand command;
  this->command_mutex_.lock();
  command = this->command_;
  this->command_mutex_.unlock();
  return command;
}

RoverState Remote::getData()
{
  StateBytes bytes;
  unsigned char buffer[80];

  /*! read 19 bytes from serial */
  this->read(buffer, 50);

  /*! Detect Headers and footers */
  for (int i = 0; i < 50; ++i) {
      if ( (buffer[i + 1]== 0xAA) && (buffer[i+sizeof(StateBytes) - 2] == 0x75) ) {
          //! motor data array
          memcpy(&bytes, &buffer[i], sizeof(bytes));
          return RoverState(bytes);
      }
  }
  throw std::runtime_error("Broken data"); //! data is broken
}

void Remote::doTask_()
{
  while(true)
  {
    /*! read from udp */
    this->socket_mutex_.lock();
    std::string message = this->read();
    this->socket_mutex_.unlock();

    /*! Detect Headers "$" and footers ";" */
    size_t first{0}, last{0};
    first = message.find_last_of("$");
    last = message.find_last_of(";");

    /* check received data */
    if(first == std::string::npos || last == std::string::npos) continue;
    if(first > last) continue;
    auto buff = message.substr(first + 1, last - first - 1);

    std::stringstream ss;
    ss << buff;
    cereal::PortableBinaryInputArchive iarchive(ss);
    MotorCommand command;
    iarchive(command);

    this->command_mutex_.lock();
    this->command_ = command;
    this->command_mutex_.unlock();

    if(isEnd_) return;
  }
}

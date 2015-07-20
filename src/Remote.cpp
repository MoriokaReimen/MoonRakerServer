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
  return;
}

void Remote::sendCommand(const MotorCommand& command)
{
  std::string serialized = command.serialize();
  this->write(serialized);
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
  /*! read from udp */
  std::string message = this->read();

  /* debugging code */
  std::cout << message << std::endl;

  /*! Detect Headers "$" and footers ";" */
  size_t first{0}, last{0};
  first = message.find_last_of("$");
  last = message.find_last_of(";");
  auto buff = message.substr(first, last - first);
  /* check data */
  if(std::count(buff.begin(), buff.end(),',') != 3)
    throw std::runtime_error("Broken UDP Command"); //! message is broken
  MotorCommand command(buff);
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

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
#include "Command.hpp"
#include "RoverState.hpp"
#include "Socket/UDP.hpp"
#include <cstring>

class Remote : private UDP
{
public:

  Remote(const std::string& address)
    : UDP(2015)
  {
    this->setTarget(address, 2015);
    return;
  }

  void sendCommand(const MotorCommand& command)
  {
    CommandBytes bytes {command.toRemoteByteArray()};
    unsigned char buffer[25];
    memcpy(buffer, &bytes, sizeof(bytes));
    this->write(buffer, sizeof(bytes) + 2);
    return;
  }

  void sendData(const RoverState& data);
  {
    return;
  }

  MotorCommand getCommand()
  {
    CommandBytes bytes;
    unsigned char buffer[40];

    /*! read 19 bytes from serial */
    this->read(buffer, 25);

    /*! Detect Headers and footers */
    for (int i = 0; i < 25; ++i) {
        if ( (buffer[i + 1]== 0xAA) && (buffer[i+sizeof(CommandBytes) - 2] == 0x75) ) {
            //! motor data array
            memcpy(&bytes, &buffer[i], sizeof(bytes));
            return MotorCommand(bytes);
        }
    }
    throw runtime_error("Broken data"); //! data is broken
  }

  RoverState getData();
};

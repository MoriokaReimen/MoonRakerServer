/*!
-----------------------------------------------------------------------------
@file    motor.cpp
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
@brief motor serial communication manipulation class
-----------------------------------------------------------------------------
*/
#include "motor.hpp"
using std::runtime_error;


/*!
 * @brief Constructor for MotorData class
 */
Motor::Motor()
{
  serial.open("/dev/ttyS0", 115200);
}


/*!
 * @brief send command to the motor
 * @param command[in] command for the motor
 */
int Motor::sendCommand(const MotorCommand& command)
{
    CommandBytes bytes {command.toByteArray()};
    char buffer[sizeof(bytes)];
    memcpy(buffer, &bytes, sizeof(bytes));
    serial.write(buffer, sizeof(bytes));
    return EXIT_SUCCESS;
}

/*!
 * @brief get data from the motor
 * @return data as MotorData class
 * @exception runtime_error when no data
 * @exception runtime_error when broken data
 */
MotorData Motor::getData()
{
    DataBytes bytes;
    char buffer[40];
    char pattern[2] = {0x75, 0xFF};
    //serial.readUntil(reinterpret_cast<char*>(buffer), 40, pattern, 2);
    serial.read(buffer, 40, true, 100);
    serial.poll();

    //! Detect Headers and footers
    for (int i = 0; i < 40; ++i) {
        if (((unsigned char)buffer[i] == 0x75) && ((unsigned char)buffer[i + 1] == 0xAA) &&
            ((unsigned char)buffer[i + 17] == 0x75) && ((unsigned char)buffer[i + 18] == 0xFF)) {
            //! motor data array
            memcpy(&bytes, &buffer[i], sizeof(bytes));
            return MotorData(bytes);
        }
    }
    throw runtime_error("Broken data"); //! data is broken
}

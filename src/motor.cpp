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
    unsigned char buffer[sizeof(bytes)];
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
    unsigned char buffer[40];
    unsigned char pattern[1] = {0xFF};
    serial.readUntil(buffer, sizeof(bytes) + 1, pattern, 1, true, 20);

    //! Detect Headers and footers
    for (int i = 0; i < 40; ++i) {
        if ( (buffer[i+1]== 0xAA) && (buffer[i+18] == 0xFF) )
        {
            //! motor data array
            memcpy(&bytes, &buffer[i], sizeof(bytes));
            return MotorData(bytes);
        }
    }
    throw runtime_error("Broken data"); //! data is broken
}

bool Motor::work(const MotorCommand& command, MotorData& left, MotorData& right)
{
  /* initialize sleep function */
  std::chrono::milliseconds interval(5);

  this->sendCommand(command);
  std::this_thread::sleep_for(interval);
  left = this->getData();
  std::this_thread::sleep_for(interval);
  right = this ->getData();
  serial.clear();
  serial.clearBuffers();
  return true;
}
void Motor::halt()
{
  this->sendCommand(MotorCommand(0, 0));
  this->sendCommand(MotorCommand(0, 0));
  return;
}

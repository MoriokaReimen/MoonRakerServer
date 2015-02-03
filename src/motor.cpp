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
using bit8 = unsigned char;


/*!
 * @brief Constructor for MotorData class
 */
Motor::Motor() : serial(115200, "/dev/ttyS0")
{
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
    serial.sendData(buffer, sizeof(bytes));
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
    auto data = serial.getData("0xFF0x75");
    decltype(data) buff[data.size()];

    //! Detect Headers and footers
    for (auto c = data.size(); c >= 0; --c) {
        if (((data[c-1] == (char)0xAA) && data[c] == (char)0x75))
       {
            //! motor data array
              //memcpy(reinterpret_cast<char*>(bytes), (buff+c), sizeof(bytes));
              memcpy(&bytes, (buff+c), sizeof(bytes));
              return MotorData(bytes);
      }
    }
    throw runtime_error("Broken data"); //! data is broken
}

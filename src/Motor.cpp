/*!
-----------------------------------------------------------------------------
@file    Motor.cpp
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
#include "Motor.hpp"
using std::runtime_error;


/*!
 * @brief Constructor for MotorData class
 */
Motor::Motor()
{
    serial.open("/dev/ttyS0", 38400);
}


/*!
 * @brief send command to the motor
 * @param[in] command command for the motor
 */
int Motor::sendCommand(const MotorCommand& command)
{
    CommandBytes bytes {command.toByteArray()};
    unsigned char buffer[25];
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

    /*! read 19 bytes from serial */
    serial.read(buffer, 25, true, 10);

    /*! Detect Headers and footers */
    for (int i = 0; i < 25; ++i) {
        if ( (buffer[i + 1]== 0xAA) && (buffer[i+sizeof(DataBytes) - 2] == 0x75) ) {
            //! motor data array
            memcpy(&bytes, &buffer[i], sizeof(bytes));
            return MotorData(bytes);
        }
    }
    throw runtime_error("Broken data"); //! data is broken
}

/*!
 * @brief send command and get data from the motor
 * @param[in] command to be send to motor controller
 * @param[out] left motor data
 * @param[out] right motor data
 * @return true
 */
bool Motor::work(const MotorCommand& command, MotorData& left, MotorData& right)
{
    /*! initialize sleep function */
    std::chrono::milliseconds interval(50);

    std::this_thread::sleep_for(interval);
    /*! send command */
    this->sendCommand(command);
    //std::this_thread::sleep_for(interval);

    /*! get left motor data */
    serial.poll();
    left = this->getData();

    /*! get right motor data */
    serial.poll();
    right = this ->getData();

    /*! clear serial port buffer */
    serial.clear();
    return true;
}

/*!
 * @brief halt motor without getting motor data
 */
void Motor::halt()
{
    this->sendCommand(MotorCommand(0, 0, 0, 0));
    return;
}

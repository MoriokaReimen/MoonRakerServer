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
 * @brief send command to left V10
 * @param[in] command command for the motor
 */
int Motor::sendLeftCommand(const MotorCommand& command)
{
    CommandBytes bytes {command.toLeftByteArray()};
    unsigned char buffer[25];
    memcpy(buffer, &bytes, sizeof(bytes));
    serial.write(buffer, sizeof(bytes) + 2);
    return EXIT_SUCCESS;
}

/*!
 * @brief send command to right V10
 * @param[in] command command for the motor
 */
int Motor::sendRightCommand(const MotorCommand& command)
{
    CommandBytes bytes {command.toRightByteArray()};
    unsigned char buffer[25];
    memcpy(buffer, &bytes, sizeof(bytes));
    serial.write(buffer, sizeof(bytes) + 2);
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

    while(true)
    {
      try {
            /*! send command  to Right V10*/
            this->sendRightCommand(command);
            /*! get right motor data */
            right = this->getData();
            break;
      } catch(const runtime_error& e){}
    }

    while(true)
    {
      try {
            /*! send command  to Left V10*/
            this->sendLeftCommand(command);
            /*! get left motor data */
            left = this->getData();
            break;
      } catch(const runtime_error& e){}
    }

    /*! clear serial port buffer */
    serial.clear();
    return true;
}

/*!
 * @brief halt motor without getting motor data
 */
void Motor::halt()
{
    this->sendLeftCommand(MotorCommand(0, 0, 0, 0));
    return;
}

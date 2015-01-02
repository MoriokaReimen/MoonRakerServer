/*!
-----------------------------------------------------------------------------
@file    publish.cpp
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
 ###    @@@@@               @date Dec. 29. 2014
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
@brief published contents manipulation class
-----------------------------------------------------------------------------
*/
#include "publish.hpp"
#include <stdint>
using std::abs;

/*!
 * @struct Publish
 * @brief wrapper for published contents bytes array
*/
#pragma pack(1)
struct Publish
{
    const uint16_t header = 0x75AA; //! Header byte
    uint32_t time = 0;         //! Time stamp Byte
    int16_t left_front_rpm = 0; //! front Motor RPM
    int16_t left_rear_rpm = 0; //! rear Motor RPM
    int16_t right_front_rpm = 0; //! front Motor RPM
    int16_t right_rear_rpm = 0; //! rear Motor RPM
    int16_t left_front_current = 0; //! front Current
    int16_t left_rear_current = 0; //! rear Current
    int16_t right_front_current = 0; //! front Current
    int16_t right_rear_current = 0; //! rear Current
    uint16_t battery_v = 0;        //! Battery Level milliVolts
    const uint16_t footer = 0x75FF; //! Footer byte
} __attribute__((__packed__));
#pragma pack()

/*!
 * @class ZmqMoon
 * @brief wrapper class for zmq manupulation
*/
class ZmqMoon
{
public:
/*!
 * @brief Constructor for ZmqMoon class
 * @param[in] left left motr rotation velocity in rpm
 * @param[in] right left motr rotation velocity in rpm
 */
  ZmqMoon()
  {
  }
  ~ZmqMoon()
  {
  }
  MotorCommand getCommand()
  {
  }
  sendData(const MotorData& data)
  {
  }
};

MotorCommand::MotorCommand(const signed short& left, const signed short& right)
{
    if(abs(left_rpm) > max_rpm_ || abs(right_rpm) > max_rpm_)
        throw std::range_error("rpm is over max rpm");

    this->left_rpm  = left;
    this->right_rpm = right;
    return;
}

/*!
 * @brief generate Bytes array of command
 * @return bytes array of command
 */
CommandBytes MotorCommand::toByteArray() const
{
    CommandBytes command;
    command.left_rpm = this->left_rpm;
    command.right_rpm = this->right_rpm;
    return command;
}

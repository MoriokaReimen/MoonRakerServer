/*!
-----------------------------------------------------------------------------
@file    Command.cpp
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
@brief command manipulation class
-----------------------------------------------------------------------------
*/
#include "Command.hpp"
using std::abs;

/*!
 * @brief Constructor for MotorCommand class
 * @param[in] left left motr rotation velocity in rpm
 * @param[in] right left motr rotation velocity in rpm
 */
MotorCommand::MotorCommand(const signed short& left_front, const signed short& left_rear,
                           const signed short& right_front, const signed short& right_rear)
{
    this->set(left_front, left_rear, right_front, right_rear);
}

/*!
 * @brief Constructor for MotorCommand class
 * @param[in] bytes array of command
 */
MotorCommand::MotorCommand(const CommandBytes& command)
{
    this->set(command);
    return;
}

/*!
 * @brief set internal variables
 * @param[in] left left motr rotation velocity in rpm
 * @param[in] right left motr rotation velocity in rpm
 */
void MotorCommand::set(const signed short& left_front, const signed short& left_rear,
                       const signed short& right_front, const signed short& right_rear)
{
    if(abs(left_front) > max_rpm_ || abs(left_rear) > max_rpm_
       || abs(right_front) > max_rpm_ || abs(right_rear) > max_rpm_)
        throw std::range_error("rpm is over max rpm");

    this->left_front_rpm  = left_front;
    this->left_rear_rpm  = left_rear;
    this->right_front_rpm = right_front;
    this->right_rear_rpm = right_rear;
    return;
}

/*!
 * @brief set internal variables
 * @param[in] bytes array of command
 */
void MotorCommand::set(const CommandBytes& command)
{
    signed short left_front = static_cast<int16_t>(be16toh(command.left_front_rpm));
    signed short left_rear = static_cast<int16_t>(be16toh(command.left_rear_rpm));
    signed short right_front = static_cast<int16_t>(be16toh(command.right_front_rpm));
    signed short right_rear = static_cast<int16_t>(be16toh(command.right_rear_rpm));
    this->set(left_front, left_rear, right_front, right_rear);
    return;
}

/*!
 * @brief generate Bytes array of command
 * @return bytes array of command
 */
CommandBytes MotorCommand::toLeftByteArray() const
{
    CommandBytes command;
    command.target = 0x21;
    command.left_front_rpm = htobe16(this->left_front_rpm);
    command.left_rear_rpm = htobe16(this->left_rear_rpm);
    command.right_front_rpm = htobe16(this->right_front_rpm);
    command.right_rear_rpm = htobe16(this->right_rear_rpm);
    return command;
}

CommandBytes MotorCommand::toRightByteArray() const
{
    CommandBytes command;
    command.target = 0x22;
    command.left_front_rpm = htobe16(this->left_front_rpm);
    command.left_rear_rpm = htobe16(this->left_rear_rpm);
    command.right_front_rpm = htobe16(this->right_front_rpm);
    command.right_rear_rpm = htobe16(this->right_rear_rpm);
    return command;
}

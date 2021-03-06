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
MotorCommand::MotorCommand(const signed short& left, const signed short& right)
{
    this->set(left, right);
}

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
 * @param[in] left left motr rotation velocity in rpm
 * @param[in] right left motr rotation velocity in rpm
 */
void MotorCommand::set(const signed short& left, const signed short& right)
{
    if(abs(left) > max_rpm_ || abs(right) > max_rpm_)
        throw std::range_error("rpm is over max rpm");

    this->left_front_rpm  = left;
    this->left_rear_rpm  = left;
    this->right_front_rpm = right;
    this->right_rear_rpm = right;
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
 * @brief generate Bytes array of command for left motor
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

/*!
 * @brief generate Bytes array of command for Right motor
 * @return bytes array of command
 */
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

/*!
 * @brief serialize command for udp
 * @return serialized command in std::string
 */
std::string MotorCommand::serialize() const
{
    std::string serialized;
    serialized += "$";
    serialized += std::to_string(this->left_front_rpm) + ",";
    serialized += std::to_string(this->left_rear_rpm) + ",";
    serialized += std::to_string(this->right_front_rpm) + ",";
    serialized += std::to_string(this->right_rear_rpm) + ";";

    return serialized;
}

/*!
 * @brief deserialize command for udp
 */
void MotorCommand::deserialize(const std::string& serialized)
{
    auto first = serialized.find_last_of('$');
    auto last = serialized.find_last_of(';');
    /* remove "$" and ";"; */
    auto buff = serialized.substr(first + 1, last - first - 1);

    std::stringstream ss(buff);
    std::string element;

    std::getline(ss, element, ',');
    this->left_front_rpm = std::stoi(element);
    std::getline(ss, element, ',');
    this->left_rear_rpm = std::stoi(element);
    std::getline(ss, element, ',');
    this->right_front_rpm = std::stoi(element);
    std::getline(ss, element, ',');
    this->right_rear_rpm = std::stoi(element);

    return;
}

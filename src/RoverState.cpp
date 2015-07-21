/*!
-----------------------------------------------------------------------------
@file    RoverState.cpp
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
@brief hold rover state class
-----------------------------------------------------------------------------
*/
#include "RoverState.hpp"

RoverState::RoverState(const StateBytes& bytes)
{
  this->set(bytes);
}

/*! @function set
 *  @brief set Rover state with Motordata
*/
void RoverState::set(const MotorData& left, const MotorData& right, const Math3D::Quaternion& quat)
{
    /* set rpm */
    this->left_front.rpm = left.front_rpm / GEAR_RATIO;
    this->left_rear.rpm = left.rear_rpm / GEAR_RATIO;
    this->right_front.rpm = right.front_rpm / GEAR_RATIO;
    this->right_rear.rpm = right.rear_rpm / GEAR_RATIO;

    /* set torque */
    this->left_front.torque = left.front_current * TORQUE_CONSTANT;
    this->left_rear.torque = left.rear_current * TORQUE_CONSTANT;
    this->right_front.torque = right.front_current * TORQUE_CONSTANT;
    this->right_rear.torque = right.rear_current * TORQUE_CONSTANT;

    /* set Attitude */
    this->quat = quat;

    /* set time */
    this->time = std::chrono::duration_cast<std::chrono::milliseconds>
                 (std::chrono::system_clock::now().time_since_epoch()).count();

    return;
}

void RoverState::set(const StateBytes& bytes)
{
    /* set rpm */
    this->left_front.rpm = bytes.left_front_rpm;
    this->left_rear.rpm = bytes.left_rear_rpm;
    this->right_front.rpm = bytes.right_front_rpm;
    this->right_rear.rpm = bytes.right_rear_rpm;

    /* set torque */
    this->left_front.torque = bytes.left_front_torque;
    this->left_rear.torque = bytes.left_rear_torque;
    this->right_front.torque = bytes.right_front_torque;
    this->right_rear.torque = bytes.right_rear_torque;

    /* set Attitude */
    this->quat.fromRPY(bytes.roll, bytes.pitch, bytes.yaw);

    /* set time */
    this->time = bytes.time;

  return;
}

void RoverState::set(const MotorData& left, const MotorData& right)
{
    /* set rpm */
    this->left_front.rpm = left.front_rpm / GEAR_RATIO;
    this->left_rear.rpm = left.rear_rpm / GEAR_RATIO;
    this->right_front.rpm = right.front_rpm / GEAR_RATIO;
    this->right_rear.rpm = right.rear_rpm / GEAR_RATIO;

    /* set torque */
    this->left_front.torque = left.front_current * TORQUE_CONSTANT;
    this->left_rear.torque = left.rear_current * TORQUE_CONSTANT;
    this->right_front.torque = right.front_current * TORQUE_CONSTANT;
    this->right_rear.torque = right.rear_current * TORQUE_CONSTANT;

    /* set time */
    this->time = std::chrono::duration_cast<std::chrono::milliseconds>
                 (std::chrono::system_clock::now().time_since_epoch()).count();

    return;
}

StateBytes RoverState::toByteArray() const
{
  StateBytes bytes;
  bytes.time = this->time;

  Math3D::Degree roll, pitch, yaw;
  this->quat.toRPY(roll, pitch, yaw);
  bytes.roll = roll.val;
  bytes.pitch = pitch.val;
  bytes.yaw = yaw.val;

  bytes.left_front_rpm = this->left_front.rpm;
  bytes.left_rear_rpm = this->left_rear.rpm;
  bytes.right_front_rpm = this->right_front.rpm;
  bytes.right_rear_rpm = this->right_rear.rpm;

  bytes.left_front_torque = this->left_front.torque;
  bytes.left_rear_torque = this->left_rear.torque;
  bytes.right_front_torque = this->right_front.torque;
  bytes.right_rear_torque = this->right_rear.torque;

  return bytes;
}

/*!
 * @brief serialize command for udp
 * @return serialized command in std::string
 */
std::string RoverState::serialize() const
{
    std::string serialized;

    Math3D::Degree roll, pitch, yaw;
    this->quat.toRPY(roll, pitch, yaw);

    serialized += "$";
    serialized += std::to_string(this->time) + ",";
    serialized += std::to_string(roll.val) + ",";
    serialized += std::to_string(pitch.val) + ",";
    serialized += std::to_string(yaw.val) + ",";

    serialized += std::to_string(this->left_front.rpm) + ",";
    serialized += std::to_string(this->left_rear.rpm) + ",";
    serialized += std::to_string(this->right_front.rpm) + ",";
    serialized += std::to_string(this->right_rear.rpm) + ",";

    serialized += std::to_string(this->left_front.torque) + ",";
    serialized += std::to_string(this->left_rear.torque) + ",";
    serialized += std::to_string(this->right_front.torque) + ",";
    serialized += std::to_string(this->right_rear.torque) + ";";

    return serialized;
}

/*!
 * @brief deserialize command from udp received data
 */
void RoverState::deserialize(const std::string& serialized)
{
    Math3D::Degree roll, pitch, yaw;

    auto first = serialized.find_last_of('$');
    auto last = serialized.find_last_of(';');
    /* remove "$" and ";"; */
    auto buff = serialized.substr(first + 1, last - first - 1);

    std::stringstream ss(buff);
    std::string element;

    std::getline(ss, element, ',');
    this->time = std::stoll(element);

    std::getline(ss, element, ',');
    roll.val = std::stod(element);
    std::getline(ss, element, ',');
    pitch.val = std::stod(element);
    std::getline(ss, element, ',');
    yaw.val = std::stod(element);
    this->quat.fromRPY(roll, pitch, yaw);


    std::getline(ss, element, ',');
    this->left_front.rpm = std::stod(element);
    std::getline(ss, element, ',');
    this->left_rear.rpm = std::stod(element);
    std::getline(ss, element, ',');
    this->right_front.rpm = std::stod(element);
    std::getline(ss, element, ',');
    this->right_rear.rpm = std::stod(element);

    std::getline(ss, element, ',');
    this->left_front.torque = std::stod(element);
    std::getline(ss, element, ',');
    this->left_rear.torque = std::stod(element);
    std::getline(ss, element, ',');
    this->right_front.torque = std::stod(element);
    std::getline(ss, element, ',');
    this->right_rear.torque = std::stod(element);

    return;
}

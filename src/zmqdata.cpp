/*!
-----------------------------------------------------------------------------
@file    zmqdata.cpp
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
 ###    @@@@@               @date Jan. 2. 2015
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
@brief published/subscribe data manipulation class
-----------------------------------------------------------------------------
*/
#include "zmqdata.hpp"

/*!
 * @brief Constructor for ZMQData class
 * @param[in] raw bits array from serial port.
 */
ZMQData::ZMQData(const ZMQBytes& data)
{
  this->set(data);
  return;
}

/*!
 * @brief set internal variables
 * @param[in] raw bits array from serial port.
 */
void ZMQData::set(const ZMQBytes& data)
{
  this -> time  = static_cast<uint32_t>(data.time);
  this -> left_rear_current  = static_cast<int16_t>(data.left_rear_current);
  this -> left_front_current = static_cast<int16_t>(data.left_front_current);
  this -> right_rear_current  = static_cast<int16_t>(data.right_rear_current);
  this -> right_front_current = static_cast<int16_t>(data.right_front_current);
  this -> left_rear_rpm  = static_cast<int16_t>(data.left_rear_rpm);
  this -> left_front_rpm  = static_cast<int16_t>(data.left_front_rpm);
  this -> right_rear_rpm  = static_cast<int16_t>(data.right_rear_rpm);
  this -> right_front_rpm  = static_cast<int16_t>(data.right_front_rpm);
  this -> battery_v  = static_cast<uint16_t>(data.battery_v);

  return;
}

/*!
 * @brief Generate raw bits array from private variables
 * @return Data bits array
 */
ZMQBytes ZMQData::toByteArray() const
{
  ZMQBytes data;
  data.time = static_cast<long>(this->time);
  data.left_rear_current = static_cast<int>(this->left_rear_current);
  data.left_front_current = static_cast<int>(this->left_front_current);
  data.right_rear_current = static_cast<int>(this->right_rear_current);
  data.right_front_current = static_cast<int>(this->right_front_current);
  data.left_rear_rpm = static_cast<int>(this->left_rear_rpm);
  data.left_front_rpm = static_cast<int>(this->left_front_rpm);
  data.right_rear_rpm = static_cast<int>(this->right_rear_rpm);
  data.right_front_rpm = static_cast<int>(this->right_front_rpm);
  data.battery_v = static_cast<int>(this->battery_v);
  return data;
}

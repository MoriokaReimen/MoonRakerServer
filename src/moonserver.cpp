/*!
-----------------------------------------------------------------------------
@file    moonserver.cpp
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
 ###    @@@@@               @date Jan. 3. 2014
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
@brief moonraker server main class
-----------------------------------------------------------------------------
*/
#include <chrono>
#include "zmqserver.hpp"
#include "motor.hpp"
using std::string;
using std::chrono;

class MoonServer
{
  MotorCommand command_;
  MotorData data_;
  ZMQServer zmq_;
  Motor motor_;
public:
  MoonServer(const string& rover_address, const string& console_address);
  void doRoutine()
  {
    auto start_time = high_resolution_clock::now();
    MotorCommand command;
    MotorData r_motor_data; //! wrapper class for right motor data
    MotorData l_motor_data; //! wrapper class for left motor data
    ZMQData zmq_data;
    while(true)
    {
      try
      {
        command = this->zmq_.getCommand();
        break;
      } catch(...) {}
    }
    this->motor_.sendCommand(command);
    while(true)
    {
      try
      {
        l_motor_data = this->motor_.getData();
        if(l_motor_data.device == "L") break;
      } catch(...) {}
    }
    while(true)
    {
      try
      {
        r_motor_data = this->motor_.getData();
        if(r_motor_data.device == "R") break;
      } catch(...) {}
    }
    auto finish_time = high_resolution_clock::now();
    if(std::chrono::duration_cast<milliseconds>(finish_time - start_time).count > 50) return;
    this->zmq_.sendData(zmq_data);
    return;
  }

  void run()
  {
    while(true) this->doRoutine();
  }

  void run(double seconds)
  {
    auto start_time = high_resolution_clock::now();
    while(true)
    {
      this->doRoutine();
      auto finish_time = high_resolution_clock::now();
      if(std::chrono::duration_cast<seconds>(finish_time - start_time).count > seconds) return;
    }
  }
};
/*!
 * @brief Constructor for ZMQServer class
 * @param[in] address to publish data like ""
 * @param[in] address to subscribe command like ""
*/
ZMQServer::ZMQServer(const string& data_address, const string& command_address) :
    publish_socket_(publish_context_, publish_type_), subscribe_socket_(subscribe_context_, subscribe_type_)
{
    /* Initialize socket for receiving command */
    this -> subscribe_socket_.bind(command_address);

    /* Initialize socket for publishing motor data */
    this -> publish_socket_.bind(data_address);

    return;
}

/*!
 * @brief subscribe command through zmq socket.
 * @return Motor command
*/
MotorCommand ZMQServer::getCommand()
{
    CommandBytes  bytes;
    zmqpp::message message;
    this->subscribe_socket_.receive(message);
    memcpy(&bytes, message.raw_data(0), sizeof(bytes));
    return MotorCommand(bytes);
}

/*!
 * @brief publish Motor data through zmq socket
 * @param[in] motor data
*/
void ZMQServer::sendData(const ZMQData& data)
{
    ZMQBytes bytes = data.toByteArray();
    zmqpp::message message;
    message.add_raw(&bytes, sizeof(bytes));
    this->publish_socket_.send(message);
    return;
}

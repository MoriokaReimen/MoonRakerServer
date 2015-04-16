/*
-----------------------------------------------------------------------------
Filename:    Logger.cpp
-----------------------------------------------------------------------------
         @@
       @@@@@@
      @```@@@@
     @`  `@@@@@@
   @@`   `@@@@@@@@
  @@`    `@@@@@@@@@           Tohoku University
  @` `   `@@@@@@@@@       SPACE ROBOTICS LABORATORY
  @`` ## `@@@@@@@@@    http://www.astro.mech.tohoku.ac.jp/
  @` #..#`@@@@@@@@@        Planetary Robotics Group @` #..#`@@@@@@@@@
  @` ### `@@@@@@@@@          Professor Kazuya Yoshida
  @` ###``@@@@@@@@@      Associate Professor Keiji Nagatani
   @### ``@@@@@@@@
   ###  ` @@@@@@@
  ###  @  @@@@@                 Creation Date:
 ###    @@@@@                    Spring 2013
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%                   Kei Nakata
   #### %%%                 nakata@astro.mech.tohoku.ac.jp
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------
@brief class for logging MotorData to file
-----------------------------------------------------------------------------
*/
#include "Logger.hpp"

namespace MOTOR
{
const double GEAR_RATIO = 690.0f;
const double GEAR_EFFICIENCY = 0.49f;
const double TORQUE_CONSTANT = 0.00902f * GEAR_RATIO * GEAR_EFFICIENCY; //! in [mNm/mA]
const double PI = 3.14159265359;
};

/*!
 * @brief Constructor for Logger class
 * @param[in] file name to logging
 */
Logger::Logger(const std::string& file_name)
{
        file_.open(file_name);
        return;
}

/*!
 * @brief Destructor for Logger class
 */
Logger::~Logger()
{
        file_.close();

        return;
}

/*!
 * @brief function to log one line
 * @param[in] command sended to motor
 * @param[in] data from motor
 */
bool Logger::log(const MotorCommand& command, const MotorData& data)
{
        long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::string device = data.device;

        /*! calculate torque [mNm] */
        auto back_torque = MOTOR::TORQUE_CONSTANT * data.front_current;
        auto front_torque = MOTOR::TORQUE_CONSTANT * data.rear_current;

        /*! calculate power [mW] */
        auto back_power = (back_torque * (data.rear_rpm / MOTOR::GEAR_RATIO)) * 2 * MOTOR::PI / 60;
        auto front_power = (front_torque * (data.front_rpm / MOTOR::GEAR_RATIO)) * 2 * MOTOR::PI / 60;

        file_ << device << ":" //! device id 'L' or 'R'
              << now / 1000 << "," << (now % 1000) * 1000 << ";" //! rover pc board time in [ms]
              << now / 1000 << "," << (now % 1000) * 1000 << ";" //! rover pc board time in [ms]
              << data.time << ";" //! motor controler's time
              << command.getTargetSpeed(device) << "," << data.front_rpm << "," << data.rear_rpm << ";"
              << back_torque << ","<< front_torque << ";"
              << back_power << ","<< front_power << ";"
              << data.battery_v << std::endl;

        return true;
}

/*!
 * @brief function to log data
 * @param[in] command sended to motor
 * @param[in] data from left motor
 * @param[in] data from right motor
 */
bool Logger::log(const MotorCommand& command, const MotorData& left, const MotorData& right)
{
        this -> log(command, left);
        this -> log(command, right);
        return true;
}

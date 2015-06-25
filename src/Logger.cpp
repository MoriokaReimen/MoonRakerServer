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
constexpr double GEAR_RATIO = 690.0f;
constexpr double GEAR_EFFICIENCY = 0.49f;
constexpr double TORQUE_CONSTANT = 0.00902f * GEAR_RATIO * GEAR_EFFICIENCY; //! in [mNm/mA]
constexpr double PI = 3.14159265359;
};

/*!
 * @brief Constructor for Logger class
 * @param[in] file name to logging
 */
Logger::Logger(const std::string& file_name)
{
    file_.open(file_name);
    file_ << "Time[ms]" << ","
          << "LF RPM"  << ","
          << "LR RPM"  << ","
          << "RF RPM"  << ","
          << "RR RPM"  << ","
          << "LF torque" << ","
          << "LR torque" << ","
          << "RF torque" << ","
          << "RR torque" << ","
          << "Roll" << ","
          << "Pitch" << ","
          << "Yaw" << std::endl;
    file_.precision(5);
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
void Logger::log(const RoverState& data)
{
  Math3D::Degree roll{0}, pitch{0}, yaw{0};
  data.quat.toRPY(roll, pitch, yaw);
    file_ << data.time << ","
          << data.left_front.rpm << ","
          << data.left_rear.rpm << ","
          << data.right_front.rpm << ","
          << data.right_rear.rpm << ","
          << data.left_front.torque << ","
          << data.left_rear.torque << ","
          << data.right_front.torque << ","
          << data.right_rear.torque << ","
          << roll << ","
          << pitch << ","
          << yaw << std::endl;

    return;
}

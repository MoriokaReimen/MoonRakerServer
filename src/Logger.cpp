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
    file_ << device << ","; //! device id 'L' or 'R'
    file_ << now << ","; //! rover pc board time in [ms]
    file_ << data.time << ","; //! motor controler's time
    file_ << command.getTargetSpeed(device) << ","; //! target speed in RPM
    file_ << data.front_rpm << "," << data.rear_rpm << ","; //! current motor RPM
    file_ << data.front_current << "," << data.rear_current << ","; //! current in mA
    file_ << data.battery_v << std::endl;
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

/*!
-----------------------------------------------------------------------------
@file    TimerRunner.cpp
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
@brief Run Specified time in specified Rotation speed
-----------------------------------------------------------------------------
*/
#include <iostream>
#include <cstdio>
#include <ncurses.h>
#include "Motor.hpp"
#include "Data.hpp"
#include "Logger.hpp"
#include "RoverState.hpp"
#include "IMU.hpp"
#include <string>
using std::cout;
using std::cin;
using std::endl;

constexpr short GEAR_RATIO = 690;

int main()
{
    /*! set up log file */
    std::string file_name;
    cout << "log file name:" << endl;
    cin >> file_name;
    file_name = file_name + ".mlog";
    Logger logger(file_name);

    /*! set up MoonRaker */
    RoverState rover;
    MotorData left_data, right_data;
    short left_rpm(0), right_rpm(0);
    Motor motor;
    MotorCommand command(0, 0, 0, 0);

    cout << "Input left RPM:" << endl;
    cin >> left_rpm;
    cout << "Input left RPM:" << endl;
    cin >> right_rpm;

    /* refer gear reduction */
    left_rpm *= GEAR_RATIO;
    right_rpm *= GEAR_RATIO;

    /*! set up IMU */
    Math3D::Degree roll, pitch, yaw;
    cout << "Initialize IMU...";
    IMU imu;
    imu.calibrate();
    cout << "done" << std::endl;

    /*! set up curses*/
    int ch = 0;
    int line = 0;
    initscr();
    cbreak();
    noecho();
    curs_set(2);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    timeout(0);
    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
        ch = getch();
        if(ch == 'q') break;

        auto current_time = std::chrono::high_resolution_clock::now();
        auto ellapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        command.set(0,0);
        if(ellapsed_time > 5.0)  command.set(left_rpm, right_rpm);
        if(ellapsed_time > 65.0) command.set(0, 0);
        if(ellapsed_time > 70.0) break;

        /*! send command and get data */
        try {
            motor.work(command, left_data, right_data);
        } catch(...) {
            move(1, 0);
            attron(COLOR_PAIR(2));
            printw("Error !!");
            attroff(COLOR_PAIR(2));
        }

        rover.set(left_data, right_data, imu.getQuat());
        logger.log(rover);

        /*! show data to console */
        rover.quat.toRPY(roll, pitch, yaw);
        move(5, 0);
        printw("Roll:%5f Pitch:%5f Yaw:%5f", roll, pitch, yaw);

        move(6, 0);
        printw("%1s%15s%15s%15s%15s","Device", "Rear Current", "Front Current", "Rear RPM", "Front RPM");

        move(7, 0);
        attron(COLOR_PAIR(1));
        printw("%1s%15d%15d%15d%15d",
               left_data.device.c_str(), left_data.rear_current, left_data.front_current,
               left_data.rear_rpm, left_data.front_rpm);
        attroff(COLOR_PAIR(1));

        move(8, 0);
        attron(COLOR_PAIR(1));
        printw("%1s%15d%15d%15d%15d",
               right_data.device.c_str(), right_data.rear_current, right_data.front_current,
               right_data.rear_rpm, right_data.front_rpm);
        attroff(COLOR_PAIR(1));

        move(9 + line, 0);
        printw("%1s%15d%15d%15d%15d",
               left_data.device.c_str(), left_data.rear_current, left_data.front_current,
               left_data.rear_rpm, left_data.front_rpm);

        move(21 + line, 0);
        printw("%1s%15d%15d%15d%15d",
               right_data.device.c_str(), right_data.rear_current, right_data.front_current,
               right_data.rear_rpm, right_data.front_rpm);

        move(4, 0);
        printw("press (q) to quit");
        line++;
        if(line > 10) line = 0;
        refresh();
    }

    /*! stop motor and clean up curses */
    for(int i = 0; i < 3; ++i)
    {
      try {
        motor.halt();
        break;
      } catch(...) { }
    }
    endwin();

    return 0;
}

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
    RoverState rover;
    /*! set up log file */
    std::string file_name;
    cout << "log file name:" << endl;
    cin >> file_name;
    file_name = file_name + ".mlog";
    Logger logger(file_name);

    /*! set up MoonRaker */
    short left_front_rpm(0), left_rear_rpm(0);
    short right_front_rpm(0), right_rear_rpm(0);
    Motor motor;
    MotorData left, right;
    MotorCommand command(0, 0, 0, 0);
    cout << "Input left front RPM:" << endl;
    cin >> left_front_rpm;
    cout << "Input left rear RPM:" << endl;
    cin >> left_rear_rpm;
    cout << "Input right front RPM:" << endl;
    cin >> right_front_rpm;
    cout << "Input right rear RPM:" << endl;
    cin >> right_rear_rpm;

    /* refer gear reduction */
    left_front_rpm *= GEAR_RATIO;
    left_rear_rpm *= GEAR_RATIO;
    right_front_rpm *= GEAR_RATIO;
    right_rear_rpm *= GEAR_RATIO;

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
        command.set(0,0, 0, 0);
        if(ellapsed_time > 5.0)  command.set(left_front_rpm, left_rear_rpm, right_front_rpm, right_rear_rpm);
        if(ellapsed_time > 35.0) command.set(0, 0, 0, 0);
        if(ellapsed_time > 40.0) break;

        /*! send command and get data */
        try {
            motor.work(command, left, right);
        } catch(...) {
            move(1, 0);
            attron(COLOR_PAIR(2));
            printw("Error !!");
            attroff(COLOR_PAIR(2));
        }

        rover.set(left, right, imu.getQuat());
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
               left.device.c_str(), left.rear_current, left.front_current,
               left.rear_rpm, left.front_rpm);
        attroff(COLOR_PAIR(1));

        move(8, 0);
        attron(COLOR_PAIR(1));
        printw("%1s%15d%15d%15d%15d",
               right.device.c_str(), right.rear_current, right.front_current,
               right.rear_rpm, right.front_rpm);
        attroff(COLOR_PAIR(1));

        move(9 + line, 0);
        printw("%1s%15d%15d%15d%15d",
               left.device.c_str(), left.rear_current, left.front_current,
               left.rear_rpm, left.front_rpm);

        move(21 + line, 0);
        printw("%1s%15d%15d%15d%15d",
               right.device.c_str(), right.rear_current, right.front_current,
               right.rear_rpm, right.front_rpm);

        move(4, 0);
        printw("press (q) to quit");
        line++;
        if(line > 10) line = 0;
        refresh();
    }

    /*! stop motor and clean up curses */
    motor.halt();
    endwin();

    return 0;
}

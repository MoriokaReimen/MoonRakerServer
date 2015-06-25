/*!
-----------------------------------------------------------------------------
@file    SimpleMain.cpp
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
@brief Simple Console for controling MoonRaker
-----------------------------------------------------------------------------
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
#include "Motor.hpp"
#include "Data.hpp"
#include "RoverState.hpp"
#include "Logger.hpp"
#include "IMU.hpp"
#include <string>
using std::cout;
using std::cin;
using std::endl;

constexpr int GEAR_RATIO = 690;

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
    int norm_rpm(0);
    Motor motor;
    MotorData left, right;
    int left_rpm(0), right_rpm(0);
    MotorCommand command(0, 0, 0, 0);
    cout << "Input Nominal RPM (-5 to +5):" << endl;
    cin >> norm_rpm;

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

    while (true) {

        /* get input from consoler and cope it */
        ch = getch();
        if(ch == 'q') break;
        if(ch == 'k') left_rpm = norm_rpm, right_rpm = norm_rpm;
        if(ch == 'j') left_rpm = - norm_rpm, right_rpm = - norm_rpm;
        if(ch == 'h') left_rpm = - norm_rpm, right_rpm = norm_rpm;
        if(ch == 'l') left_rpm = norm_rpm, right_rpm = - norm_rpm;
        if(ch == 's') left_rpm = right_rpm = 0;

        /* gear ratio */
        command.set(left_rpm * GEAR_RATIO, left_rpm * GEAR_RATIO,
                    right_rpm * GEAR_RATIO, right_rpm * GEAR_RATIO);

        /*! send command and get data */
        try {
            motor.work(command, left, right);
            move(1, 0);
            clrtoeol();
        } catch(const range_error& error) {
            motor.halt();
            attron(COLOR_PAIR(2));
            printw("norm_rpm: %d", norm_rpm);
            printw("Error !!");
            attroff(COLOR_PAIR(2));
            std::exit(0);
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

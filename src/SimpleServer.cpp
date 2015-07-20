/*!
-----------------------------------------------------------------------------
@file    SimpleServer.cpp
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
@brief Simple Server control EM from remote PC using UDP
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
#include "Remote.hpp"
#include <string>
using std::cout;
using std::cin;
using std::endl;

constexpr int GEAR_RATIO = 690;

int main()
{
    /*! set up MoonRaker */
    RoverState rover;
    Motor motor;
    MotorData left, right;
    MotorCommand command(0, 0, 0, 0);

    /*! set up log file */
    Logger logger("Remote.mlog");

    /*! set up IMU */
    Math3D::Degree roll, pitch, yaw;
    IMU imu;
    imu.calibrate();

    /*! set up curses*/
    int line = 0;
    initscr();
    cbreak();
    noecho();
    curs_set(2);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    timeout(0);

    /*! set up UDP */
    Remote remote("192.168.11.2");

    while (true) {
        /* get Command from UDP */
        for(int i = 0; i < 3; ++i)
        {
          try {
            command = remote.getCommand();
            break;
          } catch(...) {}
        }

        /*! send command and get data from serial*/
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
        remote.sendData(rover);

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

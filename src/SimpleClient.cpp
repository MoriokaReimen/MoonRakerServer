/*!
-----------------------------------------------------------------------------
@file    SimpleClient.cpp
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
#include "RoverState.hpp"
#include "Logger.hpp"
#include "Remote.hpp"
#include <string>
#include <thread>
#include <chrono>
using std::cout;
using std::cin;
using std::endl;

constexpr int GEAR_RATIO = 690;

int main()
{
    /*! set up log file */
    std::string file_name;
    cout << "log file name:" << endl;
    cin >> file_name;
    file_name = file_name + ".mlog";
    Logger logger(file_name);

    /*! set up MoonRaker */
    Remote remote("192.168.11.99");
    RoverState rover;
    MotorCommand command(0, 0, 0, 0);
    int norm_rpm(0);
    int left_rpm(0), right_rpm(0);
    cout << "Input Nominal RPM (-5 to +5):" << endl;
    cin >> norm_rpm;

    /*! set up IMU */
    Math3D::Degree roll, pitch, yaw;

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
        std::this_thread::sleep_for(std::chrono::microseconds(10000)); // wait 10 ms

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

        /*! send command*/
        remote.sendCommand(command);

        /*! get Data trial 3 times */
        /* for(int i = 0; i < 3; ++i)
        {
          try {
            rover = remote.getData();
            break;
          } catch(...){}
        } */

        logger.log(rover);

        /*! show data to console */
        rover.quat.toRPY(roll, pitch, yaw);

        move(1, 0);
        printw("LF:%5d LR:%5d RF:%5d RR:%5d",
            command.left_front_rpm, command.left_rear_rpm,
            command.left_front_rpm, command.left_rear_rpm);

        move(5, 0);
        printw("Roll:%5f Pitch:%5f Yaw:%5f", roll, pitch, yaw);

        move(6, 0);
        printw("%15s%15s%15s%15s",
            "LF RPM", "LR RPM", "RF RPM", "RR RPM"
            );

        move(7, 0);
        attron(COLOR_PAIR(1));
        printw("%15d%15d%15d%15d",
               rover.left_front.rpm,  rover.left_rear.rpm,
               rover.right_front.rpm, rover.right_rear.rpm);
        attroff(COLOR_PAIR(1));

        printw("press (q) to quit");
        line++;
        if(line > 10) line = 0;
        refresh();
    }

    endwin();

    return 0;
}

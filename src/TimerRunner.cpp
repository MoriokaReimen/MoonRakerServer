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
#include "motor.hpp"
#include "data.hpp"
#include "Logger.hpp"
#include <string>
using std::cout;
using std::cin;
using std::endl;

int main()
{
    /*! set up log file */
    std::string file_name;
    cout << "log file name:" << endl;
    cin >> file_name;
    file_name = file_name + ".mlog";
    Logger logger(file_name);

    /*! set up MoonRaker */
    short left_rpm(0), right_rpm(0);
    Motor motor;
    MotorData left, right;
    MotorCommand command(0, 0);
    cout << "Input left RPM:" << endl;
    cin >> left_rpm;
    cout << "Input right RPM:" << endl;
    cin >> right_rpm;

    /*! set up curses*/
    int ch = 0;
    int line = 0;
    initscr();
    cbreak(); noecho();
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
        if(ellapsed_time > 5.0) command.set(left_rpm, right_rpm);
        if(ellapsed_time > 20.0) command.set(0, 0);
        if(ellapsed_time > 25.0) break;

        /*! send command and get data */
        try {
            motor.work(command, left, right);
            move(1, 0);
            clrtoeol();
        } catch(...) {
            move(1, 0);
            attron(COLOR_PAIR(2));
            printw("Error !!");
            attroff(COLOR_PAIR(2));
        }
        logger.log(command, left, right);

        /*! show data to console */
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

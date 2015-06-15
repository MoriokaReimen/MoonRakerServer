/*!
-----------------------------------------------------------------------------
@file    test-serial.cpp
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
#include "Logger.hpp"
#include <string>
using std::cout;
using std::cin;
using std::endl;

const short GEAR_RATIO = 690;

int main()
{
        /*! initialize sleep function */
        std::chrono::milliseconds interval(5);
        MotorData data;
        /*! set up Motor */
        short left_rpm = 0, right_rpm = 0;
        Motor motor;
        MotorCommand command;

        /*! set up curses*/
        int ch = 0;
        int old_ch = 0;
        initscr();
        cbreak();
        noecho();
        curs_set(2);
        timeout(0);

        while (true) {
            ch = 0;
                /* get input from consoler and cope it */
                ch = getch();
                if(ch == 'k')
                {
                  left_rpm = 1, right_rpm = 1;
                  /* gear ratio */
                  command.set(3000, -3000, 3000, -3000);
                  motor.sendCommand(command);
                  //std::this_thread::sleep_for(interval);
                  try { data = motor.getData(); } catch(...) {} }
                if(ch == 's')
                {
                  /* gear ratio */
                  command.set(0, 0, 0, 0);
                  motor.sendCommand(command);
        //std::this_thread::sleep_for(interval);
                  try {
                  data = motor.getData();
                  } catch(...) {}
                }
                if(ch == 'q') break;

                /* show rpm */
                mvprintw(1, 0, "L:%5d", left_rpm * GEAR_RATIO);
                mvprintw(2, 0, "R:%5d", right_rpm * GEAR_RATIO);

                mvprintw(4, 0, "Front RPM:%5d", data.front_rpm);
                mvprintw(5, 0, "Front Current:%5d", data.front_current);
                mvprintw(6, 0, "Rear RPM:%5d", data.rear_rpm);
                mvprintw(7, 0, "Rear Current:%5d", data.rear_current);
                refresh();
        }

        /*! stop motor and clean up curses */
        motor.halt();
        endwin();

        return 0;
}

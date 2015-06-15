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
        constexpr std::chrono::milliseconds interval(5);
        MotorData left, right;
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
                if(ch == '1')
                {
                  command.set(3000, 0, 0, 0);
                }
                if(ch == '2')
                {
                  command.set(0, 3000, 0, 0);
                }
                if(ch == '3')
                {
                  command.set(0, 0, 3000, 0);
                }
                if(ch == '4')
                {
                  command.set(0, 0, 0, 3000);
                }
                if(ch == 'k')
                {
                  command.set(3000, 3000, 3000, 3000);
                }
                if(ch == 's')
                {
                  /* gear ratio */
                  command.set(0, 0, 0, 0);
                }
                if(ch == 'q') break;
                motor.sendCommand(command);
                try {
                    left = motor.getData();
                    std::this_thread::sleep_for(interval);
                    right = motor.getData();
                } catch(...){}

                /* show rpm */
                move(7, 0);
                printw("%1s%15d%15d%15d%15d",
                       left.device.c_str(), left.rear_current, left.front_current,
                       left.rear_rpm, left.front_rpm);
                move(8, 0);
                printw("%1s%15d%15d%15d%15d",
                       right.device.c_str(), right.rear_current, right.front_current,
                       right.rear_rpm, right.front_rpm);
                refresh();
        }

        /*! stop motor and clean up curses */
        motor.halt();
        endwin();

        return 0;
}

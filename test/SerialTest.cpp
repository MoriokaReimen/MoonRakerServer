#include <iostream>
#include <cstdio>
#include <ncurses.h>
#include "motor.hpp"
#include "data.hpp"
using std::cout;
using std::endl;

int main()
{
	int ch = 0;
  int line = 0;

	initscr();
	cbreak();
	noecho();
	timeout(1000);

  Motor motor;
  MotorData data;
	while ((ch = getch()) != 'q') {
    try {
    data = motor.getData();
    } catch(...){}
    move(8, 0);
		printw("Rear Current\tFront Current\tRear RPM\tFront RPM");
    move(9 + line, 0);
		printw("%5f\t%5f\t%5f\t%5f",
        data.rear_current, data.front_current,
        data.rear_rpm, data.front_rpm);

		move(6, 0);
		printw("press (q) to quit");
    line++;
    if(line > 20) line = 0;
		refresh();
  }

	endwin();
  return 0;
}

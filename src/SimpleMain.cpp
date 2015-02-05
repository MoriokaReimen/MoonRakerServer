#include <iostream>
#include <cstdio>
#include <ncurses.h>
#include "motor.hpp"
#include "data.hpp"
using std::cout;
using std::cin;
using std::endl;

int main()
{
	int ch = 0;
  int line = 0;
  int left_rpm, right_rpm;
  cout << "Enter Left RPM" << endl;
  cin >> left_rpm;
  cout << "Enter Right RPM" << endl;
  cin >> right_rpm;

	initscr();
	cbreak();
	noecho();
  curs_set(2);
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
	timeout(50);

  Motor motor;
  MotorData left, right;
	while ((ch = getch()) != 'q') {
    try {
      motor.work(MotorCommand(left_rpm, right_rpm), left, right);
      move(1, 0);
      clrtoeol();
    } catch(...)
    {
      move(1, 0);
      attron(COLOR_PAIR(2));
      printw("Error !!");
      attroff(COLOR_PAIR(2));
    }
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
  motor.halt();

	endwin();
  return 0;
}

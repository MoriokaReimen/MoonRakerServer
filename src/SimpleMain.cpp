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
	timeout(1000);

  Motor motor;
  MotorData data;
	while ((ch = getch()) != 'q') {
    motor.sendCommand(MotorCommand(left_rpm, right_rpm));
    data = motor.getData();
    try {
      move(1, 0);
      clrtoeol();
    } catch(...)
    {
      move(1, 0);
      attron(COLOR_PAIR(2));
      printw("Error !!");
      attroff(COLOR_PAIR(2));
    }
    move(8, 0);
		printw("%1s%15s%15s%15s%15s","Device", "Rear Current", "Front Current", "Rear RPM", "Front RPM");

    move(9, 0);
    attron(COLOR_PAIR(1));
		printw("%1s%15d%15d%15d%15d",
        data.device.c_str(), data.rear_current, data.front_current,
        data.rear_rpm, data.front_rpm);
    attroff(COLOR_PAIR(1));

    move(10 + line, 0);
		printw("%1s%15d%15d%15d%15d",
        data.device.c_str(), data.rear_current, data.front_current,
        data.rear_rpm, data.front_rpm);

		move(6, 0);
		printw("press (q) to quit");
    line++;
    if(line > 15) line = 0;
		refresh();
  }
  motor.sendCommand(MotorCommand(0, 0));
  motor.sendCommand(MotorCommand(0, 0));

	endwin();
  return 0;
}

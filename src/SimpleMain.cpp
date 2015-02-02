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
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
	timeout(1000);

  Motor motor;
  MotorData data;
	while ((ch = getch()) != 'q') {
    try {
    data = motor.getData();
    } catch(...)
    {
      move(1, 0);
      printw("Error !!");
    }
    motor.sendCommand(MotorCommand(left_rpm, right_rpm));
    move(8, 0);
		printw("Device\tRear Current\tFront Current\tRear RPM\tFront RPM");

    move(9, 0);
    attron(COLOR_PAIR(1));
		printw("CURRENT:%s\t%d\t%d\t%d\t%d",
        data.device.c_str(), data.rear_current, data.front_current,
        data.rear_rpm, data.front_rpm);
    attroff(COLOR_PAIR(1));

    move(10 + line, 0);
		printw("%s\t%d\t%d\t%d\t%d",
        data.device.c_str(), data.rear_current, data.front_current,
        data.rear_rpm, data.front_rpm);

		move(6, 0);
		printw("press (q) to quit");
    line++;
    if(line > 30) line = 0;
		refresh();
  }
  motor.sendCommand(MotorCommand(0, 0));
  motor.sendCommand(MotorCommand(0, 0));

	endwin();
  return 0;
}

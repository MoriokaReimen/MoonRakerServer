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
    move(9 + line, 0);
		printw("%s\t%5d\t%5d\t%5d\t%5d",
        data.device.c_str(), data.rear_current, data.front_current,
        data.rear_rpm, data.front_rpm);

		move(6, 0);
		printw("press (q) to quit");
    line++;
    if(line > 30) break;
		refresh();
  }
  motor.sendCommand(MotorCommand(0, 0));
  motor.sendCommand(MotorCommand(0, 0));

	endwin();
  return 0;
}

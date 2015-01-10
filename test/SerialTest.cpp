#include <iostream>
#include "motor.hpp"
#include "data.hpp"
using std::cout;
using std::endl;

int main()
{
  Motor motor;
  MotorData data;
  while(true)
  {
    try {
    data = motor.getData();
    } catch(...){}
    cout << data.device << ",";
    cout << data.rear_current << ",";
    cout << data.front_current << ",";
    cout << data.rear_rpm << ",";
    cout << data.front_rpm << ",";
    cout << data.battery_v << ",";
    cout << data.time << endl;
  }

  return 0;
}

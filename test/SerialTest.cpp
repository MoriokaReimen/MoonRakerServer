#include <iostream>
#include <thread>
#include <chrono>
#include "motor.hpp"
#include "data.hpp"
using std::cout;
using std::endl;

int main()
{
  const auto wait_time = std::chrono::milliseconds(1000);
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
    std::this_thread::sleep_for(wait_time);
  }

  return 0;
}

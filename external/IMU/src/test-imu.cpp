#include "IMU.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

int main()
{
  const auto one_sec = std::chrono::seconds(1);
  IMU imu;
  std::cout << "Caribrate...";
  imu.calibrate();
  std::cout << "done" << std::endl;
  std::this_thread::sleep_for(one_sec);
  imu.run();
  while(true)
  {
    std::cout
      << "Roll:" << imu.getRoll() << " "
      << "Pitch:" << imu.getPitch() << " "
      << "Yaw:" << imu.getYaw() << std::endl;
    std::this_thread::sleep_for(one_sec);
  }
  return EXIT_SUCCESS;
}

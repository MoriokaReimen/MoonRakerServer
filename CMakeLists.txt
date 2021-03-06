cmake_minimum_required(VERSION 2.8)
project(MoonServer)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
INCLUDE_DIRECTORIES("/usr/include" "/usr/local/include" "${CMAKE_BINARY_DIR}/include" "${CMAKE_BINARY_DIR}/include/Math3D" "${CMAKE_BINARY_DIR}/include/IMU")
set(CMAKE_LIBRARY_PATH "/usr/lib/" "/usr/local/lib" "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_C_FLAGS "-Wall")

set(MOON_SRC src/Command.cpp src/Data.cpp src/Motor.cpp src/BoostComPort.cpp src/RoverState.cpp src/Logger.cpp src/RemoteServer.cpp src/RemoteClient.cpp)
set(MATH3D_SRC src/Math3D/Degree.cpp src/Math3D/Quaternion.cpp src/Math3D/RotateFunc.cpp src/Math3D/Vector3.cpp)
set(IMU_SRC src/IMU/IMU.cpp)
set(SOCKET_SRC src/Socket/UDP.cpp)
set(GAMEPAD_SRC src/gamepad.c src/GamePad.cpp)
set(SRC ${MATH3D_SRC} ${IMU_SRC} ${MOON_SRC} ${SOCKET_SRC} ${GAMEPAD_SRC})

add_library(moonraker STATIC ${SRC})

add_executable(TheSimple.exe src/SimpleMain.cpp)
target_link_libraries(TheSimple.exe moonraker pthread curses c m boost_system rt phidget21 udev)

add_executable(TimerRunner.exe src/TimerRunner.cpp)
target_link_libraries(TimerRunner.exe moonraker pthread curses c m boost_system rt phidget21 udev)

add_executable(SimpleServer.exe src/SimpleServer.cpp)
target_link_libraries(SimpleServer.exe moonraker pthread curses c m boost_system rt phidget21 udev)

add_executable(SimpleClient.exe src/SimpleClient.cpp)
target_link_libraries(SimpleClient.exe moonraker pthread curses c m boost_system rt phidget21 udev)

add_executable(SimpleClient2.exe src/SimpleClient2.cpp)
target_link_libraries(SimpleClient2.exe moonraker pthread curses c m boost_system rt phidget21 udev)

enable_testing()
add_executable(test.exe test/test-data.cpp test/test-command.cpp)
target_link_libraries(test.exe moonraker gtest gtest_main pthread rt boost_system phidget21 udev)
add_test(NAME "UnitTest" COMMAND "./bin/test.exe")

add_executable(TestSerial.exe test/test-serial.cpp)
target_link_libraries(TestSerial.exe moonraker pthread curses c m boost_system rt phidget21 udev)

add_executable(test-udp.exe test/test-udp.cpp)
target_link_libraries(test-udp.exe moonraker pthread curses c m boost_system rt phidget21 udev)

add_executable(test-udp2.exe test/test-udp2.cpp)
target_link_libraries(test-udp2.exe moonraker pthread curses c m boost_system rt phidget21 udev)

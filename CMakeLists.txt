cmake_minimum_required(VERSION 2.8)
project(MoonServer)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
INCLUDE_DIRECTORIES("/usr/local/include" "${CMAKE_BINARY_DIR}/include")
set(CMAKE_LIBRARY_PATH "/usr/local/lib" "${CMAKE_BINARY_DIR}/lib")
LINK_DIRECTORIES("/opt/cross/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set(CMAKE_CXX_COMPILER "i686-linux-gnu-g++")
#set(CMAKE_C_COMPILER "i686-linux-gnu-gcc")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -m32")
set(CMAKE_C_FLAGS "-Wall -m32")
set(SRC src/Command.cpp src/Data.cpp src/Motor.cpp src/BoostComPort.cpp src/RoverState.cpp src/Logger.cpp)
set(MATH3D_SRC src/Math3D/Degree.cpp src/Math3D/Quaternion.cpp src/Math3D/RotateFunc.cpp src/Math3D/Vector3.cpp)
set(IMU_SRC src/IMU/IMU.cpp)

add_executable(TheSimple.exe src/SimpleMain.cpp  ${SRC})
target_link_libraries(TheSimple.exe pthread curses c m boost_system rt)

add_executable(TimerRunner.exe src/TimerRunner.cpp ${SRC})
target_link_libraries(TimerRunner.exe pthread curses c m boost_system rt)


enable_testing()
add_executable(test.exe test/test-data.cpp test/test-command.cpp ${SRC})
target_link_libraries(test.exe gtest gtest_main pthread rt boost_system)
add_test(NAME "UnitTest" COMMAND "./bin/test.exe")

add_executable(TestSerial.exe test/test-serial.cpp ${SRC})
target_link_libraries(TestSerial.exe pthread curses c m boost_system rt)


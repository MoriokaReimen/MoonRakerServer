MoonRaker-EM Rover side Application
===================================

C++ app for MoonRaker-EM.

## REQUIREMENT
* gcc 4.9
 32bit compiler which supports C++11

* boost library1.48
 boost system for serial/udp communication

* ncurses

* cmake

* gtest
  [you can download gtest here](https://code.google.com/p/googletest)

* libphidget
  [you can download libphidget here](www.phidgets.com/docs/OS_-_Linux)

## BUILD
at the same directory of README.md

   cmake .
   make

and you will get two executables
   bin/SimpleServer.exe
   bin/SimpleClient.exe
upload SimpleServer.exe on rover and execute it.
execute SimpleClient on your console

## Author

Kei Nakata
nakata@astro.mech.tohoku.ac.jp

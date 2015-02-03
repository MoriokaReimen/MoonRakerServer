/**
-----------------------------------------------------------------------------
@file    serial.hpp
----------------------------------------------------------------------------
         @@
       @@@@@@
      @```@@@@
     @`  `@@@@@@
   @@`   `@@@@@@@@
  @@`    `@@@@@@@@@           Tohoku University
  @` `   `@@@@@@@@@       SPACE ROBOTICS LABORATORY
  @`` ## `@@@@@@@@@    http://www.astro.mech.tohoku.ac.jp/
  @` #..#`@@@@@@@@@        Planetary Robotics Group
  @` #..#`@@@@@@@@@
  @` ### `@@@@@@@@@          Professor Kazuya Yoshida
  @` ###``@@@@@@@@@      Associate Professor Keiji Nagatani
   @### ``@@@@@@@@
   ###  ` @@@@@@@
  ###  @  @@@@@                 Creation Date:
 ###    @@@@@               @date May. 18. 2014
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%             @author Kei Nakata
   #### %%%                  menschenjager.mark.neun@gmail.com
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------
@brief class for serial communication
-----------------------------------------------------------------------------
*/
#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
* @class Serial
* @brief Class for Serial Communication
*/
class Serial
{
    boost::asio::io_service io;
    boost::asio::serial_port serialport;

public:
    Serial(unsigned int speed, std::string comnum);
    ~Serial();
    bool sendData(const char* bytes, size_t n);
    std::vector<char> getData(std::string footter);
    int setBaudrate(const int& bps);
    int flush();
};

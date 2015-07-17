/*!
-----------------------------------------------------------------------------
@file    TCP_Client.hpp
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
 ###    @@@@@               @date July 29. 2015
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
@brief TCP manipulation class Client side
-----------------------------------------------------------------------------
*/
#pragma once
#include <cstring>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TCP_Client
{
    boost::asio::io_service io_service_;
    tcp::socket socket_;
    static constexpr int MAX_LENGTH{100};
public:
    TCP_Client(const std::string& address, const int& port);
    ~TCP_Client();
    bool isOpen() const;
    void close();
    void write(const std::string& message);
    void write(const void* data, size_t size);
    std::string read();
    void read(void* data, size_t size);
};

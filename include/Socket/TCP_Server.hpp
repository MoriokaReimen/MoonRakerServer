/*!
-----------------------------------------------------------------------------
@file    TCP_Server.hpp
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
@brief TCP manipulation class Server side
-----------------------------------------------------------------------------
*/
#pragma once
#include <cstdlib>
#include <boost/asio.hpp>
#include <exception>

using boost::asio::ip::tcp;

class TCP_Server
{
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  static constexpr int MAX_LENGTH{100};
public:
  TCP_Server(const int& port);
  ~TCP_Server();
  bool isOpen() const;
  void close();
  void write(const std::string& message);
  void write(const void* data, const size_t& size);
  std::string read();
  void read(void* data, const size_t& size);
};

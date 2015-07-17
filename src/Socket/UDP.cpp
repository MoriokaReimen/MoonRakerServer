/*!
-----------------------------------------------------------------------------
@file    UDP.hpp
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
@brief UDP manipulation class Client side
-----------------------------------------------------------------------------
*/
#include "Socket/UDP.hpp"

/*!
 * @brief Constructor
 * @param[in] address IP address of Server
 * @param[in] port of Server
 */
UDP::UDP(const int& port)
  : socket_(io_service_, udp::endpoint(udp::v4(), port))
{
  return;
}

/*!
 * @brief set target endpoint
 * @param[in] address IP address of target
 * @param[in] port of target
 */
void UDP::setTarget(const std::string& address, const int& port)
{
  target_.address(boost::asio::ip::address::from_string(address));
  target_.port(port);
  return;
}

/*!
 * @brief Close socket
 */
UDP::~UDP()
{
  socket_.close();
  return;
}

/*!
 * @brief Close socket
 */
void UDP::close()
{
  socket_.close();
  return;
}

/*!
 * @brief check socket is open
 * @return true if socket is open
 */
bool UDP::isOpen() const
{
  return socket_.is_open();
}

/*!
 * @brief write message to socket
 * @param[in] message string to be written
 */
void UDP::write(const std::string& message)
{
  this->write(message.c_str(), message.size());
  return;
}

/*!
 * @brief write message to socket
 * @param[in] pointer to written data
 * @param[in] bytes of data
 */
void UDP::write(const void* data, size_t size)
{
  size_t wrote_bytes = socket_.send_to(boost::asio::buffer(data, size), target_);
  if(wrote_bytes != size)
    throw std::runtime_error("UDP Client write Error");
  return;
}

/*!
 * @brief read message from socket
 * @return read string
 */
std::string UDP::read()
{
  char raw_message[MAX_LENGTH];
  this->read(raw_message, MAX_LENGTH);
  return std::string(raw_message);
}

/*!
 * @brief read message from socket
 * @param[out] pointer data
 * @param[in] bytes of data
 */
void UDP::read(void* data, size_t size)
{
  socket_.receive_from(boost::asio::buffer(data, size), target_);
  return;
}

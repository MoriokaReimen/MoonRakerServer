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
#include "Socket/TCP_Client.hpp"

/*!
 * @brief Constructor
 * @param[in] address IP address of Server
 * @param[in] port of Server
 */
TCP_Client::TCP_Client(const std::string& address, const int& port)
  : socket_(io_service_)
{
  boost::asio::ip::tcp::endpoint endpoint;
  endpoint.address(boost::asio::ip::address::from_string(address));
  endpoint.port(port);
  socket_.connect(endpoint);
  return;
}

/*!
 * @brief Close socket
 */
TCP_Client::~TCP_Client()
{
  socket_.close();
  return;
}

/*!
 * @brief Close socket
 */
void TCP_Client::close()
{
  socket_.close();
  return;
}

/*!
 * @brief check socket is open
 * @return true if socket is open
 */
bool TCP_Client::isOpen() const
{
  return socket_.is_open();
}

/*!
 * @brief write message to socket
 * @param[in] message string to be written
 */
void TCP_Client::write(const std::string& message)
{
  this->write(message.c_str(), message.size());
  return;
}

/*!
 * @brief write message to socket
 * @param[in] pointer to written data
 * @param[in] bytes of data
 */
void TCP_Client::write(const void* data, size_t size)
{
  size_t wrote_bytes = socket_.write_some(boost::asio::buffer(data, size));
  if(wrote_bytes != size)
    throw std::runtime_error("TCP Client write Error");
  return;
}

/*!
 * @brief read message from socket
 * @return read string
 */
std::string TCP_Client::read()
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
void TCP_Client::read(void* data, size_t size)
{
  boost::asio::read(socket_, boost::asio::buffer(data, size));
  return;
}

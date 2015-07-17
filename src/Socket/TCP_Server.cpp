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
#include "Socket/TCP_Server.hpp"
using boost::asio::ip::tcp;

/*!
 * @brief Constructor
 * @param[in] port of Server
 */
TCP_Server::TCP_Server(const int& port) :
  acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
  socket_(io_service_)
{
  acceptor_.accept(socket_);
  return;
}

/*!
 * @brief Destructor close socket
 */
TCP_Server::~TCP_Server()
{
  socket_.close();
  return;
}

/*!
 * @brief close socket
 */
void TCP_Server::close()
{
  socket_.close();
  return;
}

/*!
 * @brief check socket is open
 * @return true if socket is open
 */
bool TCP_Server::isOpen() const
{
  return socket_.is_open();
}

/*!
 * @brief write message to socket
 * @param[in] message string to be written
 */
void TCP_Server::write(const std::string& message)
{
  this->write(message.c_str(), message.size());
  return;
}

/*!
 * @brief write message to socket
 * @param[in] pointer to written data
 * @param[in] bytes of data
 */
void TCP_Server::write(const void* data, const size_t& size)
{
  size_t wrote_bytes = socket_.write_some(boost::asio::buffer(data, size));
  if(wrote_bytes != size)
    throw std::runtime_error("TCP Server write Error");
  return;
}

/*!
 * @brief read message from socket
 * @return read string
 */
std::string TCP_Server::read()
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
void TCP_Server::read(void* data, const size_t& size)
{
  boost::asio::read(socket_, boost::asio::buffer(data, size));
  return;
}

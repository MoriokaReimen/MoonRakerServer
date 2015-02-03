/**
-----------------------------------------------------------------------------
@file    Serial.cpp
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
@brief Serial Communication Class
-----------------------------------------------------------------------------
*/
#include "serial.hpp"
#include <algorithm>

/**
* @brief Serial class constructer
* @param speed baudrate of serial port
* @param comnum COM number for serial port in string
* @param timeout set timeout interval in msec
*/
Serial::Serial(unsigned int speed, std::string comnum)
    : io(),
      serialport(io)
{
    try{
        serialport.open(comnum);
    }
    catch (boost::system::system_error& e) {
        std::cout << "Failed to Open Serial Port" << std::endl;
        std::cout << e.what() << std::endl;
    }
    /* set baud rate */
    boost::asio::serial_port_base::baud_rate
        baudrate(speed);
    serialport.set_option(baudrate);

    /* set character size */
    boost::asio::serial_port_base::character_size charsize(8);
    serialport.set_option(charsize);

    /* disable flow control */
    boost::asio::serial_port_base::flow_control
        flow_control(boost::asio::serial_port_base::flow_control::none);
    serialport.set_option(flow_control);

    /* disable parity */
    boost::asio::serial_port_base::parity
        parity(boost::asio::serial_port_base::parity::none);
    serialport.set_option(parity);

    /* set one stop bits */
    boost::asio::serial_port_base::stop_bits
        stop_bits(boost::asio::serial_port_base::stop_bits::one);
    serialport.set_option(stop_bits);
}

/**
* @brief Serial class destructer
*/
Serial::~Serial()
{
    serialport.close();
}



/**
* @brief Serial class method to send data
* @param message sending message in string
* @return true when success, else false
*/
bool Serial::sendData(const char* bytes, size_t n)
{
        return serialport.write_some(boost::asio::buffer(bytes, n));
}

/**
* @brief Serial class method to get data
* @param command sending command in string
* @return data in string or constant string EMPTY
*/
/*int Serial::getData(char* data, int size)
{
  serialport.read_some(boost::asio::buffer(data, size));

  return EXIT_SUCCESS;
}
*/

/**
* @brief Serial class method to get data
* @param command sending command in string
* @return data in string or constant string EMPTY
*/
int Serial::getData(char* data, int size)
{
  char ch;

  while(true)
  {
    serialport.read_some(boost::asio::buffer(&ch, 1));
    if(ch == 0x77) break;
  }
  int c = 0;
  while(true)
  {
      data[c] = ch;
      serialport.read_some(boost::asio::buffer(&ch, 1));
      ++c;
      if(c > size) return EXIT_SUCCESS;
  }
}

/**
* @brief Serial class method change baudrate
* @param int baudrate
* @return int: EXIT_SUCCESS when success
*/
int Serial::setBaudrate(const int& bps)
{
    /* set baud rate */
    boost::asio::serial_port_base::baud_rate
        baudrate(bps);
    serialport.set_option(baudrate);
    return EXIT_SUCCESS;
}

/**
* @brief flush the buffer of serial port
* @return int: EXIT_SUCCESS when success
*/
int Serial::flush()
{
    serialport.get_io_service().reset();
    return EXIT_SUCCESS;
}

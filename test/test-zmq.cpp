/*!
-----------------------------------------------------------------------------
@file    test-zmq.cpp
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
 ###    @@@@@               @date Dec. 29. 2014
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
@brief test zmq class
-----------------------------------------------------------------------------
*/
#include <gtest/gtest.h>

#include "zmqserver.hpp"
#include "zmqclient.hpp"
#include <portable_endian.h>

TEST(ZMQServerTest,General){
  ZMQServer server("ipc://testA","ipc://testA");
  //ZMQClient client("tcp://localhost:4041","tcp://localhost:4040");
  //MotorCommand command_send(100, 2800);
  //ZMQData data_send;
  //ZMQData data_recv;
  //client.sendCommand(command_send);
  //MotorCommand command_recv = server.getCommand();
  //
  //EXPECT_EQ(100, command_recv.left_rpm);
  //EXPECT_EQ(2800, command_recv.right_rpm);
}

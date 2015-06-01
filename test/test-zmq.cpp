/*!
-----------------------------------------------------------------------------
@file    test-zmqserver.cpp
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
@brief test zmqserver class
-----------------------------------------------------------------------------
*/
#include <gtest/gtest.h>

#include "zmqserver.hpp"
#include "zmqclient.hpp"

TEST(ZMQServerTest, Construct)
{
    ZMQServer server("tcp://*:4242","tcp://*:4243");
}

TEST(ZMQServerTest, ReceiveCommand)
{
    ZMQServer server("tcp://*:4242","tcp://*:4243");
    ZMQClient client("tcp://localhost:4242","tcp://localhost:4243");
    MotorCommand command_send(100, 2800);
    client.sendCommand(command_send);
    MotorCommand command_recv = server.getCommand();
    EXPECT_EQ(100, command_recv.left_front_rpm);
    EXPECT_EQ(2800, command_recv.right_front_rpm);
}

TEST(ZMQServerTest, SendData)
{
    ZMQServer server("tcp://*:4242","tcp://*:4243");
    ZMQClient client("tcp://localhost:4242","tcp://localhost:4243");
    ZMQData data_send;
    data_send.time = 1000;
    data_send.left_rear_rpm = 1000;
    data_send.left_front_rpm = 2000;
    data_send.right_rear_rpm = -1000;
    data_send.right_front_rpm = -2000;
    data_send.left_rear_current = 1000;
    data_send.left_front_current = 2000;
    data_send.right_rear_current = -1000;
    data_send.right_front_current = -2000;
    data_send.battery_v = 1000;

    ZMQData data_recv;
    server.sendData(data_send);
    data_recv = client.getData();
    EXPECT_EQ(1000, data_recv.time);
    EXPECT_EQ(1000, data_recv.left_rear_rpm);
    EXPECT_EQ(2000, data_recv.left_front_rpm);
    EXPECT_EQ(-1000, data_recv.right_rear_rpm);
    EXPECT_EQ(-2000, data_recv.right_front_rpm);
    EXPECT_EQ(1000, data_recv.left_rear_current);
    EXPECT_EQ(2000, data_recv.left_front_current);
    EXPECT_EQ(-1000, data_recv.right_rear_current);
    EXPECT_EQ(-2000, data_recv.right_front_current);
    EXPECT_EQ(1000, data_recv.battery_v);
}

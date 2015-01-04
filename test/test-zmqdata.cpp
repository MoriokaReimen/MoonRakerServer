/*!
-----------------------------------------------------------------------------
@file    test-zmqdata.cpp
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
 ###    @@@@@               @date Jan. 3. 2014
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
@brief test zmqdata class
-----------------------------------------------------------------------------
*/
#include <gtest/gtest.h>
#include "zmqdata.hpp"
#include <portable_endian.h>

TEST(ZMQDataTest, Bytes)
{
    ZMQBytes data_bytesA;
    char* binary = reinterpret_cast<char *>(&data_bytesA);
    EXPECT_EQ(30, sizeof(data_bytesA));
    EXPECT_EQ('\x75', binary[0]);
    EXPECT_EQ('\xAA', binary[1]);
    EXPECT_EQ('\x75', binary[28]);
    EXPECT_EQ('\xFF', binary[29]);
}

TEST(ZMQDataTest,Construct)
{
    ZMQData data;
    data.left_front_rpm = 100;
    data.left_rear_rpm = 200;
    data.right_front_rpm = 300;
    data.right_rear_rpm = 400;
    data.left_front_current = 500;
    data.left_rear_current = 600;
    data.right_front_current = 700;
    data.right_rear_current = 800;
    data.battery_v = 900;
    data.time = 1000;
    EXPECT_EQ(100, data.left_front_rpm);
    EXPECT_EQ(200, data.left_rear_rpm);
    EXPECT_EQ(300, data.right_front_rpm);
    EXPECT_EQ(400, data.right_rear_rpm);
    EXPECT_EQ(500, data.left_front_current);
    EXPECT_EQ(600, data.left_rear_current);
    EXPECT_EQ(700, data.right_front_current);
    EXPECT_EQ(800, data.right_rear_current);
    EXPECT_EQ(900, data.battery_v);
    EXPECT_EQ(1000, data.time);
}

TEST(ZMQDataTest, toByteArray)
{
    ZMQData data;
    data.left_front_rpm = 100;
    data.left_rear_rpm = 200;
    data.right_front_rpm = 300;
    data.right_rear_rpm = 400;
    data.left_front_current = 500;
    data.left_rear_current = 600;
    data.right_front_current = 700;
    data.right_rear_current = 800;
    data.battery_v = 900;
    data.time = 1000;
    ZMQBytes data_bytes = data.toByteArray();
    char* binary = reinterpret_cast<char *>(&data_bytes);
    binary = reinterpret_cast<char *>(&data_bytes);
    EXPECT_EQ(100, (int) static_cast<int16_t>(be16toh(data_bytes.left_front_rpm)));
    EXPECT_EQ(200, (int) static_cast<int16_t>(be16toh(data_bytes.left_rear_rpm)));
    EXPECT_EQ(300, (int) static_cast<int16_t>(be16toh(data_bytes.right_front_rpm)));
    EXPECT_EQ(400, (int) static_cast<int16_t>(be16toh(data_bytes.right_rear_rpm)));
    EXPECT_EQ(500, (int) static_cast<int16_t>(be16toh(data_bytes.left_front_current)));
    EXPECT_EQ(600, (int) static_cast<int16_t>(be16toh(data_bytes.left_rear_current)));
    EXPECT_EQ(700, (int) static_cast<int16_t>(be16toh(data_bytes.right_front_current)));
    EXPECT_EQ(800, (int) static_cast<int16_t>(be16toh(data_bytes.right_rear_current)));
    EXPECT_EQ(900, (int) static_cast<int16_t>(be16toh(data_bytes.battery_v)));
    EXPECT_EQ(1000, (int) static_cast<int64_t>(be64toh(data_bytes.time)));
    EXPECT_EQ('\x75', binary[0]);
    EXPECT_EQ('\xAA', binary[1]);
    EXPECT_EQ('\x75', binary[28]);
    EXPECT_EQ('\xFF', binary[29]);
}

TEST(ZMQDataTest, fromByteArray)
{
    ZMQBytes bytes;
    bytes.left_front_rpm = htobe16(100);
    bytes.left_rear_rpm = htobe16(200);
    bytes.right_front_rpm = htobe16(300);
    bytes.right_rear_rpm = htobe16(400);
    bytes.left_front_current = htobe16(500);
    bytes.left_rear_current = htobe16(600);
    bytes.right_front_current = htobe16(700);
    bytes.right_rear_current = htobe16(800);
    bytes.battery_v = htobe16(900);
    bytes.time = htobe64(1000);
    ZMQData data;
    data.set(bytes);
    EXPECT_EQ(100, data.left_front_rpm);
    EXPECT_EQ(200, data.left_rear_rpm);
    EXPECT_EQ(300, data.right_front_rpm);
    EXPECT_EQ(400, data.right_rear_rpm);
    EXPECT_EQ(500, data.left_front_current);
    EXPECT_EQ(600, data.left_rear_current);
    EXPECT_EQ(700, data.right_front_current);
    EXPECT_EQ(800, data.right_rear_current);
    EXPECT_EQ(900, data.battery_v);
    EXPECT_EQ(1000, data.time);
}

#include <gtest/gtest.h>

#include "command.hpp"
/*!
-----------------------------------------------------------------------------
@file    test-data.cpp
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
@brief test data class
-----------------------------------------------------------------------------
*/
#include "data.hpp"
#include <portable_endian.h>

TEST(DataTest,General){
  DataBytes data_bytesA;
  char* binary = reinterpret_cast<char *>(&data_bytesA);
  EXPECT_EQ(19, sizeof(data_bytesA));
  EXPECT_EQ(0, (int) static_cast<int16_t>(data_bytesA.rear_rpm));
  EXPECT_EQ(0, (int) static_cast<int16_t>(data_bytesA.front_rpm));
  EXPECT_EQ('\x75', binary[0]);
  EXPECT_EQ('\xAA', binary[1]);
  EXPECT_EQ('\x00', binary[2]);
  EXPECT_EQ('\x75', binary[17]);
  EXPECT_EQ('\xFF', binary[18]);

  MotorData data;
  data.rear_rpm = 100;
  data.front_rpm = 200;
  data.rear_current = 300;
  data.front_current = 400;
  data.battery_v = 500;
  data.time = 1000;
  DataBytes data_bytesB = data.toByteArray();
  binary = reinterpret_cast<char *>(&data_bytesB);
  EXPECT_EQ("O", data.device);
  EXPECT_EQ(100, (int) static_cast<int16_t>(be16toh(data_bytesB.rear_rpm)));
  EXPECT_EQ(200, (int) static_cast<int16_t>(be16toh(data_bytesB.front_rpm)));
  EXPECT_EQ(300, (int) static_cast<int16_t>(be16toh(data_bytesB.rear_current)));
  EXPECT_EQ(400, (int) static_cast<int16_t>(be16toh(data_bytesB.front_current)));
  EXPECT_EQ(500, (int) static_cast<int16_t>(be16toh(data_bytesB.battery_v)));
  EXPECT_EQ(1000, (int) static_cast<int32_t>(be32toh(data_bytesB.time)));
  EXPECT_EQ('\x75', binary[0]);
  EXPECT_EQ('\xAA', binary[1]);
  EXPECT_EQ('\x00', binary[2]);
  EXPECT_EQ('\x75', binary[17]);
  EXPECT_EQ('\xFF', binary[18]);
}

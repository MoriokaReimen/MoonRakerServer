/*!
-----------------------------------------------------------------------------
@file    test-command.cpp
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
@brief test command class
-----------------------------------------------------------------------------
*/
#include <gtest/gtest.h>

#include "Command.hpp"
#include <portable_endian.h>

TEST(CommandTest, Construct)
{
    MotorCommand command(100, 90, -120, -180);

    EXPECT_EQ(100, command.left_front_rpm);
    EXPECT_EQ(90, command.left_rear_rpm);
    EXPECT_EQ(-120, command.right_front_rpm);
    EXPECT_EQ(-180, command.right_rear_rpm);
}

TEST(CommandTest, SetRPM)
{
    MotorCommand command(100, -120, 0, 0);
    command.set(100, 90, -120, -180);

    EXPECT_EQ(100, command.left_front_rpm);
    EXPECT_EQ(-120, command.right_front_rpm);
}

TEST(CommandTest, ToByteArray)
{
    MotorCommand command(100, 0, -120, 0);
    CommandBytes command_bytesA = command.toLeftByteArray();
    char* binary = reinterpret_cast<char *>(&command_bytesA);

    EXPECT_EQ(15, sizeof(command_bytesA));
    EXPECT_EQ(100, (int) static_cast<int16_t>(be16toh(command_bytesA.left_front_rpm)));
    EXPECT_EQ(-120, (int) static_cast<int16_t>(be16toh(command_bytesA.right_front_rpm)));
    EXPECT_EQ('\x75', binary[0]);
    EXPECT_EQ('\xAA', binary[1]);
    EXPECT_EQ('\x13', binary[2]);
    EXPECT_EQ('\x21', binary[3]);
    EXPECT_EQ('\x00', binary[4]);
    EXPECT_EQ('\x75', binary[13]);
    EXPECT_EQ('\xFF', binary[14]);
}

TEST(CommandTest, FromByteArray)
{
    CommandBytes bytes;
    bytes.left_front_rpm = static_cast<int16_t>(htobe16(100));
    bytes.right_front_rpm = static_cast<int16_t>(htobe16(-120));
    MotorCommand command(bytes);

    EXPECT_EQ(100, command.left_front_rpm);
    EXPECT_EQ(-120, command.right_front_rpm);
}

TEST(CommandTest, COPY)
{
    MotorCommand commanda(100, 0, -120, 0);
    MotorCommand commandb;
    commandb = commanda;
    commanda.set(120, 0, 100, 0);

    EXPECT_EQ(100, commandb.left_front_rpm);
    EXPECT_EQ(-120, commandb.right_front_rpm);
}

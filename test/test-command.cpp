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

#include "command.hpp"
#include <portable_endian.h>

TEST(CommandTest, Construct)
{
    MotorCommand command(100, -120);

    EXPECT_EQ(100, command.left_rpm);
    EXPECT_EQ(-120, command.right_rpm);
}

TEST(CommandTest, SetRPM)
{
    MotorCommand command(100, -120);
    command.set(2000, 3000);

    EXPECT_EQ(2000, command.left_rpm);
    EXPECT_EQ(3000, command.right_rpm);
}

TEST(CommandTest, ToByteArray)
{
    MotorCommand command(100, -120);
    CommandBytes command_bytesA = command.toByteArray();
    char* binary = reinterpret_cast<char *>(&command_bytesA);

    EXPECT_EQ(11, sizeof(command_bytesA));
    EXPECT_EQ(100, (int) static_cast<int16_t>(be16toh(command_bytesA.left_rpm)));
    EXPECT_EQ(-120, (int) static_cast<int16_t>(be16toh(command_bytesA.right_rpm)));
    EXPECT_EQ('\x75', binary[0]);
    EXPECT_EQ('\xAA', binary[1]);
    EXPECT_EQ('\x13', binary[2]);
    EXPECT_EQ('\x00', binary[3]);
    EXPECT_EQ('\x00', binary[4]);
    EXPECT_EQ('\x75', binary[9]);
    EXPECT_EQ('\xFF', binary[10]);
}

TEST(CommandTest, FromByteArray)
{
    CommandBytes bytes;
    bytes.left_rpm = static_cast<int16_t>(htobe16(100));
    bytes.right_rpm = static_cast<int16_t>(htobe16(-120));
    MotorCommand command(bytes);

    EXPECT_EQ(100, command.left_rpm);
    EXPECT_EQ(-120, command.right_rpm);
}

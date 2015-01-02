#include <gtest/gtest.h>

#include "command.hpp"
#include "data.hpp"
#include <endian.h>

TEST(CommandTest,General){
  MotorCommand command(100, -120);
  CommandBytes command_bytes = command.toByteArray();
  char* binary = reinterpret_cast<char *>(&command_bytes);
  EXPECT_EQ(11, sizeof(command_bytes));
  EXPECT_EQ(100, (int) static_cast<int16_t>(be16toh(command_bytes.left_rpm)));
  EXPECT_EQ(-120, (int) static_cast<int16_t>(be16toh(command_bytes.right_rpm)));
  EXPECT_EQ('\x75', binary[0]);
  EXPECT_EQ('\xAA', binary[1]);
  EXPECT_EQ('\x13', binary[2]);
  EXPECT_EQ('\x00', binary[3]);
  EXPECT_EQ('\x00', binary[4]);
  EXPECT_EQ('\x75', binary[9]);
  EXPECT_EQ('\xFF', binary[10]);
  command.set(3000, -3000);
  CommandBytes command_bytesB = command.toByteArray();
  EXPECT_EQ(3000, (int) static_cast<int16_t>(be16toh(command_bytesB.left_rpm)));
  EXPECT_EQ(-3000, (int) static_cast<int16_t>(be16toh(command_bytesB.right_rpm)));
}

TEST(DataTest,General){
  DataBytes data_bytes;
  char* binary = reinterpret_cast<char *>(&data_bytes);
  MotorData data(data_bytes);
  EXPECT_EQ(19, sizeof(data_bytes));
  EXPECT_EQ("O", data.device);
  EXPECT_EQ(0, (int) static_cast<int16_t>(data_bytes.rear_rpm));
  EXPECT_EQ(0, (int) static_cast<int16_t>(data_bytes.front_rpm));
  EXPECT_EQ('\x75', binary[0]);
  EXPECT_EQ('\xAA', binary[1]);
  EXPECT_EQ('\x00', binary[2]);
  EXPECT_EQ('\x75', binary[17]);
  EXPECT_EQ('\xFF', binary[18]);
}


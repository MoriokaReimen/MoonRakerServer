#include <gtest/gtest.h>

#include "command.hpp"
#include "data.hpp"
#include <endian.h>

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


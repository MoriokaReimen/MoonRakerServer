#include <gtest/gtest.h>

#include "command.hpp"
#include "data.hpp"

TEST(CommandTest,General){
  MotorCommand command(100, -120);
  CommandBytes command_bytes = command.toByteArray();
  EXPECT_EQ(11, sizeof(command_bytes));
  EXPECT_EQ(100, (int) static_cast<int16_t>(command_bytes.left_rpm));
  EXPECT_EQ(-120, (int) static_cast<int16_t>(command_bytes.right_rpm));
}

TEST(DataTest,General){
  DataBytes data_bytes;
  MotorData data(data_bytes);
  EXPECT_EQ(19, sizeof(data_bytes));
  //EXPECT_STR_EQ("O", data.device);
  EXPECT_EQ(0, (int) static_cast<int16_t>(data_bytes.rear_rpm));
  EXPECT_EQ(0, (int) static_cast<int16_t>(data_bytes.front_rpm));
}


#include "gtest/gtest.h"
#include "Socket/UDP.hpp"
#include <iostream>
#include <cstring>
#include <thread>

TEST(SOCKET_TEST, INIT_SOCKET)
{
  UDP server(2011);
  UDP client(2012);
  return;
}

TEST(SOCKET_TEST, CLIENT_SEND_STRING)
{
  UDP server(2011);
  server.setTarget("127.0.0.1", 2012);
  UDP client(2012);
  client.setTarget("127.0.0.1", 2011);

  client.write("1234567891011121314151617181920");
  auto message = server.read();
  EXPECT_EQ(message == "1234567891011121314151617181920", true);

  return;
}

TEST(SOCKET_TEST, SERVER_SEND_STRING)
{
  UDP server(2011);
  server.setTarget("127.0.0.1", 2012);
  UDP client(2012);
  client.setTarget("127.0.0.1", 2011);

  server.write("Hello");
  auto message = client.read();
  EXPECT_EQ(message == "Hello", true);

  return;
}

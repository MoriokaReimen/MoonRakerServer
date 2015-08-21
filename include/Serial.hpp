#pragma once

#include "rs232.h"
#include <string>
#include <algorithm>

class Serial
{
private:
  int baud_;
  int port_;

public:
  Serial(int port, int baud);
  ~Serial();
  bool open(int port, int baud);
  bool close();
  size_t write(unsigned char* Data, size_t size);
  size_t read(unsigned char* data, size_t size);
  size_t flash();
};

#include "Serial.hpp"

Serial::Serial(int port, int baud)
{
  this->open(port, baud);

  return;
}

Serial::~Serial()
{
  this->close();

  return;
}

bool Serial::open(int port, int baud)
{
  this->port_ = port;
  OpenComport(port, baud);

  return true;
}

bool Serial::close()
{
  CloseComport(this->port_);
  return true;
}

size_t Serial::write(unsigned char* data, size_t size)
{
  size_t wrote_size = SendBuf(this->port_, data, size);
  return wrote_size;
}

size_t Serial::read(unsigned char* data, size_t size)
{
  /* clean buffer */
  std::fill(data, data + size, 0);

  size_t read_size = PollComport(this->port_, data, size);
  return read_size;
}

#include <iostream>
using namespace std;
#include "command.hpp"
#include "data.hpp"

int main()
{
  DataBytes data;
  CommandBytes command;
  cout << "Data(expected: 19): ";
  cout << sizeof(data) << endl;
  cout << "Command(expected: 11): ";
  cout << sizeof(command) << endl;
  return 0;
};


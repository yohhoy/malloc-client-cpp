// malloc-client.cpp
// SPDX-License-Identifier: MIT
#include <format>
#include <iostream>
#include "malloc-client.hpp"


int main()
{
  std::cout << "malloc REST client" << std::endl;
  mc::RemoteMemory rmem;

  const auto addr = rmem.malloc(1024);
  std::cout << "addr:" << addr << std::endl;

  rmem.write_byte(addr, 128);
  rmem.write<int>(addr + 4, 1234567890);
  rmem.write<double>(addr + 8, 3.14159265);

  std::cout
    << std::format("byte:{}\n", rmem.read_byte(addr))
    << std::format("int:{}\n", rmem.read<int>(addr + 4))
    << std::format("double:{}\n", rmem.read<double>(addr + 8));

  rmem.free(addr);
}

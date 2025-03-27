// malloc-client.cpp
// SPDX-License-Identifier: MIT
#include <format>
#include <iostream>
#include <string>
#include "malloc-client.hpp"


int main()
{
  std::cout << "malloc REST client" << std::endl;
  mc::RemoteMemory rmem;

  const auto addr = rmem.malloc(1024);
  std::cout << "addr:" << addr << std::endl;

  rmem.write_byte(addr, 128);
  std::cout << std::format("byte:{}", rmem.read_byte(addr)) << std::endl;

  rmem.write<int>(addr, 1234567890);
  std::cout << std::format("int:{}", rmem.read<int>(addr)) << std::endl;

  rmem.write<double>(addr, 3.14159265);
  std::cout << std::format("double:{}", rmem.read<double>(addr)) << std::endl;

  rmem.free(addr);
}

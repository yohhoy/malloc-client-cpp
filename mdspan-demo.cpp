// mdspan-demo.cpp
// SPDX-License-Identifier: MIT
#include <iostream>
#include <numeric>
// kokkos/mdspan
#include <experimental/mdspan>
namespace stdex = std::experimental;

#include "malloc-client-mdspan.hpp"


int main()
{
  std::cout << "malloc REST client: mdspan-demo" << std::endl;

  // allocate remote memory on malloc-server
  mc::RemoteMemory rmem;
  auto addr = rmem.malloc(sizeof(int) * 10);

  // create `mdspan` as a view of remote memory
  using extents_type = stdex::dextents<size_t, 1>;
  using mapping_type = stdex::layout_right::mapping<extents_type>;
  using accessor_type = mc::RemoteMemoryAccessor<int>;
  stdex::mdspan arr{addr, mapping_type{extents_type{10}}, accessor_type{rmem}};

  // write to remote memory
  for (size_t i = 0; i < arr.extent(0); i++) {
    arr[i] = i;
  }

  // read from remote memory
  for (size_t i = 0; i < arr.extent(0); i++) {
    std::cout << (i ? " " : "") << arr[i];
  }
  std::cout << std::endl;

  rmem.free(addr);
}

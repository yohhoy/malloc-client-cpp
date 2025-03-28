// mdspan-demo.cpp
// SPDX-License-Identifier: MIT
#include <iostream>
// kokkos/mdspan
#include <experimental/mdspan>
namespace stdx = std::experimental;

#include "malloc-client-mdspan.hpp"


int main()
{
  std::cout << "malloc REST client: mdspan-demo" << std::endl;
  constexpr int N = 10;

  // allocate remote memory on malloc-server
  mc::RemoteMemory rmem;
  mc::RemoteAddress addr = rmem.malloc(sizeof(int) * N);

  // create `mdspan` as a view of remote memory
  using extents_type = stdx::dextents<size_t, 1>;  // 1D-array[dynamic]
  using mapping_type = stdx::layout_right::mapping<extents_type>;
  using accessor_type = mc::RemoteMemoryAccessor<int>;
  stdx::mdspan arr{addr, mapping_type{extents_type{N}}, accessor_type{rmem}};

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

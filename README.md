# malloc-client-cpp

C++ client for [malloc REST Server](https://github.com/yohhoy/malloc-server)

```sh
# run `malloc-server`

$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .

$ ./malloc-client
$ ./mdspan-demo
```

## Dependencies

- [libcpr/cpr](https://github.com/libcpr/cpr) - [libcurl](https://curl.se/libcurl/) C++ wrapper
- [nlohmann/json](https://github.com/nlohmann/json) - JSON utility
- [kokkos/mdspan](https://github.com/kokkos/mdspan) - `std::mdspan`


# License

MIT License

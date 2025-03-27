cmake_minimum_required(VERSION 3.16)
project(malloc-client-cpp)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
# https://github.com/libcpr/cpr
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.2
)
FetchContent_MakeAvailable(cpr)
# https://github.com/nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

add_executable(malloc-client
    malloc-client.cpp)
target_link_libraries(malloc-client PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json)
target_compile_options(malloc-client PRIVATE -Wall -Wextra -pedantic)

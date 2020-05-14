cmake_minimum_required(VERSION 3.13)

project(PongServer C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -Wshadow -Wfatal-errors -Wunused -Wpedantic -Warray-bounds")

add_executable(server server.cpp)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

cmake_minimum_required(VERSION 3.13)

project(PongServer C)

find_package(Threads REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -Wshadow -Wfatal-errors -Wunused -Wpedantic -Warray-bounds")

add_executable(server main.c)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

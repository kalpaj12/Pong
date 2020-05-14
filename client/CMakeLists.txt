# credits: https://ryozuki.xyz/blog/sdl2_cmake.html

cmake_minimum_required(VERSION 3.13)

project(Pong C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# sdl packages
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

include_directories(src/include)
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wshadow -Wfatal-errors -Wnon-virtual-dtor -Wunused -Wpedantic -Warray-bounds")

add_executable(pong ${SOURCES} main.cpp)

if( MULTIPLAYER )
	message("GAME MODE: MULTIPLAYER")
	add_definitions(-DMULTIPLAYER)
endif ()

target_link_libraries(pong ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(pong SDL2::Main SDL2::Mixer SDL2::Image SDL2::TTF SDL2::Net)
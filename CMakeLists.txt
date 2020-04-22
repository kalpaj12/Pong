cmake_minimum_required(VERSION 3.13)

project(Pong)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wshadow -Wfatal-errors -Wnon-virtual-dtor -Wunused -Wpedantic -Warray-bounds")

# sdl flags
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

include_directories(src/include)
file(GLOB SOURCES "src/*.cpp")

add_executable(pong ${SOURCES} main.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

target_link_libraries(pong ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
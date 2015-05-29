cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name asteroids)
set(sources main.cpp)

add_executable(${name} ${sources})
target_link_libraries(${name} sdl2 sdl2_image)

file(COPY loaded.png DESTINATION ${CMAKE_BINARY_DIR})

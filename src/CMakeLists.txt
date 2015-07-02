cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name asteroids)
file(GLOB sources "*.cpp")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${FREETYPE_INCLUDE_DIRS})

add_executable(${name} ${sources})

target_link_libraries(
    ${name}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${GLFW_STATIC_LIBRARIES}
    ${FREETYPE_LIBRARIES})

file(COPY Hyperspace.otf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

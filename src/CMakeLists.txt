cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name asteroids)
set(sources main.cpp logger.cpp window.cpp glfw_app.cpp shader.cpp program.cpp vao.cpp windowed_app.cpp engine.cpp)

add_executable(${name} ${sources})

find_package(OpenGL REQUIRED)
find_package(glew REQUIRED)

target_link_libraries(${name} glfw3 ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

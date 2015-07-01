cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name asteroids)
set(sources main.cpp logger.cpp window.cpp glfw_app.cpp program.cpp vao.cpp windowed_app.cpp drawable.cpp asteroid.cpp bullet.cpp ship.cpp space_object.cpp screen_boundary.cpp generic_shader.cpp ship_graphic.cpp bullet_graphic.cpp asteroid_graphic.cpp life_counter.cpp particle_system.cpp geometry_helpers.cpp)

add_executable(${name} ${sources})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(${name} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_STATIC_LIBRARIES})

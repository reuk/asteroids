cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name Asteroids)
file(GLOB sources "*.cpp")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${FREETYPE_INCLUDE_DIRS})

IF(APPLE)
    SET(MACOSX_BUNDLE_ICON_FILE asteroids.icns)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/asteroids.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/Hyperspace.otf PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF(APPLE)

set(sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/asteroids.icns)
set(sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/Hyperspace.otf)

set(GLFW_USE_RETINA true)

add_executable(${name} MACOSX_BUNDLE ${sources})

target_link_libraries(
    ${name}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${GLFW_STATIC_LIBRARIES}
    ${FREETYPE_LIBRARIES})

install(TARGETS ${name}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime)

set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)

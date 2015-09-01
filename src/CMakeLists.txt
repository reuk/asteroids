cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name Asteroids)
file(GLOB sources "*.cpp")

SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

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

set(libraries
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${GLFW_STATIC_LIBRARIES}
    ${FREETYPE_LIBRARIES})

if(UNIX)
    set(libraries ${libraries} png z)
    file(COPY Hyperspace.otf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(UNIX)

target_link_libraries(
    ${name}
    ${libraries})

install(TARGETS ${name}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime)

set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)

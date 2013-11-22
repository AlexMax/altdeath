cmake_minimum_required(VERSION 2.8)

project(altdeath)

# Compiler Flags
if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
endif()

# Source Files
file(GLOB ALTDEATH_HEADERS *.hpp)
file(GLOB ALTDEATH_SOURCES *.cpp)
set(GL_HEADERS opengl/gl_core_3_1.hpp)
set(GL_SOURCES opengl/gl_core_3_1.cpp)

# Generate lexer using Ragel
set(RAGEL_FLAGS "")
set(RAGEL_EXTENSION "cpp")
find_package(Ragel REQUIRED)
ragel_parser(udmfLexer.rl)
list(APPEND ALTDEATH_SOURCES ${CMAKE_BINARY_DIR}/source/udmfLexer.cpp)

# Executable Target
add_executable(altdeath ${ALTDEATH_SOURCES} ${ALTDEATH_HEADERS} ${GL_SOURCES} ${GL_HEADERS})

# SDL2 Library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(altdeath ${SDL2_LIBRARY})

# System OpenGL Library
find_package(OpenGL REQUIRED)
target_link_libraries(altdeath ${OPENGL_gl_LIBRARY})

# GLM Library
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

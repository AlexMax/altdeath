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
set(RAGEL_FLAGS "-L")
set(RAGEL_EXTENSION "cpp")
find_package(Ragel REQUIRED)
ragel_parser(udmfLexer.rlpp)
list(APPEND ALTDEATH_SOURCES ${CMAKE_BINARY_DIR}/source/udmfLexer.cpp)

# Generate parser using Lemon
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/source/udmfParser.cpp ${CMAKE_BINARY_DIR}/source/udmfParser.hpp
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/source/udmfParser.ypp ${CMAKE_BINARY_DIR}/source/udmfParser.ypp
  COMMAND lemon -l ${CMAKE_BINARY_DIR}/source/udmfParser.ypp T=${CMAKE_SOURCE_DIR}/tools/lemon/lempar.c
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/source/udmfParser.c ${CMAKE_BINARY_DIR}/source/udmfParser.cpp
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/source/udmfParser.h ${CMAKE_BINARY_DIR}/source/udmfParser.hpp
  DEPENDS lemon udmfParser.ypp
)
list(APPEND ALTDEATH_HEADERS ${CMAKE_BINARY_DIR}/source/udmfParser.hpp)
list(APPEND ALTDEATH_SOURCES ${CMAKE_BINARY_DIR}/source/udmfParser.cpp)

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

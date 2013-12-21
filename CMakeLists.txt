cmake_minimum_required(VERSION 2.8)

project(altdeath)

# Initial CMake Setup
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Altdeath engine proper
add_subdirectory(source)

# Static Libraries
add_subdirectory(lib)

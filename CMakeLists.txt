cmake_minimum_required(VERSION 2.8)

# Initial CMake Setup
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Altdeath engine proper
add_subdirectory(source)

# Additional tools needed to compile the project
add_subdirectory(tools)

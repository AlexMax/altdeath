cmake_minimum_required(VERSION 2.8)

project(enet C)

file(GLOB ENET_HEADERS include/enet/*.h)
file(GLOB ENET_SOURCES *.c)

add_library(enet ${ENET_SOURCES} ${ENET_HEADERS})
target_include_directories(enet PUBLIC include)

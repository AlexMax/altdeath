cmake_minimum_required(VERSION 2.8)

project(enet C)

file(GLOB ENET_HEADERS include/enet/*.h)
file(GLOB ENET_SOURCES *.c)

if(WIN32)
  list(REMOVE_ITEM ENET_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/enet/unix.h")
  list(REMOVE_ITEM ENET_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/unix.c")
else()
  list(REMOVE_ITEM ENET_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/enet/win32.h")
  list(REMOVE_ITEM ENET_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/win32.c")

  # Platform checks
  include(CheckFunctionExists)
  include(CheckTypeSize)

  check_function_exists(gethostbyaddr_r HAS_GETHOSTBYADDR_R)
  if(HAS_GETHOSTBYADDR_R)
    add_definitions(-DHAS_GETHOSTBYADDR_R)
  endif()

  check_function_exists(gethostbyname_r HAS_GETHOSTBYNAME_R)
  if(HAS_GETHOSTBYNAME_R)
    add_definitions(-DHAS_GETHOSTBYNAME_R)
  endif()

  check_function_exists(poll HAS_POLL)
  if(HAS_POLL)
    add_definitions(-DHAS_POLL)
  endif()

  check_function_exists(fcntl HAS_FCNTL)
  if(HAS_FCNTL)
    add_definitions(-DHAS_FCNTL)
  endif()

  check_function_exists(inet_pton HAS_INET_PTON)
  if(HAS_INET_PTON)
    add_definitions(-DHAS_INET_PTON)
  endif()

  check_function_exists(inet_ntop HAS_INET_NTOP)
  if(HAS_INET_NTOP)
    add_definitions(-DHAS_INET_NTOP)
  endif()

  set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h" "sys/socket.h")
  check_type_size(socklen_t HAS_SOCKLEN_T)
  set(CMAKE_EXTRA_INCLUDE_FILES "")
  if(HAS_SOCKLEN_T)
    add_definitions(-DHAS_SOCKLEN_T)
  endif()
endif()

add_library(enet ${ENET_SOURCES} ${ENET_HEADERS})
target_include_directories(enet PUBLIC include)

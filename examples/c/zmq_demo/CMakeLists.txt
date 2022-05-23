cmake_minimum_required(VERSION 3.16)

project(zmq-demo C CXX)

# Set default build type to release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")

# Include rules
include(FindPkgConfig)
include(utils.cmake)

# Dependencies
pkg_check_modules(CZMQ libczmq)
link_directories(${CZMQ_LIBRARY_DIRS})
include_directories(${CZMQ_INCLUDE_DIRS})

# Build flags
set(C_WARNINGS "-Wall -Wstrict-prototypes -Wshadow -Werror")
set(CXX_WARNINGS "-Wall -Wshadow -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${C_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 ${CXX_WARNINGS}")

# Example executables
add_executable(zmq_server_rep_pthread src/zmq_server_rep_pthread.c)
target_link_libraries(zmq_server_rep_pthread pthread ${CZMQ_LIBRARIES})

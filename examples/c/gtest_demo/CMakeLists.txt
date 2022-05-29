cmake_minimum_required(VERSION 3.16)

project(gtest-demo C CXX)

if (NOT LIB_NAME)
  set (LIB_NAME demo_api)
endif()

# Set default build type to release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")

# Include rules
include(FindPkgConfig)
include(tools/cmake/utils.cmake)
include(CTest)

# Dependencies

# Build flags
set(C_WARNINGS "-Wall -Wstrict-prototypes -Wshadow -Werror")
set(CXX_WARNINGS "-Wall -Wshadow -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${C_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 ${CXX_WARNINGS}")

# Library to test (demo_api)
ADD_LIB_DIRS(src)
add_library(${LIB_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIB_NAME} pthread atomic)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Testing
add_subdirectory(test EXCLUDE_FROM_ALL)

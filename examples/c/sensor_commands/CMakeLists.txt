cmake_minimum_required(VERSION 3.16)

project(sensor-commands C CXX)

if (NOT LIB_NAME)
  set (LIB_NAME sensor_commands)
endif()

set(VERSION "0.0.1")

# Set default build type to release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")

# Include rules
include(FindPkgConfig)
include(utils.cmake)

# Build flags
set(C_WARNINGS "-Wall -Wstrict-prototypes -Wshadow -Werror")
set(CXX_WARNINGS "-Wall -Wshadow -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${C_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 ${CXX_WARNINGS}")

# Dependencies
pkg_check_modules(CJSON REQUIRED libcjson>=1.7.15)
add_definitions(${CJSON_CFLAGS})
link_directories(${CJSON_LIBRARY_DIRS})
include_directories(${CJSON_INCLUDE_DIRS})

# Shared library
ADD_LIB_DIRS(src)

set(VERSION_SOURCE ${CMAKE_BINARY_DIR}/src/version.c)
configure_file(
  src/version.c.in
  "${VERSION_SOURCE}"
  @ONLY
)
list(APPEND LIB_SOURCES ${VERSION_SOURCE})

add_library(${LIB_NAME} SHARED ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} pthread atomic ${CJSON_LIBRARIES})

set_target_properties(
  ${LIB_NAME} PROPERTIES VERSION
  ${VERSION}
)

install(
  TARGETS ${LIB_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)

include_directories(${INCLUDE_DIR})

install(
  DIRECTORY ${INCLUDE_DIR}/${LIB_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  PATTERN "_*" EXCLUDE
  PATTERN "external" EXCLUDE
)

configure_file(
  ${LIB_NAME}.pc.in
  ${CMAKE_BINARY_DIR}/${LIB_NAME}.pc
  @ONLY
)
install(
  FILES ${CMAKE_BINARY_DIR}/${LIB_NAME}.pc
  DESTINATION lib/pkgconfig
)

install(
  DIRECTORY ${CONFIG_DIR}
  DESTINATION /etc/${LIB_NAME}
)

# Custom targets
set(TMP_SYSROOT_PATH ${CMAKE_BINARY_DIR}/tmp_sysroot)

add_custom_command(
  OUTPUT ${TMP_SYSROOT_PATH}
  DEPENDS ${LIB_NAME}
  COMMENT "Deploy sysroot for experiments..."
  COMMAND rm -Rf ${TMP_SYSROOT_PATH}
  COMMAND make install DESTDIR=${TMP_SYSROOT_PATH}
)
add_custom_target(deploy_tmp_sysroot DEPENDS ${TMP_SYSROOT_PATH})

add_subdirectory(tools)

include(FetchContent)

# Fetch gtest and add it to CMake
set(GTEST_RELEASE 1.11.0)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-${GTEST_RELEASE}
  GIT_PROGRESS   TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Testing utilities
include_directories(testutil/include)

add_subdirectory(testutil)

# Unittest executable

# NOTE: CMake discourages file globbing as it collects the files
# on cmake configuration (need to rerun cmake when adding new files).
# We use it here for simplicity since manually adding the test files
# can get tedious.
file(GLOB_RECURSE UNITTEST_SOURCES tests/*.cpp)
add_executable(unittest unittest_main.cpp ${UNITTEST_SOURCES})
target_link_libraries(unittest ${LIB_NAME} gtest testutil)

# NOTE: This test discovery command is for the CTest runner
gtest_discover_tests(unittest)

# Custom runner target for the unittest executable
add_custom_target(run_unittest
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/unittest $(GTEST_ARGS)
  DEPENDS unittest
)

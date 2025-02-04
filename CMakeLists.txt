cmake_minimum_required(VERSION 3.10)
project(RandomForestImp)

set(GOOGLETEST_VERSION 1.15.2)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

enable_testing() #to  discover tests in test explorer

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

add_subdirectory(googletest) # add googletest subdirectory

include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

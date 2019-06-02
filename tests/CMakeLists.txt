cmake_minimum_required(VERSION 3.0)
project(la_tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(la_tests tests.cpp)
target_include_directories(la_tests PRIVATE Catch2/single_include/catch2)

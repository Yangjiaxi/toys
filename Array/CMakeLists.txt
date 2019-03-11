cmake_minimum_required(VERSION 3.0.0)
project(Array VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

find_package(Catch2 REQUIRED)

include_directories(src/)
file(GLOB Src src/*.h src/*.cc)

# main
add_executable(Array ${Src} main.cc)


# Tests
add_executable(test_Array ${Src} test/test_Array.cc)
target_link_libraries(test_Array Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(test_Array)
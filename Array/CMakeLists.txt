cmake_minimum_required(VERSION 3.0.0)
project(Array VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/)

file(GLOB Src main.cc src/*.h src/*.cc)

add_executable(Array ${Src})


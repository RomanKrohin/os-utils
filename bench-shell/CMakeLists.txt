cmake_minimum_required(VERSION 3.10)
project(LabProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")

add_executable(shell shell.cpp)
add_executable(bench1 bench1.cpp)
add_executable(bench2 bench2.cpp)
add_executable(multi_bench multi_bench.cpp)

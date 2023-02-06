cmake_minimum_required(VERSION 3.18)
project(multithreaded_sorting_algorithms)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread")
add_compile_options(-pthread)

add_executable(multithreaded_sorting_algorithms main.cpp sorting_algorithms.hpp table.h spinners.hpp)

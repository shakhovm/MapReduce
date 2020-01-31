cmake_minimum_required(VERSION 3.12)
project(MapReduce)

set(CMAKE_CXX_STANDARD 17)

add_executable(MapReduce main.cpp my_reduce.h my_map.h)
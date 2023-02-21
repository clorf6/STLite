cmake_minimum_required(VERSION 3.22)
project(STLite)

set(CMAKE_CXX_STANDARD 14)

include_directories(vector/data)
include_directories(vector/src)

add_executable(STLite
        vector/data/four.memcheck/code.cpp)

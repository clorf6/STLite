cmake_minimum_required(VERSION 3.22)
project(STLite)

set(CMAKE_CXX_STANDARD 17)

include_directories(map/data)
include_directories(map/src)
include_directories(priority_queue/data)
include_directories(priority_queue/src)
include_directories(vector/data)
include_directories(vector/src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "-fmax-errors=10 -O2 -DONLINE_JUDGE -lm -std=c++17")
add_compile_options(-fmax-errors=10 -O2 -DONLINE_JUDGE -lm -std=c++17)

add_executable(vector vector/data/one/code.cpp)
add_executable(priority_queue priority_queue/data/one/code.cpp)
add_executable(map map/data/five/code.cpp)

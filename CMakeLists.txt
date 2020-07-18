cmake_minimum_required(VERSION 3.16)
project(clion)

set(CMAKE_CXX_STANDARD 14)

add_executable(clion main.cpp mask_simulation.h functions.cpp)
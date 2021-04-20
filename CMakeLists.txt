cmake_minimum_required(VERSION 3.19)
project(hw1_smash)

set(CMAKE_CXX_STANDARD 11)

add_executable(hw1_smash  Commands.cpp signals.cpp smash.cpp)
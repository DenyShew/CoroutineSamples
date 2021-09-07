cmake_minimum_required(VERSION 3.21)
project(Task CXX)

add_library(Task STATIC Task.hpp Task.cpp)

target_link_libraries(Task PUBLIC Waiter)
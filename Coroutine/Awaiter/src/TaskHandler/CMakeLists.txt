cmake_minimum_required(VERSION 3.21)
project(TaskHandler CXX)

add_library(TaskHandler STATIC TaskHandler.hpp TaskHandler.cpp)

target_link_libraries(TaskHandler PUBLIC Task)
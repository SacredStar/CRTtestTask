cmake_minimum_required(VERSION 3.2.0) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
project(TestCRTtask)	
set(SRC_LIST main.cpp Messages.cpp Messages.h json.hpp)
add_executable(EXECUTABLE ${SRC_LIST})


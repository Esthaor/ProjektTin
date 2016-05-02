cmake_minimum_required(VERSION 3.5)
project(Agent)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Agent.cpp include/Agent.h)
add_executable(Agent ${SOURCE_FILES} src/Agent.cpp include/Agent.h)

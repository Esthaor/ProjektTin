cmake_minimum_required(VERSION 3.5)
project(ProjektTin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Server.cpp include/Server.h)
add_executable(ProjektTin ${SOURCE_FILES} src/Server.cpp include/Server.h)
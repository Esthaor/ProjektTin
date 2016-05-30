cmake_minimum_required(VERSION 3.5)

project(ProjektTin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpcap -lsqlite3")

set(AGENT_SOURCE_FILES Agent/src/main.cpp Agent/src/Agent.cpp Agent/include/Agent.h Agent/src/Socket.cpp Agent/include/Socket.h)
set(SERVER_SOURCE_FILES Server/src/main.cpp Server/src/Server.cpp Server/include/Server.h Server/src/Socket.cpp Server/include/Socket.h Agent/Database.cpp Agent/Database.h Server/src/Database.cpp Server/include/Database.h)

add_executable(SIDagent ${AGENT_SOURCE_FILES})
add_executable(SIDserver ${SERVER_SOURCE_FILES} Agent/Database.cpp Agent/Database.h Server/src/Database.cpp Server/include/Database.h)
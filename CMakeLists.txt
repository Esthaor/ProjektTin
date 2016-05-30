cmake_minimum_required(VERSION 3.5)
project(ProjektTin)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

set(AGENT_SOURCE_FILES
        Agent/src/main.cpp
        Agent/src/Agent.cpp
        Agent/src/Socket.cpp
        )

set(SERVER_SOURCE_FILES
        Server/src/main.cpp
        Server/src/Server.cpp
        Server/src/Socket.cpp
        )

find_package(Boost COMPONENTS system thread REQUIRED)
find_package(Threads REQUIRED)
find_package(sqlite3 REQUIRED)

add_executable(SIDagent ${AGENT_SOURCE_FILES})
target_include_directories(SIDagent PRIVATE
        Agent/include
        ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(SIDagent ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} pcap)

add_executable(SIDserver ${SERVER_SOURCE_FILES})
target_include_directories(SIDserver PRIVATE
        Server/include
        ${Boost_INCLUDE_DIRS}
        )
target_link_libraries(SIDserver ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SQLITE3_LIBRARY} pcap)
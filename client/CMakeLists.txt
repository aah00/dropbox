cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)
set(BINARY client)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} client.cpp)

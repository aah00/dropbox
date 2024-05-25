cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

project(Dropbox C CXX)

# set(BINARY ${CMAKE_PROJECT_NAME}_test)

# file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
# set(SOURCES ${TEST_SOURCES})

add_subdirectory(utils)
add_subdirectory(server)
add_subdirectory(client)
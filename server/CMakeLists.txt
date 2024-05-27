cmake_minimum_required(VERSION 3.17)

set(BINARY server)

# file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp *.hpp)
# set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} server.cpp)
target_link_libraries(${BINARY} PUBLIC utilsLib)

add_library(ServerLib STATIC server.cpp) # Can also be "SHARED"
target_include_directories(ServerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utils/)

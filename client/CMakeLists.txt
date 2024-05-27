cmake_minimum_required(VERSION 3.17)

set(BINARY client)

# file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
# set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} client.cpp)
target_link_libraries(${BINARY} PRIVATE utilsLib)

add_library(ClientLib STATIC client.cpp) # Can also be "SHARED"
target_include_directories(ClientLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utils/)
# target_sources(${BINARY} PRIVATE ${other_SRC})


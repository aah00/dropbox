cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)
set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})
add_subdirectory(lib/googletest)

target_link_libraries(${BINARY} PUBLIC FootballScoreBoardLib gtest)
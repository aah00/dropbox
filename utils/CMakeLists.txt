set(target utilsLib)

add_library(${target} STATIC fileop.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
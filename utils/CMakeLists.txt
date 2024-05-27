set(target utilsLib)

# file(GLOB_RECURSE UTILS_SOURCES LIST_DIRECTORIES false *.cpp)
# set(SOURCES ${UTIL_SOURCES})
# add_library(${target} STATIC ${UTILS_SOURCES})

add_library(${target} STATIC
    socket.cpp
    clientSocket.cpp
    serverSocket.cpp
    fileop.cpp)

# target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
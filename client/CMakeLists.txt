cmake_minimum_required(VERSION 3.10)
project(client)

set(RPCLIB_INCLUDE_DIR "$ENV{HOME}/rpclib/include")
set(RPCLIB_LIBRARY_DIR "$ENV{HOME}/rpclib/build")

include_directories(${RPCLIB_INCLUDE_DIR})

add_executable(client main.cpp)

target_link_libraries(client PRIVATE ${RPCLIB_LIBRARY_DIR}/librpc.a)
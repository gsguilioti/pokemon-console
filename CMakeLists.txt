cmake_minimum_required(VERSION 3.10)
project(pokemon_console)

set(RPCLIB_DIR "$ENV{HOME}/rpclib/build")

find_package(rpclib REQUIRED PATHS ${RPCLIB_DIR})

add_executable(pokemon_console main.cpp)

target_link_libraries(pokemon_console PRIVATE ${RPCLIB_DIR}/librpc.a)
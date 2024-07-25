cmake_minimum_required(VERSION 3.20)
project(hello_world)

set(CMAKE_CXX_STANDARD 11)


add_executable(hello_world main.cpp)

install(TARGETS hello_world  DESTINATION bin)

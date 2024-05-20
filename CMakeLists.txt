cmake_minimum_required(VERSION 3.20)
project(cpp17_sandbox)

set(CMAKE_CXX_STANDARD 17)

### OpenCV Libraries ###
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS})

add_executable(cpp17_sandbox main.cpp)

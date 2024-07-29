cmake_minimum_required(VERSION 3.2)
project(Dijkstra)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the Python library
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

add_executable(dijkstra dijkstra.cpp)
target_link_libraries(dijkstra ${Python3_LIBRARIES})

cmake_minimum_required(VERSION 3.2)
project(cubic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the Python library
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

add_executable(cubic_spline cubic_spline.cpp)
target_link_libraries(cubic_spline ${Python3_LIBRARIES})

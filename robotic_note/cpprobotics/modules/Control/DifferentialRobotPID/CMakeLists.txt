cmake_minimum_required(VERSION 3.2)
project(pid_controller_1)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/usr/include/eigen3")
include_directories(include)

# Find the Python library
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

add_executable(pid_controller_1 main.cpp)
target_link_libraries(pid_controller_1 Eigen3::Eigen ${OpenCV_LIBS})

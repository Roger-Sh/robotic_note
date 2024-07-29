cmake_minimum_required(VERSION 3.2)
project(rear_wheel_fb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the Python library
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

add_executable(rear_wheel_fb RearWheelFeedbackControl.cpp KinematicModel.cpp MyReferencePath.cpp RearWheelFeedback.cpp)
target_link_libraries(rear_wheel_fb ${Python3_LIBRARIES})

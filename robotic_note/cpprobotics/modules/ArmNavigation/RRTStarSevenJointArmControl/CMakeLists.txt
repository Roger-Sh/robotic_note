cmake_minimum_required(VERSION 3.2)
project(rrt_star_seven_joint_arm_control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find Eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Find the Python library
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

# arm_obstacle_navigation
add_executable(rrt_star_seven_joint_arm_control rrt_star_seven_joint_arm_control.cpp)
target_link_libraries(rrt_star_seven_joint_arm_control ${EIGEN3_LIBIRARIES} ${Python3_LIBRARIES} Matplot++::matplot)

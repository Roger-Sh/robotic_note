cmake_minimum_required(VERSION 3.2)
project(n_link_arm_3d)

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

# random_forward_kinematics_3d
add_executable(random_forward_kinematics_3d random_forward_kinematics_3d.cpp)
target_link_libraries(random_forward_kinematics_3d ${EIGEN3_LIBIRARIES} Matplot++::matplot)

# random_inverse_kinematics_3d
add_executable(random_inverse_kinematics_3d random_inverse_kinematics_3d.cpp)
target_link_libraries(random_inverse_kinematics_3d ${EIGEN3_LIBIRARIES} Matplot++::matplot)

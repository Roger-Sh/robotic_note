cmake_minimum_required(VERSION 3.10)
project(aiforce_robot)

# 设置环境支持C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(modules/include)

add_subdirectory(3rdparty/matplotplusplus)

add_subdirectory(modules/ArmNavigation)

add_subdirectory(modules/Localization)

add_subdirectory(modules/Control)

add_subdirectory(modules/PathPlanning)

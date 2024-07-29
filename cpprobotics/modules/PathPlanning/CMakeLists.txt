cmake_minimum_required(VERSION 3.10)
project(PathPlanning)

add_subdirectory(A_star)

add_subdirectory(Dijkstra)

add_subdirectory(DWA)

add_subdirectory(BreadthFirstSearch)

add_subdirectory(DepthFirstSearch)

add_subdirectory(CubicSpline)

add_subdirectory(FrenetOptimalTrajectory)

add_subdirectory(DubinsPath)

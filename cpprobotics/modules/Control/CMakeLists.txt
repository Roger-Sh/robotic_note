cmake_minimum_required(VERSION 3.10)
project(Control)

add_subdirectory(PP)

add_subdirectory(Stanley)

add_subdirectory(LQRSteerControl)

add_subdirectory(RearWheelFeedback)

add_subdirectory(DifferentialRobotPID)

add_subdirectory(DiffRobotVelSteerPID)

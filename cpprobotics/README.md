# CppRobotics

This is the cpp implementation of the PythonRobotics.

## Requirements

* Eigen3

* OpenCV 3.2

* Python3-matplotlib

  ```bash
  sudo apt-get install python3-matplotlib
  ```
  
* Python3-numpy

  ```bash
  sudo apt-get install python3-numpy
  ```
  
* [matplotplusplus](https://github.com/alandefreitas/matplotplusplus)

  * A C++ Graphics Library for Data Visualization
  
  * depend
  
    ```
    sudo apt-get install libfftw3-dev
    sudo apt-get install gnuplot
    ```
  
  * how to add matplotplusplus to CMake project

    * cmake 3.14+
    * add matplotplusplus source folder to CMake project
  
      ```cmake
      add_subdirectory(3rdparty/matplotplusplus)
      ```
    * link the library to the targets you want
  
      ```cmake
      add_executable(my_target main.cpp)
      target_link_libraries(my_target Matplot++::matplot)
      ```
    * Add this header to your source files:
  
      ```c++
      #include <matplot/matplot.h>
      ```
    * build with examples
  
      ```bash
      cmake -DMATPLOTPP_BUILD_EXAMPLES=ON ..
      ```

## Getting started

Just clone the code, `mkdir build, cd build, cmake .., make -j6`
Then, you are good to go!

## Test result

- AerialNavigation
- ArmNavigation

  - [X] TwoJointArmToPointControl
  - [X] NJointArmToPointControl
  - [X] NLinkArm3d
  - [X] ArmObstacleNavigation
  - [X] RRTStarSevenJointArmControl
- Bipedal
- Control

  - [X] DiffRobotVelSteerPID
  - [X] DifferentialRobotPID
  - [X] LQRSteerControl
  - [X] PurePursuit
  - [X] RearWheelFeedback
  - [X] Stanley
- Localization

  - [X] ExtendedKalmanFilter
  - [X] HistogramFilter
  - [X] ParticleFilter
- Mapping
- Path Planning

  - [X] DWA (Dynamic Window Approach)
  - [ ] Grid based search
    - [X] Dijkstra
    - [X] AStar
    - [X] DepthFirstSearch
    - [X] BreadthFirstSearch
  - [X] FrenetOptimalTrajectory
  - [X] CubicSpline
  - [X] DubinsPath
- SLAM

## Timeline

2023/05/31: Add [RRTStarSevenJointArmControl](./modules/ArmNavigation/RRTStarSevenJointArmControl/rrt_star_seven_joint_arm_control.cpp)

2023/05/20: Add [DiffRobotVelSteerPID](./modules/Control/DiffRobotVelSteerPID/main.cpp), new version of PID

2023/05/19: Add [ArmObstacleNavigation](./modules/ArmNavigation/ArmObstacleNavigation/arm_obstacle_navigation.cpp)

2023/05/18: Add [NLinkArm3d](./modules/ArmNavigation/NLinkArm3d/NLinkArm3d.h)

2023/05/16: Add [NJointArmToPointControl](./modules/ArmNavigation/NJointArmToPointControl/n_joint_arm_to_point_control.cpp)

2023/05/09: Add [DubinsPath](./modules/PathPlanning/DubinsPath/Dubins.cpp)

2023/04/28: Add [FrenetOptimalTrajectory](./modules/PathPlanning/FrenetOptimalTrajectory/frenet_optimal_trajectory.cpp)

2023/04/26: Add [DifferentialRobotPID](./modules/Control/DifferentialRobotPID/main.cpp)

2023/04/25: Add [CubicSpline](./modules/PathPlanning/CubicSpline/cubic_spline.cpp)

2023/04/24: Add [DWA](./modules/PathPlanning/DWA/dwa.cpp), [BreadthFirstSearch](./modules/PathPlanning/BreadthFirstSearch/BreadthFirstSearch.cpp), [DepthFirstSearch](./modules/PathPlanning/BreadthFirstSearch/DepthFirstSearch.cpp), [RearWheelFeedback](./modules/Control/RearWheelFeedback/RearWheelFeedbackControl.cpp)

2023/04/23: Add [LQRSteerControl](./modules/Control/LQRSteerControl/lqr_steer_control.cpp), [ExtendedKalmanFilter](./modules/Localization/ExtendedKalmanFilter/extended_kalman_filter.cpp)

2023/04/23: First version of only 6 functions

## Acknowledgement

Many thanks to these excellent projects:

- [PythonRobotics](https://github.com/AtsushiSakai/PythonRobotics)
- [CppRobotics](https://github.com/onlytailei/CppRobotics)
- [chhRobotics](https://github.com/CHH3213/chhRobotics)

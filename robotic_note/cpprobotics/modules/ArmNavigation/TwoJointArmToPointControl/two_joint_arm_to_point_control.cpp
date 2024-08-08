/**
 * @file two_joint_arm_to_point_control.cpp
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief Inverse kinematics of a two-joint arm
 * @version 0.1
 * @date 2023-05-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

/**
 * @brief global parameter
 *
 */

// simulation parameter
const double Kp = 20.0;   // arm update step size
const double dt = 0.01;   // arm update time interval

// link length
const double l1 = 1.0;
const double l2 = 1.0;

// sleep time
const int sleep_time = 2000000;

// dist to goal init value
const int d2goal_init = 100;

// goal position of end-effector
// std::vector<double> x = {2};
// std::vector<double> y = {0};
double goal_x = 2.5;
double goal_y = 1.0;
double d2goal_prev = 100;
std::vector<double> goal_x_vec = {1.0, 1.3, -1.2, 1.8, 1.5, -2.0, -2.5};
std::vector<double> goal_y_vec = {1.0, -1.2, 1.3, 1.6, -1.5, -1.5, 2.0};

// settings
bool show_animation = true;

/**
 * @brief Returns the difference between two angles in the range -pi to +pi
 *
 * @param theta1
 * @param theta2
 * @return double
 */
double ang_diff(double theta1, double theta2)
{
    return std::fmod(theta1 - theta2 + M_PI, 2 * M_PI) - M_PI;
}

/**
 * @brief check goal pose x, y in range [-3, 3]
 *
 * @param x
 * @param y
 * @return true
 * @return false
 */
bool check_goal_pose(double x, double y)
{
    if (x < -3.0 || x > 3.0 || y < -3.0 || y > 3.0)
    {
        std::cout << "x: " << x << "\n";
        std::cout << "y: " << y << "\n";
        return false;
    }

    return true;
}

/**
 * @brief calc arm pose and plot arm
 *
 * @param theta1
 * @param theta2
 * @param target_x
 * @param target_y
 * @return std::vector<double>
 */
std::vector<double> plot_arm(double theta1, double theta2, double target_x, double target_y)
{
    std::vector<double> shoulder = {0, 0};
    std::vector<double> elbow = {l1 * std::cos(theta1), l1 * std::sin(theta1)};
    std::vector<double> wrist = {elbow[0] + l2 * std::cos(theta1 + theta2), elbow[1] + l2 * std::sin(theta1 + theta2)};

    if (show_animation)
    {
        plt::clf();

        // line: shoulder->elbow
        plt::plot({shoulder[0], elbow[0]}, {shoulder[1], elbow[1]}, "k-");

        // line: elbow->wrist
        plt::plot({elbow[0], wrist[0]}, {elbow[1], wrist[1]}, "k-");

        // point: shoulder, elbow, wrist
        plt::plot({shoulder[0]}, {shoulder[1]}, "ro");
        plt::plot({elbow[0]}, {elbow[1]}, "ro");
        plt::plot({wrist[0]}, {wrist[1]}, "ro");

        // line: wrist->target
        plt::plot({wrist[0], target_x}, {wrist[1], target_y}, "g--");

        // point: target
        plt::plot({target_x}, {target_y}, "g*");

        // coordinate limit
        plt::xlim(-3, 3);
        plt::ylim(-3, 3);

        plt::draw();
        plt::pause(dt);
    }

    std::vector<double> result = {wrist[0], wrist[1]};
    return result;
}

/**
 * @brief run two_joint_arm analytical inverse kinematic with global goal
 *
 * @param GOAL_TH
 * @param theta1
 * @param theta2
 * @return std::pair<double, double>
 */
std::pair<double, double> two_joint_arm(double GOAL_TH = 0.001, double theta1 = 0.0, double theta2 = 0.0)
{
    double goal_x_prev = 0.0, goal_y_prev = 0.0;
    double theta1_goal, theta2_goal;
    while (true)
    {
        try
        {
            // check endeffector goal pose x y
            if (check_goal_pose(goal_x, goal_y))
            {
                goal_x_prev = goal_x;
                goal_y_prev = goal_y;
            }
            else
            {
                std::cout << "goal_x: " << goal_x << "\n";
                std::cout << "goal_y: " << goal_y << "\n";
                std::cout << "skip invalid goal pose\n";
                return std::make_pair(0.0, 0.0);
            }

            // calc theta2_goal
            if (std::hypot(goal_x, goal_y) > (l1 + l2))
            {
                // if goal pose is out of arm workspace, theta2_goal will keep 0
                theta2_goal = 0.0;
            }
            else
            {
                // calc theta2_goal
                theta2_goal = std::acos((goal_x * goal_x + goal_y * goal_y - l1 * l1 - l2 * l2) / (2.0 * l1 * l2));
            }

            // calc theta1_goal
            double tmp = std::atan2(l2 * std::sin(theta2_goal), (l1 + l2 * std::cos(theta2_goal)));
            theta1_goal = std::atan2(goal_y, goal_x) - tmp;

            // keep theta1 >= 0
            if (theta1_goal < 0)
            {
                theta2_goal = -theta2_goal;
                tmp = std::atan2(l2 * std::sin(theta2_goal), (l1 + l2 * std::cos(theta2_goal)));
                theta1_goal = std::atan2(goal_y, goal_x) - tmp;
            }

            // get theta1 and theta2 in step with P-controller
            if (std::fabs(theta1 - theta1_goal) > GOAL_TH)
            {
                theta1 = theta1 + Kp * ang_diff(theta1_goal, theta1) * dt;
            }
            else
            {
                theta1 = theta1_goal;
            }
            if (std::fabs(theta2 - theta2_goal) > GOAL_TH)
            {
                theta2 = theta2 + Kp * ang_diff(theta2_goal, theta2) * dt;
            }
            else
            {
                theta2 = theta2_goal;
            }

            // std::cout << "theta1: " << theta1 << "\n";
            // std::cout << "theta2: " << theta1 << "\n";
        }
        catch (std::exception& e)
        {
            std::cerr << "Unreachable goal: " << e.what() << std::endl;
        }

        // plot arm and get endeffector pose
        std::vector<double> ee_pose = plot_arm(theta1, theta2, goal_x, goal_y);

        // check theta1 & theta2 reach goal
        if (std::fabs(theta1 - theta1_goal) < GOAL_TH && std::fabs(theta2 - theta2_goal) < GOAL_TH)
        {
            return std::make_pair(theta1_goal, theta2_goal);
        }
    }
}

/**
 * @brief main process of TwoJointArmToPointControl
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const* argv[])
{
    if (show_animation)
    {
        plt::ion();
    }

    plt::figure();

    for (size_t i = 0; i < goal_x_vec.size(); i++)
    {
        goal_x = goal_x_vec[i];
        goal_y = goal_y_vec[i];
        d2goal_prev = 100;

        two_joint_arm();
        usleep(sleep_time);
    }

    return 0;
}

/**
 * @file n_joint_arm_to_point_control.cpp
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief Inverse kinematics for an n-link arm using the Jacobian inverse method
 * @version 0.1
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Eigen/Core>
#include <Eigen/Dense>

#include "NLinkArm.h"

// simulation paramters
const int N_LINKS(5);
const double Kp(5.0);
const double dt(0.01);
const int N_ITERATIONS(10000);
const double dist_eps(0.01);
const double angle_eps(0.001);

// states
const int WAIT_FOR_NEW_GOAL(1);
const int MOVING_TO_GOAL(2);

// flag
const bool show_animation(true);

// target pos
std::vector<double> target_x = {
    N_LINKS,
    N_LINKS / 1.5,
    -N_LINKS / 3.0,
    -N_LINKS / 2.0,
    N_LINKS / 2.0,
    -N_LINKS / 1.9,
    N_LINKS / 2.3,
    -N_LINKS / 4.0,
    -N_LINKS / 1.3,
    N_LINKS / 1.8,
    N_LINKS / 3.2,
};
std::vector<double> target_y = {
    N_LINKS,
    N_LINKS / 1.8,
    N_LINKS / 2.3,
    N_LINKS / 4.0,
    -N_LINKS / 1.7,
    -N_LINKS / 3.0,
    N_LINKS / 2.6,
    -N_LINKS / 3.0,
    N_LINKS / 2.0,
    N_LINKS / 1.5,
    -N_LINKS / 1.3,
};

/**
 * @brief calc distance and errors to goal
 *
 * @param current_pos
 * @param goal_pos
 * @return errors
 * @return distance
 */
void distance_to_goal(
    const std::pair<double, double> current_pos,
    const std::pair<double, double> goal_pos,
    std::pair<double, double>& errors,
    double& distance)
{
    errors.first = goal_pos.first - current_pos.first;
    errors.second = goal_pos.second - current_pos.second;
    distance = std::hypot(errors.first, errors.second);
}

/**
 * @brief forward kinematics for n links arm
 *
 * @param link_lengths
 * @param joint_angles
 * @return std::pair<double, double>
 */
std::pair<double, double> forward_kinematics(
    const std::vector<double> link_lengths,
    const std::vector<double> joint_angles)
{
    std::pair<double, double> endeffector = {0, 0};

    for (size_t i = 1; i < link_lengths.size() + 1; i++)
    {
        endeffector.first +=
            link_lengths[i - 1] * cos(std::accumulate(joint_angles.begin(), joint_angles.begin() + i, 0.0));
        endeffector.second +=
            link_lengths[i - 1] * sin(std::accumulate(joint_angles.begin(), joint_angles.begin() + i, 0.0));
    }

    return endeffector;
}

/**
 * @brief Calc jacobian matrix for n-links arm
 *
 * @param link_lengths
 * @param joint_angles
 * @return Eigen::Matrix<double, 2, N_LINKS>
 */
Eigen::MatrixXd jacobian_inverse(const std::vector<double> link_lengths, const std::vector<double> joint_angles)
{
    // convert data into eigen vector
    Eigen::VectorXd link_lengths_eigen;
    link_lengths_eigen.resize(link_lengths.size());
    for (size_t i = 0; i < link_lengths.size(); i++)
    {
        link_lengths_eigen(i) = link_lengths[i];
    }
    Eigen::VectorXd joint_angles_eigen;
    joint_angles_eigen.resize(joint_angles.size());
    for (size_t i = 0; i < joint_angles.size(); i++)
    {
        joint_angles_eigen(i) = joint_angles[i];
    }

    // calc jacobian matrix for n-links arm
    Eigen::Matrix<double, 2, N_LINKS> J = Eigen::Matrix<double, 2, N_LINKS>::Zero();
    for (int i = 0; i < N_LINKS; i++)
    {
        for (int j = i; j < N_LINKS; j++)
        {
            J(0, i) -= link_lengths_eigen(j) * std::sin(joint_angles_eigen.head(j).sum());
            J(1, i) += link_lengths_eigen(j) * std::cos(joint_angles_eigen.head(j).sum());
        }
    }

    // calc pseudo inverse matrix for jacobi matrix
    Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXd> cod(J);
    Eigen::MatrixXd J_pinv = cod.pseudoInverse();

    return J_pinv;
}

/**
 * @brief inverse kinematics for n links arm
 * Calculates the inverse kinematics using the Jacobian inverse method.
 * @param link_lengths
 * @param joint_angles
 * @param goal
 * @param joint_goal_angles
 * @return solution_found
 */
void inverse_kinematics(
    const std::vector<double> link_lengths,
    std::vector<double> joint_angles,
    const std::pair<double, double> goal,
    std::vector<double>& joint_goal_angles,
    bool& solution_found)
{
    std::vector<double> joint_current_angles = joint_angles;

    for (size_t i = 0; i < N_ITERATIONS; i++)
    {
        // get current pos
        auto current_pos = forward_kinematics(link_lengths, joint_current_angles);
        std::pair<double, double> errors;
        double distance;
        distance_to_goal(current_pos, goal, errors, distance);

        // check solution found
        if (distance < dist_eps)
        {
            std::cout << "Solution found in " << i << ". iterations.\n";

            joint_goal_angles = joint_current_angles;
            for (int i = 0; i < joint_current_angles.size(); i++)
            {
                joint_goal_angles[i] = std::fmod(joint_goal_angles[i], (2 * M_PI));
            }
            solution_found = true;
            return;
        }

        // update joints using jacobian
        auto J_pinv = jacobian_inverse(link_lengths, joint_current_angles);

        Eigen::Vector2d e;
        e << errors.first, errors.second;
        // std::cout << "e: \n" << e << "\n";

        auto joint_angle_update = J_pinv * e;
        // std::cout << "joint_angle_update: \n" << joint_angle_update << "\n";

        for (size_t j = 0; j < joint_current_angles.size(); j++)
        {
            joint_current_angles[j] += joint_angle_update(j, 0);
        }
    }

    joint_goal_angles = joint_current_angles;
    solution_found = false;
    return;
}

/**
 * @brief Returns the difference between two angles in the range -pi to +pi
 *
 * @param theta1
 * @param theta2
 * @return double
 */
double GetAngDiff(double theta1, double theta2)
{
    return std::fmod(theta1 - theta2 + M_PI, 2 * M_PI) - M_PI;
}

/**
 * @brief Main process
 * Creates an arm using the NLinkArm class and uses its inverse kinematics
 * to move it to the desired position.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const* argv[])
{
    // create NLinkArm object
    std::vector<double> link_lengths;
    std::vector<double> joint_angles;
    for (size_t i = 0; i < N_LINKS; i++)
    {
        link_lengths.push_back(1 - 0.1 * i);
        joint_angles.push_back(0);
    }
    std::pair<double, double> goal(N_LINKS, 0);
    NLinkArm arm(link_lengths, joint_angles, goal, show_animation);

    // init state
    int state = WAIT_FOR_NEW_GOAL;
    int state_second_inv = 0;
    bool solution_found = false;

    // init joint_goal_angles
    std::vector<double> joint_goal_angles;

    // init goal
    int target_pose_index = 0;
    int target_pose_len = target_x.size();

    while (true)
    {
        // update goal and endeffector
        auto goal_old = goal;
        goal = arm.goal_;
        auto endeffector = arm.endeffector_;

        // calc dist to goal
        std::pair<double, double> errors;
        double distance;
        distance_to_goal(endeffector, goal, errors, distance);

        if (state == WAIT_FOR_NEW_GOAL)
        {
            std::cout << "state: wait for new goal.\n";
            usleep(2000000);

            // update new goal
            if (target_pose_index >= target_pose_len)
            {
                std::cout << "finished\n";
                return 0;
            }
            arm.goal_.first = target_x[target_pose_index];
            arm.goal_.second = target_y[target_pose_index];
            target_pose_index++;
            goal_old = goal;
            goal = arm.goal_;

            // calc dist to goal
            distance_to_goal(endeffector, goal, errors, distance);

            // calc inverse kinematics
            if (distance > dist_eps && !solution_found)
            {
                std::cout << "run inverse\n";
                usleep(2000000);

                inverse_kinematics(link_lengths, joint_angles, goal, joint_goal_angles, solution_found);

                // check inverse kinematic results
                if (!solution_found)
                {
                    std::cout << "Solution could not be found.\n";
                    state = WAIT_FOR_NEW_GOAL;
                    state_second_inv = 0;
                    arm.goal_ = endeffector;
                }
                else
                {
                    std::cout << "Solution is found.\n";
                    state = MOVING_TO_GOAL;
                }
            }
        }
        else if (state == MOVING_TO_GOAL)
        {

            // check all angle diff < angle_eps
            bool stop_flag = true;
            for (size_t i = 0; i < joint_angles.size(); i++)
            {
                double ang_diff = GetAngDiff(joint_goal_angles[i], joint_angles[i]);
                if (fabs(ang_diff) > angle_eps)
                {
                    stop_flag = (stop_flag && false);
                }
                else
                {
                    stop_flag = (stop_flag && true);
                }
                std::cout << "ang_diff: " << ang_diff << "\n";

                // update joint angles
                joint_angles[i] = joint_angles[i] + Kp * dt * ang_diff;
            }

            if (stop_flag == true)
            {
                std::cout << "stop moving\n";
                state = WAIT_FOR_NEW_GOAL;
                solution_found = false;
            }
        }

        arm.update_joints(joint_angles);
    }

    return 0;
}

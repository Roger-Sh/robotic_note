/**
 * @file random_forward_kinematics_3d.cpp
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief Demo for random forward kinematics of a N-link arm in 3d space
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <random>

#include "NLinkArm3d.hpp"

/**
 * @brief main process of random forward kinematics of a N-Link arm in 3d space
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const* argv[])
{
    // create dh params for arm
    // theta for active joints, rotation at z axis
    // alpha, rotation at x axis
    // a, trans along x
    // d, trans along z
    std::vector<Eigen::Vector4d> dh_params_list = {
        {0.0,        -M_PI / 2.0, 0.1, 0.0},
        {M_PI / 2.0, M_PI / 2.0,  0.1, 0.0},
        {0.0,        -M_PI / 2.0, 0.0, 0.4},
        {0.0,        M_PI / 2.0,  0.1, 0.0},
        {0.0,        -M_PI / 2.0, 0.0, 0.3},
        {0.0,        M_PI / 2.0,  0.1, 0.0},
        {0.0,        0.0,         0.1, 0.0},
    };
    // create NLinkArm3d
    NLinkArm3d n_link_arm(dh_params_list);

    // generate random joints configurations
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.5, 0.5);

    // run forward_kinematic 10 times with random joints
    for (size_t i = 0; i < 10; i++)
    {
        // generate random joint_angles
        Eigen::VectorXd joint_angles(n_link_arm.link_list_.size());
        for (size_t j = 0; j < joint_angles.size(); j++)
        {
            joint_angles(j) = dis(gen);
        }

        n_link_arm.set_joint_angles(joint_angles);

        n_link_arm.forward_kinematics(true);
    }

    return 0;
}

/**
 * @file random_inverse_kinematics_3d.cpp
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief Demo for random inverse kinematics of a N-link arm in 3d space
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <random>

#include "NLinkArm3d.hpp"

/**
 * @brief main process of random inverse kinematics of a N-link arm in 3d space
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

    // run random inverse kinematic 10 times
    for (size_t i = 0; i < 10; i++)
    {
        // generate random ee pos
        Eigen::VectorXd target_ee_pos(6);
        for (size_t j = 0; j < 6; j++)
        {
            target_ee_pos(j) = dis(gen);
            if (j < 2)
            {
                if (target_ee_pos(j) < 0 && target_ee_pos(j) > -0.2)
                {
                    target_ee_pos(j) = -0.2;
                }
                else if (target_ee_pos(j) > 0 && target_ee_pos(j) < 0.2)
                {
                    target_ee_pos(j) = 0.2;
                }
            }
        }

        // inverse kinematic with plot
        n_link_arm.inverse_kinematics(target_ee_pos, true);
    }

    return 0;
}

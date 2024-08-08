/**
 * @file NLinkArm.h
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief NLinkArm head file
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <math.h>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

/**
 * @brief class NLinkArm
 *
 */
class NLinkArm
{
public:
    NLinkArm(
        std::vector<double> link_length,
        std::vector<double> joint_angles,
        std::pair<double, double> goal,
        bool show_animation);
    ~NLinkArm();

    void update_joints(std::vector<double> joint_angles);
    void update_points();
    void plot();

public:
    int n_links_;
    std::vector<double> link_lengths_;
    std::vector<double> joint_angles_;
    std::vector<std::pair<double, double>> points_;
    std::pair<double, double> endeffector_;
    bool show_animation_;
    double lim_;
    std::pair<double, double> goal_;
    std::pair<double, double> goal_click_;
};
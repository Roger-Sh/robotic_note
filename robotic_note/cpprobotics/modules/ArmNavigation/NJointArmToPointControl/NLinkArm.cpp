/**
 * @file NLinkArm.cpp
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief Implementation of NLinkArm
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "NLinkArm.h"

/**
 * @brief Construct a new NLinkArm::NLinkArm object
 *
 * @param link_lengths
 * @param joint_angles
 * @param goal
 * @param show_animation
 */
NLinkArm::NLinkArm(
    std::vector<double> link_lengths,
    std::vector<double> joint_angles,
    std::pair<double, double> goal,
    bool show_animation)
{
    // check links and joints size
    if (link_lengths.size() != joint_angles.size())
    {
        throw std::invalid_argument("bad link_length and joint_angles size");
    }

    // init parameters
    this->link_lengths_ = link_lengths;
    this->joint_angles_ = joint_angles;
    this->goal_ = goal;
    this->goal_click_ = this->goal_;
    this->show_animation_ = show_animation;
    this->n_links_ = link_lengths.size();

    // init points_
    this->points_.resize(this->n_links_ + 1);
    for (size_t i = 0; i < this->points_.size(); i++)
    {
        this->points_[i] = std::make_pair(0, 0);
    }

    // get lim_
    this->lim_ = std::accumulate(this->link_lengths_.begin(), this->link_lengths_.end(), 0.0);

    // show animation
    if (this->show_animation_)
    {
        plt::ion();
        plt::figure();
        plt::show();
    }

    update_points();
}

/**
 * @brief Destroy the NLinkArm::NLinkArm object
 *
 */
NLinkArm::~NLinkArm() {}

/**
 * @brief update joints
 *
 * @param joint_angles
 */
void NLinkArm::update_joints(std::vector<double> joint_angles)
{
    std::cout << "update_joints\n";

    this->joint_angles_ = joint_angles;
    update_points();
}

/**
 * @brief update link points
 *
 */
void NLinkArm::update_points()
{
    // calculate arm point x y based on previous point
    for (int i = 1; i < this->n_links_ + 1; i++)
    {
        this->points_[i].first =
            this->points_[i - 1].first +
            this->link_lengths_[i - 1] *
                cos(std::accumulate(this->joint_angles_.begin(), this->joint_angles_.begin() + i, 0.0));
        this->points_[i].second =
            this->points_[i - 1].second +
            this->link_lengths_[i - 1] *
                sin(std::accumulate(this->joint_angles_.begin(), this->joint_angles_.begin() + i, 0.0));
    }

    // get endeffector
    this->endeffector_ = this->points_[this->n_links_];
    if (this->show_animation_)
    {
        plot();
    }
}

/**
 * @brief plot arm
 *
 */
void NLinkArm::plot()
{
    plt::cla();

    // draw link
    for (int i = 0; i < this->n_links_ + 1; i++)
    {
        if (i != this->n_links_)
        {
            plt::plot(
                {this->points_[i].first, this->points_[i + 1].first},
                {this->points_[i].second, this->points_[i + 1].second},
                "r-");
        }
        plt::plot({this->points_[i].first}, {this->points_[i].second}, "ko");
    }

    // draw goal point
    plt::plot({this->goal_.first}, {this->goal_.second}, "gx");

    // draw endeffector to goal
    plt::plot({this->endeffector_.first, this->goal_.first}, {this->endeffector_.second, this->goal_.second}, "g--");

    plt::xlim(-this->lim_, this->lim_);
    plt::ylim(-this->lim_, this->lim_);
    plt::draw();
    plt::pause(0.01);
}
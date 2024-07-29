/**
 * @file NLinkArm3d.h
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief header of NLinkArm3d
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef N_LINK_ARM_3D_H
#define N_LINK_ARM_3D_H

#include <matplot/matplot.h>
#include <unistd.h>

#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <iostream>

// Global paramters
const int INV_KINEMATIC_ITERATION(100);

/**
 * @brief Class Link
 * This class is meant to provide DH-params, transformation matrix and basic jacobian matrix of an arm link
 */
class Link
{
public:
    // DH parameters of this link
    // 0: theta for active joints, rotation at z axis
    // 1: alpha, rotation at x axis
    // 2: a, trans along x
    // 3: d, trans along z
    Eigen::Vector4d dh_params_;

public:
    /**
     * @brief Construct a new Link object
     *
     * @param dh_params
     */
    Link(Eigen::Vector4d dh_params)
    {
        this->dh_params_ = dh_params;
    }

    /**
     * @brief Destroy the Link object
     *
     */
    ~Link() {}

    /**
     * @brief return transformation matrix of this link, based on SDH
     *
     * @return Eigen::Matrix4d
     */
    Eigen::Matrix4d tranformation_matrix()
    {
        double theta = this->dh_params_[0];
        double alpha = this->dh_params_[1];
        double a = this->dh_params_[2];
        double d = this->dh_params_[3];
        double st = std::sin(theta);
        double ct = std::cos(theta);
        double sa = std::sin(alpha);
        double ca = std::cos(alpha);
        Eigen::Matrix4d trans;

        // SDH， RzTzRxTx
        trans << ct, -st * ca, st * sa, a * ct,   //
            st, ct * ca, -ct * sa, a * st,        //
            0, sa, ca, d,                         //
            0, 0, 0, 1;                           //

        return trans;
    }

    /**
     * @brief Calc basic jacobian of this link
     * Basic jacobian is the components of the geometric jacobian matrix.
     * here the basic jacobian is follow the rule of rotation joint
     * @param trans_prev
     * @param ee_pos
     * @return Eigen::MatrixXd
     */
    Eigen::MatrixXd basic_jacobian(const Eigen::Matrix4d& trans_prev, const Eigen::Vector3d& ee_pos)
    {
        Eigen::Vector3d pos_prev;
        pos_prev << trans_prev(0, 3), trans_prev(1, 3), trans_prev(2, 3);
        Eigen::Vector3d z_axis_prev;
        z_axis_prev << trans_prev(0, 2), trans_prev(1, 2), trans_prev(2, 2);

        // j_trans_i
        Eigen::Vector3d cross_product = z_axis_prev.cross(ee_pos - pos_prev);

        // Construct the Jacobian matrix by horizontally stacking the cross product of two pose and z_axis_prev
        Eigen::MatrixXd basic_jacobian(1, 6);
        basic_jacobian << cross_product.transpose(), z_axis_prev.transpose();

        return basic_jacobian.transpose();
    }
};

/**
 * @brief Class NLinkArm3d
 *
 */
class NLinkArm3d
{
public:
    std::vector<Link> link_list_;

public:
    /**
     * @brief Construct a new NLinkArm3d object
     *
     * @param dh_params_list
     */
    NLinkArm3d(const std::vector<Eigen::Vector4d> dh_params_list)
    {
        // init link_list_
        for (size_t i = 0; i < dh_params_list.size(); i++)
        {
            this->link_list_.push_back(Link(dh_params_list[i]));
        }
    }

    /**
     * @brief Destroy the NLinkArm3d object
     *
     */
    ~NLinkArm3d() {}

    /**
     * @brief get transformation matrix from base to endeffector
     *
     * @return Eigen::Matrix4d
     */
    Eigen::Matrix4d transformation_matrix()
    {
        Eigen::Matrix4d trans = Eigen::Matrix4d::Identity();

        for (size_t i = 0; i < this->link_list_.size(); i++)
        {
            trans = trans * this->link_list_[i].tranformation_matrix();
        }

        return trans;
    }

    /**
     * @brief get euler angle from transformation matrix
     *
     * @return alpha
     * @return beta
     * @return gamma
     */
    void euler_angle(double& alpha, double& beta, double& gamma)
    {
        Eigen::Matrix4d trans = this->transformation_matrix();

        alpha = std::atan2(trans(1, 2), trans(0, 2));
        if (!(alpha >= -M_PI / 2 && alpha <= M_PI / 2))
        {
            alpha = alpha + M_PI;
        }
        if (!(alpha >= -M_PI / 2 && alpha <= M_PI / 2))
        {
            alpha = alpha - M_PI;
        }

        beta = std::atan2(sqrt(trans(0, 2) * trans(0, 2) + trans(1, 2) * trans(1, 2)), trans(2, 2));
        if (!(beta >= -M_PI / 2 && beta <= M_PI / 2))
        {
            beta = beta + M_PI;
        }
        if (!(beta >= -M_PI / 2 && beta <= M_PI / 2))
        {
            beta = beta - M_PI;
        }

        gamma = std::atan2(trans(2, 1), -trans(2, 0));
        if (!(gamma >= -M_PI / 2 && beta <= M_PI / 2))
        {
            gamma = gamma + M_PI;
        }
        if (!(gamma >= -M_PI / 2 && beta <= M_PI / 2))
        {
            gamma = gamma - M_PI;
        }

        // beta = std::atan2(trans(0, 2) * std::cos(alpha) + trans(1, 2) * std::sin(alpha), trans(2, 2));
        // gamma = std::atan2(
        //     -trans(0, 0) * std::sin(alpha) + trans(1, 0) * std::cos(alpha),
        //     -trans(0, 1) * std::sin(alpha) + trans(1, 1) * std::cos(alpha));
    
    }

    /**
     * @brief calc transformation matrix using forward kinematic, and plot arm
     *
     * @param plot
     * @return Eigen::VectorXd
     */
    Eigen::VectorXd forward_kinematics(bool plot = false)
    {
        Eigen::MatrixX4d trans = this->transformation_matrix();

        double x = trans(0, 3);
        double y = trans(1, 3);
        double z = trans(2, 3);
        double alpha, beta, gamma;
        this->euler_angle(alpha, beta, gamma);

        if (plot)
        {
            std::cout << "before plot arm\n";
            plot_arm(Eigen::Matrix<double, 1, 6>().setZero(), true);
            std::cout << "after plot arm\n";
        }

        Eigen::VectorXd ee_pos(6);
        ee_pos << x, y, z, alpha, beta, gamma;
        return ee_pos;
    }

    /**
     * @brief calc basic jacobian
     *
     * @return Eigen::MatrixXd
     */
    Eigen::MatrixXd basic_jacobian()
    {
        Eigen::Vector3d ee_pos = this->forward_kinematics().segment(0, 3);
        Eigen::MatrixXd basic_jacobian_mat(6, this->link_list_.size());
        Eigen::Matrix4d trans = Eigen::Matrix4d::Identity();

        for (size_t i = 0; i < this->link_list_.size(); i++)
        {
            auto basic_jacobian_part = this->link_list_[i].basic_jacobian(trans, ee_pos);
            basic_jacobian_mat.block(0, i, basic_jacobian_part.rows(), basic_jacobian_part.cols()) =
                basic_jacobian_part;
            trans = trans * this->link_list_[i].tranformation_matrix();
        }

        return basic_jacobian_mat;
    }

    /**
     * @brief run inverse kinematics of a N-link arm
     *
     * @param ref_ee_pose
     * @param plot
     */
    void inverse_kinematics(const Eigen::VectorXd ref_ee_pose, bool plot = false)
    {
        double alpha_prev, beta_prev, gamma_prev;
        this->euler_angle(alpha_prev, beta_prev, gamma_prev);
        for (size_t i = 0; i < INV_KINEMATIC_ITERATION; i++)
        {
            std::cout << "iteration: " << i << "\n";

            Eigen::VectorXd ee_pos = this->forward_kinematics();
            Eigen::VectorXd diff_pos = ref_ee_pose - ee_pos;
            Eigen::MatrixXd basic_jacobian_mat = this->basic_jacobian();

            // here the euler angle is intrinsic zyz
            double alpha, beta, gamma;
            this->euler_angle(alpha, beta, gamma);

            // use euler angle zyz
            Eigen::Matrix3d K_zyz;
            K_zyz << 0, -sin(alpha), cos(alpha) * sin(beta), 0, cos(alpha), sin(alpha) * sin(beta), 1, 0, cos(beta);

            // K_alpha is used to convert analytic jacobian matrix to geometric jacobian matrix
            Eigen::MatrixXd K_alpha = Eigen::MatrixXd::Identity(6, 6);
            K_alpha.block(3, 3, 3, 3) = K_zyz;

            // get jacobian pseudo inverse matrix
            Eigen::MatrixXd basic_jacobian_mat_pinv =
                basic_jacobian_mat.completeOrthogonalDecomposition().pseudoInverse();

            // here K_alpha is no need to inverted twice, save a step
            Eigen::MatrixXd theta_dot = (basic_jacobian_mat_pinv * K_alpha) * diff_pos;

            this->update_joint_angles(theta_dot / 25.0);

            if (plot)
            {
                if (i == INV_KINEMATIC_ITERATION - 1)
                {
                    plot_arm(ref_ee_pose, true);
                }
                else
                {
                    plot_arm(ref_ee_pose, false);
                }
            }
        }
    }

    /**
     * @brief set joint angles with joint angle list
     *
     * @param joint_angle_list
     */
    void set_joint_angles(const Eigen::VectorXd joint_angle_list)
    {
        assert(joint_angle_list.size() == this->link_list_.size());

        for (size_t i = 0; i < joint_angle_list.size(); i++)
        {
            this->link_list_[i].dh_params_(0) = joint_angle_list[i];
        }
    }

    /**
     * @brief update joint angles
     *
     * @param diff_joint_angle_list
     */
    void update_joint_angles(const Eigen::VectorXd diff_joint_angle_list)
    {
        assert(diff_joint_angle_list.size() == this->link_list_.size());

        std::cout << "update_joint_angles: \n";
        for (size_t i = 0; i < diff_joint_angle_list.size(); i++)
        {
            this->link_list_[i].dh_params_(0) += diff_joint_angle_list[i];
        }
    }

    /**
     * @brief plot arm
     *
     * @param ref_ee_pose: if ref_ee_pose is not zero, plot line from ee to ref_ee_pose
     */
    void plot_arm(
        Eigen::Matrix<double, 1, 6> ref_ee_pose = Eigen::Matrix<double, 1, 6>().setZero(),
        const bool pause_flag = false)
    {
        // init points
        std::vector<double> x_list;
        std::vector<double> y_list;
        std::vector<double> z_list;

        // set base point
        Eigen::Matrix4d trans = Eigen::Matrix4d::Identity();
        x_list.push_back(trans(0, 3));
        y_list.push_back(trans(1, 3));
        z_list.push_back(trans(2, 3));

        // set links point
        for (auto& link : this->link_list_)
        {
            // calc joint pose
            trans = trans * link.tranformation_matrix();
            x_list.push_back(trans(0, 3));
            y_list.push_back(trans(1, 3));
            z_list.push_back(trans(2, 3));
        }

        // draw joints point
        auto fig = matplot::plot3(x_list, y_list, z_list, "-o");
        fig->line_width(2);
        std::vector<size_t> joint_indices;
        for (size_t i = 0; i < x_list.size(); i++)
        {
            joint_indices.push_back(i);
        }
        fig->marker_indices(joint_indices).marker_color("blue").marker_size(10).marker_style(".");

        if (fabs(ref_ee_pose(0)) > 0 || fabs(ref_ee_pose(1)) > 0 || fabs(ref_ee_pose(2)) > 0)
        {
            matplot::hold(matplot::on);
            std::vector<double> ref_ee_pose_x = {x_list[x_list.size() - 1], ref_ee_pose[0]};
            std::vector<double> ref_ee_pose_y = {y_list[x_list.size() - 1], ref_ee_pose[1]};
            std::vector<double> ref_ee_pose_z = {z_list[x_list.size() - 1], ref_ee_pose[2]};

            auto fig2 = matplot::plot3(ref_ee_pose_x, ref_ee_pose_y, ref_ee_pose_z, "-o");
            fig2->line_width(0);
            fig2->marker_indices({ref_ee_pose_x.size() - 1}).marker_color("red").marker_size(10).marker_style(".");
            matplot::hold(matplot::off);
        }

        matplot::xlabel("x");
        matplot::ylabel("y");
        matplot::zlabel("z");

        matplot::xlim({-0.8, 0.8});
        matplot::ylim({-0.8, 0.8});
        matplot::zlim({-0.8, 0.8});
        matplot::grid(matplot::on);

        if (pause_flag)
        {
            matplot::show();
        }
        else
        {
            matplot::gcf()->draw();
            usleep(50000);
        }
    }
};

#endif   // N_LINK_ARM_3D_H
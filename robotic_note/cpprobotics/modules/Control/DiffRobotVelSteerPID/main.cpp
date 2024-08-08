#include <algorithm>

#include "DiffKinematicModel.h"
#include "PIDController.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

#define PI 3.1415926

/**
 * @brief 计算距离参考轨迹最近点的下标
 *
 * @param robot_state 机器人状态（x, y）
 * @param refer_path 参考路径
 * @return 距离参考轨迹最近点的下标
 */
int calTargetIndex(vector<double> robot_state, vector<vector<double>> refer_path)
{
    vector<double> dists;
    for (vector<double> xy : refer_path)
    {
        double dist = sqrt(pow(xy[0] - robot_state[0], 2) + pow(xy[1] - robot_state[1], 2));
        dists.push_back(dist);
    }
    int min_id      = min_element(dists.begin(), dists.end()) - dists.begin();   // 返回vector最小元素的下标
    double min_dist = dists[min_id];
    if (min_dist < 2.0)
    {
        min_id += 20;
    }
    if (min_id >= refer_path.size())
    {
        min_id = refer_path.size() - 1;
    }
    return min_id;
}

int main()
{
    // 生成参考轨迹
    vector<vector<double>> refer_path(1000, vector<double>(2));
    vector<double> refer_x, refer_y;
    for (int i = 0; i < 1000; i++)
    {
        refer_path[i][0] = 0.1 * i;
        refer_path[i][1] = sin(refer_path[i][0] / 10.0);
        refer_x.push_back(refer_path[i][0]);
        refer_y.push_back(refer_path[i][1]);
    }

    // 运动学模型
    DiffKinematicModel ugv(0, -1, 0.5);

    // PID控制器
    PIDController distance_PID(0.8, 0.0, 0.01, 1.0, -1.0);
    PIDController theta_PID(0.8, 0.0, 0.01, 1.0, -1.0);

    // 保存机器人（小车）运动过程中的轨迹
    vector<double> x_, y_;

    // 机器人状态
    vector<double> robot_state(2);
    robot_state[0] = ugv.m_x;
    robot_state[1] = ugv.m_y;

    // 迭代
    int len      = refer_path.size() - 1;
    double error = sqrt(pow(refer_path[len][0] - robot_state[0], 2) + pow(refer_path[len][1] - robot_state[1], 2));
    while (error > 0.1)
    {
        plt::clf();

        robot_state[0] = ugv.m_x;
        robot_state[1] = ugv.m_y;

        // 计算前向距离和角度误差
        int min_ind    = calTargetIndex(robot_state, refer_path);
        double alpha   = atan2(refer_path[min_ind][1] - robot_state[1], refer_path[min_ind][0] - robot_state[0]);
        double l_e     = sqrt(pow(refer_path[min_ind][0] - robot_state[0], 2) + pow(refer_path[min_ind][1] - robot_state[1], 2));
        double theta_e = alpha - ugv.m_psi;

        // 角度误差归一化到-pi~pi
        if (theta_e > M_PI)
        {
            theta_e -= 2 * M_PI;
        }
        else if (theta_e < -M_PI)
        {
            theta_e += 2 * M_PI;
        }

        // 计算控制量
        double linear_x  = distance_PID.calOutput(l_e);    // 纵向控制
        double angular_z = theta_PID.calOutput(theta_e);   // 横向控制

        // 更新机器人状态
        ugv.updateState(linear_x, angular_z, 0.1);
        x_.push_back(ugv.m_x);
        y_.push_back(ugv.m_y);
        vector<double> target_x_, target_y_;
        target_x_.push_back(refer_path[min_ind][0]);
        target_y_.push_back(refer_path[min_ind][1]);

        error = sqrt(pow(refer_path[len][0] - robot_state[0], 2) + pow(refer_path[len][1] - robot_state[1], 2));

        // 画图
        plt::plot(refer_x, refer_y, "b--");
        plt::plot(x_, y_, "r");
        plt::plot(target_x_, target_y_, "go");
        plt::grid(true);
        plt::ylim(-2.5, 2.5);
        plt::pause(0.01);
    }

    // save figure
    // const char* filename = "./pid_demo.png";
    // cout << "Saving result to " << filename << std::endl;
    // plt::save(filename);
    plt::show();

    return 0;
}

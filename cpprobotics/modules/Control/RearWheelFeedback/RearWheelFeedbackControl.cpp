#include "KinematicModel.h"
#include "MyReferencePath.h"
#include "RearWheelFeedback.h"
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#define PI 3.1415926

int main()
{
    double Kpsi = 4.5, K2 = 2.0;   // 利亚普诺夫系数
    double dt    = 0.1;          // 时间间隔，单位：s
    double L     = 2;            // 车辆轴距，单W位：m
    double v     = 2;            // 初始速度
    double x_0   = 0;            // 初始x
    double y_0   = 0;           // 初始y
    double psi_0 = 0;            // 初始航向角

    vector<double> x_, y_;
    MyReferencePath referencePath;
    KinematicModel ugv(x_0, y_0, psi_0, v, L, dt);
    RearWheelFeedback rwf(Kpsi, K2, L);
    vector<double> robot_state;

    for (int i = 0; i < 600; i++)
    {
        plt::clf();
        robot_state              = ugv.getState();
        vector<double> one_trial = referencePath.calcTrackError(robot_state);
        double e = one_trial[0], k = one_trial[1], ref_psi = one_trial[2], index_ = one_trial[3];
        double delta = rwf.rearWheelFeedbackControl(robot_state, e, k, ref_psi);

        ugv.updateState(0, delta);
        x_.push_back(ugv.x);
        y_.push_back(ugv.y);

        plt::plot(referencePath.refer_x, referencePath.refer_y, "b--");
        vector<double> cur_x, cur_y;
        cur_x.push_back(referencePath.refer_x[index_]);
        cur_y.push_back(referencePath.refer_y[index_]);
        plt::plot(cur_x, cur_y, "go");
        plt::grid(true);
        plt::ylim(-5, 5);
        plt::plot(x_, y_, "r");
        plt::pause(0.001);
    }
    plt::show();
    plt::detail::_interpreter::kill();   // in case the segfault when exits
    return 0;
}

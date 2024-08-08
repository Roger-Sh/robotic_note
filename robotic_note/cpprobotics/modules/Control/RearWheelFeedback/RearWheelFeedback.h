#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#define PI 3.1415926

class RearWheelFeedback
{
private:
    double Kpsi, K2, L;   // 利亚普诺夫相关系数，最后一个为机器人轴距
public:
    RearWheelFeedback(double Kpsi, double K2, double L);
    static double normalizeAngle(double angle);
    double rearWheelFeedbackControl(vector<double> robot_state, double e, double k, double ref_psi);
};

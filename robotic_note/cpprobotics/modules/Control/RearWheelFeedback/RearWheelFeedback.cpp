#include "RearWheelFeedback.h"

double RearWheelFeedback::normalizeAngle(double angle)
{
    while (angle > PI)
    {
        angle -= 2.0 * PI;
    }
    while (angle < -PI)
    {
        angle += 2.0 * PI;
    }
    return angle;
}

double RearWheelFeedback::rearWheelFeedbackControl(vector<double> robot_state, double e, double k, double ref_psi)
{
    double psi = robot_state[2], v = robot_state[3];
    double psi_e = normalizeAngle(psi - ref_psi);   // psi_e=yaw-ref_yaw
    double psi_dot = v * k * cos(psi_e) / (1.0 - k * e) - K2 * v * sin(psi_e) * e / psi_e - Kpsi * abs(v) * psi_e;
    if (psi_e == 0.0 || psi_dot == 0.0)
        return 0.0;
    double delta = atan2(L * psi_dot, v);
    return delta;
}

RearWheelFeedback::RearWheelFeedback(double Kpsi, double K2, double L)
{
    this->Kpsi = Kpsi;
    this->K2   = K2;
    this->L    = L;
}

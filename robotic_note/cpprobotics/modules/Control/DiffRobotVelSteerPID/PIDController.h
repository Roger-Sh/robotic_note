#pragma once

#include <iostream>

using namespace std;

/**
 * @brief 位置式PID实现
 */
class PIDController
{
private:
    double m_kp;
    double m_ki;
    double m_kd;
    double m_upper;
    double m_lower;
    double m_error;
    double m_pre_error;
    double m_sum_error;

public:
    PIDController(double kp, double ki, double kd, double upper, double lower);

    void setK(double kp, double ki, double kd);

    void setBound(double upper, double lower);

    double calOutput(double error);

    void reset();

    void setSumError(double sum_error);
};

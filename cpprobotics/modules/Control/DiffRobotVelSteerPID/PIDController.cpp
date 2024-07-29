#include "PIDController.h"


/**
 * @brief 构造函数
 *
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数
 * @param target 目标值
 * @param upper 上界
 * @param lower 下界
 * @return 无
 */
PIDController::PIDController(double kp, double ki, double kd, double upper, double lower) :
    m_kp(kp),
    m_ki(ki),
    m_kd(kd),
    m_upper(upper),
    m_lower(lower)
{
}


/**
 * @brief 设置PID控制参数
 *
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数
 * @return void 无
 */
void PIDController::setK(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}


/**
 * @brief 设置上下界
 *
 * @param upper 上界
 * @param lower 下界
 * @return 无
 */
void PIDController::setBound(double upper, double lower)
{
    m_upper = upper;
    m_lower = lower;
}


/**
 * @brief 计算输出
 *
 * @param state 状态量
 * @return 控制量
 */
double PIDController::calOutput(double error)
{
    m_error  = error;
    double u = m_error * m_kp + m_sum_error * m_ki + (m_error - m_pre_error) * m_kd;
    if (u < m_lower)
    {
        u = m_lower;
    }
    else if (u > m_upper)
    {
        u = m_upper;
    }
    m_pre_error = m_error;
    m_sum_error = m_sum_error + m_error;

    return u;
}


/**
 * @brief 重置PID控制器
 *
 * @return 无
 */
void PIDController::reset()
{
    m_error     = 0.0;
    m_pre_error = 0.0;
    m_sum_error = 0.0;
}


/**
 * @brief 设置积分项
 *
 * @param sum_error 积分项
 * @return 无
 */
void PIDController::setSumError(double sum_error)
{
    m_sum_error = sum_error;
}

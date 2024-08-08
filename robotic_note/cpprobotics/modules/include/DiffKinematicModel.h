#pragma once

#include <cmath>
#include <iostream>

using namespace std;

/**
 * @brief 两轮差分模型
 */
class DiffKinematicModel
{
public:
    double m_x;
    double m_y;
    double m_psi;   // rad

public:
    DiffKinematicModel() = default;

    DiffKinematicModel(double x, double y, double psi)
    {
        m_x   = x;
        m_y   = y;
        m_psi = psi;
    }

    void updateState(double linear_vel_x, double angular_vel_z, double dt)
    {
        m_x   = m_x + linear_vel_x * cos(m_psi) * dt;
        m_y   = m_y + linear_vel_x * sin(m_psi) * dt;
        m_psi = m_psi + angular_vel_z * dt;
    }
};
#include <Eigen/Dense>
#include <cmath>
#include <iostream>

double getDistance(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double getAngle(double x1, double y1, double x2, double y2)
{
    return std::atan2(y2 - y1, x2 - x1);
}

class PID
{
public:
    PID(double kp_linear  = 0.1,
        double kd_linear  = 0.1,
        double ki_linear  = 0,
        double kp_angular = 0.1,
        double kd_angular = 0.1,
        double ki_angular = 0) :
        kp_linear(kp_linear),
        kd_linear(kd_linear),
        ki_linear(ki_linear),
        kp_angular(kp_angular),
        kd_angular(kd_angular),
        ki_angular(ki_angular),
        prev_error_position(0),
        prev_error_angle(0),
        prev_x(0),
        prev_y(0),
        prev_body_to_goal(0),
        prev_waypoint_idx(-1)
    {
    }

    std::pair<double, double> getControlInputs(Eigen::Vector3d x, Eigen::Vector2d goal_x, int waypoint_idx)
    {
        double error_position = getDistance(x(0), x(1), goal_x(0), goal_x(1));

        double body_to_goal = getAngle(x(0), x(1), goal_x(0), goal_x(1));

        double error_angle = (-body_to_goal) - x(2);

        double linear_velocity_control  = kp_linear * error_position + kd_linear * (error_position - prev_error_position);
        double angular_velocity_control = kp_angular * error_angle + kd_angular * (error_angle - prev_error_angle);

        prev_error_angle    = error_angle;
        prev_error_position = error_position;

        prev_waypoint_idx = waypoint_idx;
        prev_body_to_goal = body_to_goal;

        prev_x = x(0);
        prev_y = x(1);

        if (linear_velocity_control > 5)
        {
            linear_velocity_control = 5;
        }

        return std::make_pair(linear_velocity_control, angular_velocity_control);
    }

private:
    double kp_linear, kd_linear, ki_linear;
    double kp_angular, kd_angular, ki_angular;
    double prev_error_position, prev_error_angle;
    double prev_x, prev_y;
    double prev_body_to_goal;
    int prev_waypoint_idx;
};
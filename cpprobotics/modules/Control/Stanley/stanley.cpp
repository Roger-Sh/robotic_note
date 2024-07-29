#include "matplotlibcpp.h"
#include "spline.h"

namespace plt = matplotlibcpp;
using namespace std;

// Vehicle parameters
const double L            = 2.9;          // Wheel base of vehicle in meters
const double max_steer    = M_PI / 6.0;   // Maximum steering angle
const double Kp           = 1.0;          // Proportional gain for speed control
const double k            = 0.5;          // Control gain for Stanley control
const double dt           = 0.1;          // Time step for simulation in seconds
const double target_speed = 30.0 / 3.6;   // Target speed in m/s

// function calc_spline_course to generate a spline
std::tuple<std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>, double> calc_spline_course(
    const std::vector<double>& x,
    const std::vector<double>& y,
    double ds)
{
    // Compute the total length of the path
    double s = 0.0;
    std::vector<double> s_vec;
    s_vec.push_back(0);
    for (int i = 0; i < x.size() - 1; i++)
    {
        double dx = x[i + 1] - x[i];
        double dy = y[i + 1] - y[i];
        s += hypot(dx, dy);
        s_vec.push_back(s);
    }
    // Compute the cubic spline coefficients
    tk::spline sx, sy;
    sx.set_points(s_vec, x);
    sy.set_points(s_vec, y);
    // Compute the path points
    std::vector<double> cx, cy, cyaw, ck;
    double target_s = s;
    for (double s = 0.0; s < target_s; s += ds)
    {
        cx.push_back(sx(s));
        cy.push_back(sy(s));
        cyaw.push_back(atan2(sy(s + ds) - sy(s), sx(s + ds) - sx(s)));
        ck.push_back(0.0);
    }
    return std::make_tuple(cx, cy, cyaw, ck, target_s);
}


// Normalize an angle to be between -pi and pi
double normalize_angle(double angle)
{
    while (angle > M_PI)
    {
        angle -= 2.0 * M_PI;
    }
    while (angle < -M_PI)
    {
        angle += 2.0 * M_PI;
    }
    return angle;
}

// State class representing the vehicle's state
class State
{
public:
    double x, y, yaw, v;
    State(double x = 0.0, double y = 0.0, double yaw = 0.0, double v = 0.0) : x(x), y(y), yaw(yaw), v(v) {}
    void update(double acceleration, double delta)
    {
        // Clip the steering angle to the maximum value
        delta = std::clamp(delta, -max_steer, max_steer);
        x += v * cos(yaw) * dt;
        y += v * sin(yaw) * dt;
        yaw += v / L * tan(delta) * dt;
        yaw = normalize_angle(yaw);
        v += acceleration * dt;
    }
};


// Compute the index of the target point on the path using the front axle's position
std::pair<int, double> calc_target_index(State state, const std::vector<double>& cx, const std::vector<double>& cy)
{
    // Compute the position of the front axle
    double fx = state.x + L * cos(state.yaw);
    double fy = state.y + L * sin(state.yaw);

    // Find the nearest point on the path
    double dx, dy, d, error_front_axle;
    int target_idx      = -1;
    double min_distance = std::numeric_limits<double>::max();
    for (int i = 0; i < cx.size(); i++)
    {
        dx = fx - cx[i];
        dy = fy - cy[i];
        d  = hypot(dx, dy);
        if (d < min_distance)
        {
            min_distance     = d;
            target_idx       = i;
            error_front_axle = dx * sin(state.yaw) - dy * cos(state.yaw);
        }
    }
    return {target_idx, error_front_axle};
}

// Calculate the steering angle using the Stanley control algorithm
double stanley_control(
    State state,
    const std::vector<double>& cx,
    const std::vector<double>& cy,
    const std::vector<double>& cyaw,
    int last_target_idx)
{
    // Find the index of the current target point and compute the error
    int current_target_idx;
    double error_front_axle;
    std::tie(current_target_idx, error_front_axle) = calc_target_index(state, cx, cy);
    // If we have passed the current target point, use the last target point
    if (last_target_idx >= current_target_idx)
    {
        current_target_idx = last_target_idx;
    }
    // Compute the heading error and cross track error
    double theta_e = normalize_angle(cyaw[current_target_idx] - state.yaw);
    double theta_d = atan2(k * error_front_axle, state.v);
    // Compute the steering angle
    return theta_e + theta_d;
}


// Compute the PID control for the vehicle's speed
double pid_control(double target_speed, double current_speed)
{
    return Kp * (target_speed - current_speed);
}

// Main function
int main()
{
    // Define the waypoints for the desired path
    std::vector<double> ax = {0.0, 100.0, 100.0, 50.0, 60.0};
    std::vector<double> ay = {0.0, 0.0, -30.0, -20.0, 0.0};
    // Compute the cubic spline path
    std::vector<double> cx, cy, cyaw, ck;
    double max_s;
    std::cout << "Calculating the cubic spline path..." << std::endl;
    std::tie(cx, cy, cyaw, ck, max_s) = calc_spline_course(ax, ay, 0.1);
    std::cout << "Done!" << std::endl;
    // Initialize the state of the vehicle
    State state(0.0, 5.0, M_PI / 4.0, 0.0);
    // Set up the simulation parameters
    double time = 0.0;
    std::vector<double> time_history, x_history, y_history, yaw_history, v_history;
    int last_target_idx = 0;

    // Run the simulation loop
    plt::backend("TkAgg");   // in case the segfault when exits
    while (last_target_idx < cx.size()-1)
    {
        // Calculate the steering angle using Stanley control
        double delta = stanley_control(state, cx, cy, cyaw, last_target_idx);
        // Calculate the acceleration using PID control
        double acceleration = pid_control(target_speed, state.v);
        // Update the state of the vehicle
        state.update(acceleration, delta);
        // Record the state history
        time_history.push_back(time);
        x_history.push_back(state.x);
        y_history.push_back(state.y);
        yaw_history.push_back(state.yaw);
        v_history.push_back(state.v);
        // Update the last target index
        std::tie(last_target_idx, std::ignore) = calc_target_index(state, cx, cy);
        // Increment the simulation time
        time += dt;

        // plot the results
        plt::clf();
        plt::plot(cx, cy, "k--");
        plt::plot(x_history, y_history, "b");
        plt::axis("equal");
        plt::grid(true);
        plt::pause(0.001);
    }
    std::cout << "Simulation complete!" << std::endl;
    plt::show();

    return 0;
}
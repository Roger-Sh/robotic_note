#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "matplotlibcpp.h"

#define PI 3.141592653
namespace plt = matplotlibcpp;

using Traj     = std::vector<std::array<float, 5>>;
using Obstacle = std::vector<std::array<float, 2>>;
using State    = std::array<float, 5>;
using Window   = std::array<float, 4>;
using Point    = std::array<float, 2>;
using Control  = std::array<float, 2>;

class Config
{
public:
    float max_speed    = 1.0;
    float min_speed    = -0.5;
    float max_yawrate  = 40.0 * PI / 180.0;
    float max_accel    = 0.2;
    float robot_radius = 1.0;
    float max_dyawrate = 40.0 * PI / 180.0;

    float v_reso       = 0.01;
    float yawrate_reso = 0.1 * PI / 180.0;

    float dt                = 0.1;
    float predict_time      = 3.0;
    float to_goal_cost_gain = 1.0;
    float speed_cost_gain   = 1.0;
};

State motion(State x, Control u, float dt)
{
    x[2] += u[1] * dt;
    x[0] += u[0] * std::cos(x[2]) * dt;
    x[1] += u[0] * std::sin(x[2]) * dt;
    x[3] = u[0];
    x[4] = u[1];
    return x;
};

Window calc_dynamic_window(State x, Config config)
{

    return {
        {std::max((x[3] - config.max_accel * config.dt), config.min_speed),
         std::min((x[3] + config.max_accel * config.dt), config.max_speed),
         std::max((x[4] - config.max_dyawrate * config.dt), -config.max_yawrate),
         std::min((x[4] + config.max_dyawrate * config.dt), config.max_yawrate)}
    };
};


Traj calc_trajectory(State x, float v, float y, Config config)
{

    Traj traj;
    traj.push_back(x);
    float time = 0.0;
    while (time <= config.predict_time)
    {
        x = motion(
            x,
            std::array<float, 2>{
                {v, y}
        },
            config.dt);
        traj.push_back(x);
        time += config.dt;
    }
    return traj;
};


float calc_obstacle_cost(Traj traj, Obstacle ob, Config config)
{
    // calc obstacle cost inf: collistion, 0:free
    int skip_n = 2;
    float minr = std::numeric_limits<float>::max();

    for (unsigned int ii = 0; ii < traj.size(); ii += skip_n)
    {
        for (unsigned int i = 0; i < ob.size(); i++)
        {
            float ox = ob[i][0];
            float oy = ob[i][1];
            float dx = traj[ii][0] - ox;
            float dy = traj[ii][1] - oy;

            float r = std::sqrt(dx * dx + dy * dy);
            if (r <= config.robot_radius)
            {
                return std::numeric_limits<float>::max();
            }

            if (minr >= r)
            {
                minr = r;
            }
        }
    }

    return 1.0 / minr;
};

float calc_to_goal_cost(Traj traj, Point goal, Config config)
{

    float goal_magnitude = std::sqrt(goal[0] * goal[0] + goal[1] * goal[1]);
    float traj_magnitude = std::sqrt(std::pow(traj.back()[0], 2) + std::pow(traj.back()[1], 2));
    float dot_product    = (goal[0] * traj.back()[0]) + (goal[1] * traj.back()[1]);
    float error          = dot_product / (goal_magnitude * traj_magnitude);
    float error_angle    = std::acos(error);
    float cost           = config.to_goal_cost_gain * error_angle;

    return cost;
};

Traj calc_final_input(State x, Control& u, Window dw, Config config, Point goal, std::vector<std::array<float, 2>> ob)
{

    float min_cost = 10000.0;
    Control min_u  = u;
    min_u[0]       = 0.0;
    Traj best_traj;

    // evalucate all trajectory with sampled input in dynamic window
    for (float v = dw[0]; v <= dw[1]; v += config.v_reso)
    {
        for (float y = dw[2]; y <= dw[3]; y += config.yawrate_reso)
        {

            Traj traj = calc_trajectory(x, v, y, config);

            float to_goal_cost = calc_to_goal_cost(traj, goal, config);
            float speed_cost   = config.speed_cost_gain * (config.max_speed - traj.back()[3]);
            float ob_cost      = calc_obstacle_cost(traj, ob, config);
            float final_cost   = to_goal_cost + speed_cost + ob_cost;

            if (min_cost >= final_cost)
            {
                min_cost = final_cost;
                min_u    = Control{
                       {v, y}
                };
                best_traj = traj;
            }
        }
    }
    u = min_u;
    return best_traj;
};


Traj dwa_control(State x, Control& u, Config config, Point goal, Obstacle ob)
{
    // # Dynamic Window control
    Window dw = calc_dynamic_window(x, config);
    Traj traj = calc_final_input(x, u, dw, config, goal, ob);

    return u, traj;
}

int main()
{
    State x({
        {0.0, 0.0, PI / 8.0, 0.0, 0.0}
    });
    Point goal({
        {10.0, 10.0}
    });
    Obstacle ob(
        {{{-1, -1}}, {{0, 2}}, {{4.0, 2.0}}, {{5.0, 4.0}}, {{5.0, 5.0}}, {{5.0, 6.0}}, {{5.0, 9.0}}, {{8.0, 9.0}}, {{7.0, 9.0}}, {{12.0, 12.0}}});

    Control u({
        {0.0, 0.0}
    });
    Config config;
    Traj traj;
    traj.push_back(x);

    bool terminal = false;
    plt::figure();
    for (int i = 0; i < 1000 && !terminal; i++)
    {
        Traj ltraj = dwa_control(x, u, config, goal, ob);
        x          = motion(x, u, config.dt);
        traj.push_back(x);

        // visualization
        plt::clf();
        plt::title("DWA");

        std::vector<double> goal_x = {goal[0]};
        std::vector<double> goal_y = {goal[1]};
        plt::scatter(goal_x, goal_y, 50);

        for (const auto& ob_i : ob)
        {
            std::vector<double> obs_x = {ob_i[0]};
            std::vector<double> obs_y = {ob_i[1]};
            plt::plot(obs_x, obs_y, "ok");
        }

        std::vector<double> ltraj_x, ltraj_y;
        for (const auto& traj_i : ltraj)
        {
            ltraj_x.push_back(traj_i[0]);
            ltraj_y.push_back(traj_i[1]);
        }
        plt::plot(ltraj_x, ltraj_y, "green");

        std::vector<double> current_x = {x[0]};
        std::vector<double> current_y = {x[1]};
        plt::plot(current_x, current_y, "or");

        plt::xlim(-5, 20);
        plt::ylim(-5, 15);
        plt::pause(0.001);

        if (std::sqrt(std::pow((x[0] - goal[0]), 2) + std::pow((x[1] - goal[1]), 2)) <= config.robot_radius)
        {
            terminal = true;
        }
    }
    plt::show();
    plt::detail::_interpreter::kill();   // in case the segfault when exits
    return 0;
}

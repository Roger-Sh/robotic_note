#include "matplotlibcpp.h"
#include "spline.h"

namespace plt = matplotlibcpp;
using namespace std;

// Vehicle parameters
const double Lfc = 2.0;   // [m] look-ahead distance
const double Kp  = 1.0;   // Proportional gain for speed control
const double k   = 0.1;   // Control gain for Stanley control
const double dt  = 0.1;   // Time step for simulation in seconds
const double wb  = 2.9;   // [m] wheel base of vehicle

// State class representing the vehicle's state
class State
{
public:
    State(double x = 0.0, double y = 0.0, double yaw = 0.0, double v = 0.0) : x(x), y(y), yaw(yaw), v(v)
    {
        rear_x = x - ((wb / 2) * cos(yaw));
        rear_y = y - ((wb / 2) * sin(yaw));
    }

    void update(double a, double delta)
    {
        x += v * cos(yaw) * dt;
        y += v * sin(yaw) * dt;
        yaw += v / wb * tan(delta) * dt;
        v += a * dt;
        rear_x = x - ((wb / 2) * cos(yaw));
        rear_y = y - ((wb / 2) * sin(yaw));
    }

    double calc_distance(double point_x, double point_y) const
    {
        double dx = rear_x - point_x;
        double dy = rear_y - point_y;
        return hypot(dx, dy);
    }

    double x;
    double y;
    double yaw;
    double v;
    double rear_x;
    double rear_y;
};

class States
{
private:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> yaw;
    std::vector<double> v;
    std::vector<double> tt;

public:
    States() {}

    const std::vector<double>& get_x() const
    {
        return x;
    }

    const std::vector<double>& get_y() const
    {
        return y;
    }

    void append(double t, State state)
    {
        x.push_back(state.x);
        y.push_back(state.y);
        yaw.push_back(state.yaw);
        v.push_back(state.v);
        tt.push_back(t);
    }
};

double proportional_control(double target, double current)
{
    double a = Kp * (target - current);

    return a;
}

class TargetCourse
{
public:
    std::vector<double> cx;
    std::vector<double> cy;
    mutable int old_nearest_point_index;

    TargetCourse(std::vector<double> cx, std::vector<double> cy) : cx(cx), cy(cy), old_nearest_point_index(-1) {}

    std::pair<int, double> search_target_index(const State& state) const
    {
        const double k   = 0.1;   // example value, replace with appropriate value
        const double Lfc = 2.0;   // example value, replace with appropriate value

        // To speed up nearest point search, doing it at only first time.
        if (old_nearest_point_index == -1)
        {
            // search nearest point index
            std::vector<double> dx(cx.size());
            std::vector<double> dy(cy.size());
            for (int i = 0; i < cx.size(); ++i)
            {
                dx[i] = state.rear_x - cx[i];
                dy[i] = state.rear_y - cy[i];
            }
            std::vector<double> d(cx.size());
            for (int i = 0; i < cx.size(); ++i)
            {
                d[i] = std::hypot(dx[i], dy[i]);
            }
            int ind                 = std::distance(d.begin(), std::min_element(d.begin(), d.end()));
            old_nearest_point_index = ind;
        }
        else
        {
            int ind                    = old_nearest_point_index;
            double distance_this_index = state.calc_distance(cx[ind], cy[ind]);
            while (true)
            {
                double distance_next_index = state.calc_distance(cx[ind + 1], cy[ind + 1]);
                if (distance_this_index < distance_next_index)
                {
                    break;
                }
                ind                 = (ind + 1 < cx.size()) ? ind + 1 : ind;
                distance_this_index = distance_next_index;
            }
            old_nearest_point_index = ind;
        }

        double Lf = k * state.v + Lfc;   // update look ahead distance

        // search look ahead target point index
        int ind = old_nearest_point_index;
        while (Lf > state.calc_distance(cx[ind], cy[ind]))
        {
            if (ind + 1 >= cx.size())
            {
                break;   // not exceed goal
            }
            ++ind;
        }

        return std::make_pair(ind, Lf);
    }
};

std::tuple<double, int> pure_pursuit_steer_control(const State& state, const TargetCourse& trajectory, int pind)
{
    int ind;
    double Lf;
    std::tie(ind, Lf) = trajectory.search_target_index(state);

    if (pind >= ind)
    {
        ind = pind;
    }

    double tx, ty;
    if (ind < trajectory.cx.size())
    {
        tx = trajectory.cx[ind];
        ty = trajectory.cy[ind];
    }
    else
    {   // toward goal
        tx  = trajectory.cx.back();
        ty  = trajectory.cy.back();
        ind = trajectory.cx.size() - 1;
    }

    double alpha = std::atan2(ty - state.rear_y, tx - state.rear_x) - state.yaw;

    double delta = std::atan2(2.0 * wb * std::sin(alpha) / Lf, 1.0);

    return std::make_tuple(delta, ind);
}

template<typename T>
std::vector<T> linspace(T start, T stop, size_t num)
{
    std::vector<T> result(num);
    T delta = (stop - start) / (num - 1);
    for (size_t i = 0; i < num; ++i)
    {
        result[i] = start + i * delta;
    }
    return result;
}

// main function to print hello world
int main()
{
    double dt           = 0.1;    // example value, replace with appropriate value
    bool show_animation = true;   // example value, replace with appropriate value

    // target course
    std::vector<double> cx = linspace(0.0, 50.0, 100);
    std::vector<double> cy(cx.size());
    std::transform(
        cx.begin(),
        cx.end(),
        cy.begin(),
        [](double ix) -> double
        {
            return std::sin(ix / 5.0) * ix / 2.0;
        });

    double target_speed = 10.0 / 3.6;   // [m/s]

    double T = 100.0;   // max simulation time

    // initial state
    State state(0.0, -3.0, 0.0, 0.0);

    int lastIndex = cx.size() - 1;
    double time   = 0.0;
    States states;
    states.append(time, state);
    TargetCourse target_course(cx, cy);
    int target_ind = target_course.search_target_index(state).first;

    plt::backend("TkAgg");   // in case the segfault when exits
    while (lastIndex > target_ind)
    {
        // Calc control input
        double ai = proportional_control(target_speed, state.v);
        double di;
        std::tie(di, target_ind) = pure_pursuit_steer_control(state, target_course, target_ind);

        state.update(ai, di);   // Control vehicle

        time += dt;
        states.append(time, state);

        std::vector<double> xxx;
        std::vector<double> yyy;
        xxx.push_back(cx[target_ind]);
        yyy.push_back(cy[target_ind]);

        plt::clf();
        plt::plot(cx, cy, "-r");
        plt::plot(states.get_x(), states.get_y(), "-b");
        plt::plot(xxx, yyy, "bo");
        plt::axis("equal");
        plt::grid(true);
        plt::title("Speed[km/h]:" + std::to_string(state.v * 3.6).substr(0, 4));
        plt::pause(0.001);
    }
    return 0;
}
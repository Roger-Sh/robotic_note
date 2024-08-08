#include "Dubins.h"

int main()
{
    Vector3d start(1.0, 1.0, (double)45 / 180 * PI);
    Vector3d goal(-3.0, -3.0, (double)-45 / 180 * PI);
    double curvature = 1;
    double step_size = 0.1;
    Dubins dubins;
    Dubins::ResultDubins rd = dubins.dubins_path_planning(start, goal, curvature, step_size);
    plt::figure(1);
    plt::plot(rd.p_x, rd.p_y);
    plt::plot(vector<double>{start[0]}, vector<double>{start[1]}, "og");
    plt::plot(vector<double>{goal[0]}, vector<double>{goal[1]}, "xb");
    plt::arrow(start[0], start[1], 0.5, 0.5);   // draw the arrow
    plt::arrow(goal[0], goal[1], 0.5, -0.5);    // draw the arrow
    plt::grid(true);
    plt::axis("equal");
    plt::title("mode: " + rd.mode);
    plt::show();
    plt::detail::_interpreter::kill();   // in case the segfault when exits
    return 0;
}
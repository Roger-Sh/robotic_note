#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "car.h"
#include "controller.h"
#include "draw.h"

int main()
{
    std::vector<cv::Point> way_points;

    const int W = 700, H = 700;
    Draw draw(W, H, "Canvas");

    Car car_pid(30, 30);
    PID controller_pid(0.5, 0.1, 0, 3, 0.1, 0);

    for (int x = 200; x < 600; x += 2)
    {
        double y = H / 2 + 200 * sin(2 * M_PI * 0.25 * (x + 200) / 100);
        way_points.emplace_back(x, static_cast<int>(y));
    }

    int current_idx_pid = 0;
    double linear_v = 0, angular_v = 0;
    std::vector<cv::Point> pid_car_points;

    auto find_nearest_point = [&way_points](const cv::Point& point)
    {
        std::vector<double> distances;
        for (const auto& p : way_points)
        {
            distances.push_back(getDistance(point.x, point.y, p.x, p.y));
        }
        auto it = std::min_element(distances.begin(), distances.end());
        return std::distance(distances.begin(), it);
    };

    while (true)
    {
        draw.clear();

        // Trajectory
        if (!way_points.empty())
        {
            draw.draw_path(way_points, cv::Scalar(0, 0, 255), 1);
        }

        // Target
        if (!pid_car_points.empty())
        {
            draw.draw_path(pid_car_points, cv::Scalar(0, 255, 0), 1, true);
        }

        // Car
        Eigen::MatrixXd matrix = car_pid.get_points();
        std::vector<cv::Point> points;
        for (int i = 0; i < matrix.rows(); i++)
        {
            cv::Point point(matrix(i, 0), matrix(i, 1));
            points.push_back(point);
        }
        draw.draw(points, cv::Scalar(255, 0, 0), 1);

        // Text
        draw.add_text("PID Controller", cv::Scalar(255, 0, 0), 1, 1, cv::Point(100, 50));
        draw.add_text("Trajectory", cv::Scalar(0, 0, 255), 1, 1, cv::Point(100, 100));
        draw.add_text("Target", cv::Scalar(0, 255, 0), 1, 1, cv::Point(100, 150));

        // PID Car
        auto [x, _] = car_pid.get_state();
        if (!way_points.empty() && current_idx_pid != way_points.size())
        {
            pid_car_points.emplace_back(static_cast<int>(x.data()[0]), static_cast<int>(x.data()[1]));
            cv::Point goal_pt = way_points[current_idx_pid];
            cv::circle(draw.canvas, goal_pt, 5, cv::Scalar(0, 255, 0), 2);
            Eigen::Vector2d vec_goal(goal_pt.x, goal_pt.y);
            std::tie(linear_v, angular_v) = controller_pid.getControlInputs(x, vec_goal, current_idx_pid);
            double dist                   = getDistance(x.data()[0], x.data()[1], goal_pt.x, goal_pt.y);
            if (dist < 20)
            {
                current_idx_pid++;
            }
        }
        else
        {
            linear_v  = 0;
            angular_v = 0;
        }
        car_pid.set_robot_velocity(linear_v, angular_v);
        car_pid.update(0.5);

        cv::imshow(draw.window_name, draw.canvas);
        int k = cv::waitKey(20);
        if (k == 'q')
        {
            break;
        }
    }

    return 0;
}
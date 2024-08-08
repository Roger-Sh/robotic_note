/**
 * @file arm_obstacle_navigation.cpp
 * @author Weipu Shan (weipu.shan@foxmail.com)
 * @brief Demo for 2D-plane arm obstacle navigation with A-Star for path planning
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <assert.h>
#include <matplot/matplot.h>
#include <unistd.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <vector>

/**
 * @brief global parameters
 *
 */

// map size
// Discrete the joint space [0, 180] in M sections.
const int M(100);

// obstacle param, x, y, diameter
const std::vector<std::tuple<double, double, double>> OBSTACLES = {
    {1.75, 0.75, 0.6},
    {0.55, 1.5,  0.5},
    {0,    -1,   0.7}
};

// show flag
const bool SHOW_ASTAR_PROCESS(false);

/**
 * @brief Grid Node Type
 *
 */
enum GridNodeType
{
    not_set_node = -1,
    free_node = 0,
    obstacle_node = 1,
    current_node = 2,
    neighbor_node = 3,
    start_node = 4,
    goal_node = 5,
    route_node = 6,
};

/**
 * @brief class NLinkArm in 2D planar
 *
 */
class NLinkArm
{
public:
    std::vector<double> link_lengths_;
    std::vector<double> joint_angles_;
    int n_links_;
    std::vector<std::pair<double, double>> points_;
    double lim_;
    std::pair<double, double> end_effector_;

public:
    /**
     * @brief Construct a new NLinkArm object
     *
     * @param link_lengths
     * @param joint_angles
     */
    NLinkArm(const std::vector<double> link_lengths, const std::vector<double> joint_angles)
    {
        // check size
        assert(link_lengths.size() == joint_angles.size());

        // init memebers
        this->n_links_ = link_lengths.size();
        this->link_lengths_ = link_lengths;
        this->joint_angles_ = joint_angles;
        for (size_t i = 0; i < this->n_links_ + 1; i++)
        {
            this->points_.push_back({0.0, 0.0});
        }
        this->lim_ = std::accumulate(this->link_lengths_.begin(), this->link_lengths_.end(), 0.0);

        // update points
        this->update_points();
    }

    /**
     * @brief Destroy the NLinkArm object
     *
     */
    ~NLinkArm() {}

    /**
     * @brief update arm points using forward kinematic
     *
     */
    void update_points()
    {
        // run forward kinematics
        for (size_t i = 1; i < this->n_links_ + 1; i++)
        {
            this->points_[i].first =
                this->points_[i - 1].first +
                this->link_lengths_[i - 1] *
                    std::cos(std::accumulate(this->joint_angles_.begin(), this->joint_angles_.begin() + i, 0.0));
            this->points_[i].second =
                this->points_[i - 1].second +
                this->link_lengths_[i - 1] *
                    std::sin(std::accumulate(this->joint_angles_.begin(), this->joint_angles_.begin() + i, 0.0));
        }

        // update end_effector
        this->end_effector_ = this->points_[this->n_links_];
    }

    /**
     * @brief update arm joints
     *
     * @param joint_angles
     */
    void update_joints(std::vector<double> joint_angles)
    {
        assert(this->joint_angles_.size() == joint_angles.size());
        this->joint_angles_ = joint_angles;
        this->update_points();
    }

    /**
     * @brief plot arm in 2d plane with obstacles
     *
     * @param obstacles
     * @param goal_joints
     * @param pause
     */
    void plot(
        const std::vector<std::tuple<double, double, double>> obstacles = {},
        std::vector<double> goal_joints = {0, 0},
        bool pause = false)
    {
        matplot::cla();

        // set fig
        matplot::hold(matplot::on);

        // draw obstacle
        for (size_t i = 0; i < obstacles.size(); i++)
        {
            double ob_x = std::get<0>(obstacles[i]);
            double ob_y = std::get<1>(obstacles[i]);
            double ob_r = std::get<2>(obstacles[i]) / 2.0;

            auto circle = matplot::ellipse(ob_x - ob_r, ob_y - ob_r, 2 * ob_r, 2 * ob_r);
            circle->fill(matplot::on);
        }

        // draw goal
        if (fabs(goal_joints[0]) > 0 || fabs(goal_joints[1]) > 0)
        {
            std::vector<std::pair<double, double>> points;
            points.resize(this->n_links_ + 1);
            for (size_t i = 1; i < this->n_links_ + 1; i++)
            {
                points[i].first = points[i - 1].first +
                                  this->link_lengths_[i - 1] *
                                      std::cos(std::accumulate(goal_joints.begin(), goal_joints.begin() + i, 0.0));
                points[i].second = points[i - 1].second +
                                   this->link_lengths_[i - 1] *
                                       std::sin(std::accumulate(goal_joints.begin(), goal_joints.begin() + i, 0.0));
            }
            auto goal = matplot::ellipse(
                points[points.size() - 1].first - 0.02,
                points[points.size() - 1].second - 0.02,
                2 * 0.02,
                2 * 0.02);
            goal->line_width(10);
            goal->color(matplot::color::red);
        }

        // draw arm link
        std::vector<double> arm_link_x;
        std::vector<double> arm_link_y;
        std::vector<size_t> arm_link_index;
        for (size_t i = 0; i < this->points_.size(); i++)
        {
            arm_link_x.push_back(this->points_[i].first);
            arm_link_y.push_back(this->points_[i].second);
            arm_link_index.push_back(i);
        }
        auto arm_link = matplot::plot(arm_link_x, arm_link_y, "-b");
        arm_link->line_width(5);
        arm_link->marker_indices(arm_link_index);
        arm_link->marker_style(matplot::line_spec::marker_style::point);
        arm_link->marker_size(10);

        // axis
        matplot::xlabel("x");
        matplot::ylabel("y");
        matplot::xlim({-this->lim_, this->lim_});
        matplot::ylim({-this->lim_, this->lim_});

        // matplot::hold(matplot::off);

        matplot::gcf()->size(800, 800);

        if (pause)
        {
            matplot::show();
        }
        else
        {
            matplot::gcf()->draw();
            usleep(200000);
        }
    }
};

/**
 * @brief detect collision between arm and obstacle
 * Determines whether a line segment (arm link) is in contact
   with a circle (obstacle).
 * @param line_seg
 * @param obstacle
 * @return true: collision
 * @return false: collision free
 */
bool detect_collision(
    const std::vector<std::pair<double, double>> line_seg,
    const std::tuple<double, double, double> obstacle)
{
    Eigen::Vector2d a_vec = {line_seg[0].first, line_seg[0].second};
    Eigen::Vector2d b_vec = {line_seg[1].first, line_seg[1].second};
    Eigen::Vector2d c_vec = {std::get<0>(obstacle), std::get<1>(obstacle)};
    double radius = std::get<2>(obstacle) / 2.0;
    Eigen::Vector2d line_vec = b_vec - a_vec;
    double line_mag = line_vec.norm();
    Eigen::Vector2d circle_vec = c_vec - a_vec;
    double proj = circle_vec.dot(line_vec / line_mag);

    Eigen::Vector2d closest_point;
    if (proj <= 0)
    {
        closest_point = a_vec;
    }
    else if (proj >= line_mag)
    {
        closest_point = b_vec;
    }
    else
    {
        closest_point = a_vec + line_vec * proj / line_mag;
    }

    // make obstacle bigger for safety
    if ((closest_point - c_vec).norm() > radius * 2.0)
    {
        return false;
    }

    return true;
}

/**
 * @brief Create a map with size and init value
 *
 * @param map_size
 * @param map_value
 * @return std::vector<std::pair<int, int>>
 */
std::vector<std::vector<int>> create_map(const int map_size, int map_value)
{
    std::vector<std::vector<int>> map(map_size, std::vector<int>(map_size));
    for (size_t i = 0; i < map_size; i++)
    {
        for (size_t j = 0; j < map_size; j++)
        {
            map[i][j] = map_value;
        }
    }

    return map;
}

/**
 * @brief Create a node map object
 *
 * @param map_size
 * @param node_val
 * @return std::vector<std::vector<std::pair<int, int>>>: generated map
 */
std::vector<std::vector<std::pair<int, int>>> create_node_map(int map_size, int node_val)
{
    std::vector<std::vector<std::pair<int, int>>> map(map_size, std::vector<std::pair<int, int>>(map_size));
    for (size_t i = 0; i < map_size; i++)
    {
        for (size_t j = 0; j < map_size; j++)
        {
            map[i][j] = std::make_pair(node_val, node_val);
        }
    }
    return map;
}

/**
 * @brief Get the occupancy grid object
 * Discretizes joint space into M values from -pi to +pi
   and determines whether a given coordinate in joint space
   would result in a collision between a robot arm and obstacles
   in its environment.
 * @param arm: An instance of NLinkArm
 * @param obstacles: A vector of obstacles, with each obstacle defined as a vector
                   of xy coordinates and a diameter.
 * @return std::vector<std::vector<int>>: Occupancy grid in joint space
 */
std::vector<std::vector<int>> get_occupancy_grid(
    NLinkArm arm,
    const std::vector<std::tuple<double, double, double>> obstacles = {})
{
    // create grid
    std::vector<std::vector<int>> grid = create_map(M, GridNodeType::free_node);

    // create theta_list for discrete joint space
    std::vector<double> theta_list;
    for (int i = -M / 2; i < M / 2 + 1; i++)
    {
        theta_list.push_back(2.0 * i * M_PI / M);
    }

    int collision_count = 0;

    // check diskrete joint space
    for (size_t i = 0; i < M; i++)
    {
        for (size_t j = 0; j < M; j++)
        {
            // check current joint configuration
            arm.update_joints({theta_list[i], theta_list[j]});
            auto points = arm.points_;
            bool collision_detected = false;
            for (size_t k = 0; k < points.size() - 1; k++)
            {
                // check all obstacles
                for (auto& obstacle : obstacles)
                {
                    // arm link seg
                    std::vector<std::pair<double, double>> line_seg = {points[k], points[k + 1]};
                    collision_detected = detect_collision(line_seg, obstacle);
                    if (collision_detected)
                    {
                        break;
                    }
                }
                if (collision_detected)
                {
                    break;
                }
            }

            grid[i][j] = int(collision_detected);

            if (collision_detected)
            {
                collision_count++;
            }
        }
    }

    std::cout << "collision_count: " << collision_count << "\n";

    return grid;
}

/**
 * @brief show occupancy grid using matplot++
 *
 * @param grid
 * @param pause
 */
void show_grid(const std::vector<std::vector<int>> grid, bool pause = true)
{
    matplot::cla();

    matplot::hold(matplot::on);

    std::vector<double> grid_x;
    std::vector<double> grid_y;
    // std::vector<double> grid_size;
    std::vector<double> grid_color;

    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] != 0)
            {
                grid_x.push_back(i);
                grid_y.push_back(j);
                grid_color.push_back(grid[i][j]);
            }
        }
    }
    auto fig = matplot::scatter(grid_x, grid_y, 10, grid_color);
    fig->marker_style(matplot::line_spec::marker_style::square);
    fig->marker_face(matplot::on);

    matplot::xlabel("joint-1");
    matplot::ylabel("joint-2");
    matplot::hold(matplot::off);
    matplot::gcf()->size(800, 800);

    if (pause)
    {
        matplot::show();
    }
    else
    {
        matplot::gcf()->draw();
        usleep(300000);
    }
}

/**
 * @brief calculate heuristic cost map to goal node
 * check and repalce the cost of every node through 4 corner nodes to goal node
 *
 * @param grid_size
 * @param goal_node
 * @return std::vector<std::vector<int>>
 */
std::vector<std::vector<int>> calc_heuristic_map(const int grid_size, std::pair<int, int> goal_node)
{
    // init heuristic_map
    std::vector<std::vector<int>> heuristic_map(grid_size, std::vector<int>(grid_size));

    // calc dist from every node to goal_node
    for (int i = 0; i < heuristic_map.size(); i++)
    {
        for (int j = 0; j < heuristic_map[i].size(); j++)
        {
            heuristic_map[i][j] = abs(i - goal_node.first) + abs(j - goal_node.second);
        }
    }

    // for every node: check the cost through 4 corners to goal pose
    for (int i = 0; i < heuristic_map.size(); i++)
    {
        for (int j = 0; j < heuristic_map[i].size(); j++)
        {
            // heuristic_map[i][j] = std::min(
            //     {heuristic_map[i][j],
            //      i + 1 + heuristic_map[M - 1][j],
            //      M - i + heuristic_map[0][j],
            //      j + 1 + heuristic_map[i][M - 1],
            //      M - j + heuristic_map[i][0]});

            heuristic_map[i][j] = std::min(
                {heuristic_map[i][j],
                 M - i - 1 + heuristic_map[M - 1][j],
                 i + heuristic_map[0][j],
                 M - j - 1 + heuristic_map[i][M - 1],
                 j + heuristic_map[i][0]});
        }
    }

    return heuristic_map;
}

/**
 * @brief Get the min node in map object
 *
 * @param map
 * @return std::pair<int, int>: node with min value
 */
std::tuple<std::pair<int, int>, int> get_min_node_in_map(const std::vector<std::vector<int>> map)
{
    int min_val = map[0][0];
    int min_i = 0;
    int min_j = 0;
    for (size_t i = 0; i < map.size(); i++)
    {
        for (size_t j = 0; j < map.size(); j++)
        {
            if (map[i][j] < min_val)
            {
                min_val = map[i][j];
                min_i = i;
                min_j = j;
            }
        }
    }

    return std::make_tuple(std::make_pair(min_i, min_j), min_val);
}

/**
 * @brief find neighbors in torus map
 *
 * @param map_size
 * @param current_node
 * @return std::vector<std::pair<int, int>>: a vector of neighbor nodes
 */
std::vector<std::pair<int, int>> find_neighbors_in_torus_map(const int map_size, const std::pair<int, int> current_node)
{
    std::vector<std::pair<int, int>> neighbors;

    if (current_node.first - 1 >= 0)
    {
        neighbors.push_back(std::make_pair(current_node.first - 1, current_node.second));
    }
    else
    {
        neighbors.push_back(std::make_pair(map_size - 1, current_node.second));
    }

    if (current_node.first + 1 < map_size)
    {
        neighbors.push_back(std::make_pair(current_node.first + 1, current_node.second));
    }
    else
    {
        neighbors.push_back(std::make_pair(0, current_node.second));
    }

    if (current_node.second - 1 >= 0)
    {
        neighbors.push_back(std::make_pair(current_node.first, current_node.second - 1));
    }
    else
    {
        neighbors.push_back(std::make_pair(current_node.first, map_size - 1));
    }

    if (current_node.second + 1 < map_size)
    {
        neighbors.push_back(std::make_pair(current_node.first, current_node.second + 1));
    }
    else
    {
        neighbors.push_back(std::make_pair(current_node.first, 0));
    }

    return neighbors;
}

/**
 * @brief A-star pathplanning with torus grid
 *  Finds a path between an initial and goal joint configuration using
    the A* Algorithm on a tororiadal grid.
 * @param grid: An occupancy grid
 * @param start_node: Initial joint configuration
 * @param goal_node: Goal joint configuration
 * @return std::vector<std::pair<int, int>>: Obstacle-free route in joint space from start_node to goal_node
 */
std::vector<std::pair<int, int>> astar_torus(
    std::vector<std::vector<int>> occupancy_grid,
    std::pair<int, int> start_node,
    std::pair<int, int> goal_node)
{
    // init parent_map
    std::vector<std::vector<std::pair<int, int>>> parent_map = create_node_map(M, GridNodeType::not_set_node);
    // std::cout << "init parent_map: \n";
    // for (size_t i = 0; i < parent_map.size(); i++)
    // {
    //     for (size_t j = 0; j < parent_map[i].size(); j++)
    //     {
    //         std::cout << parent_map[i][j].first << " " << parent_map[i][j].second << ", ";
    //     }
    //     std::cout << "\n";
    // }

    // generate heuristic_map
    std::vector<std::vector<int>> heuristic_map = calc_heuristic_map(M, goal_node);
    // std::cout << "generate heuristic_map: \n";
    // for (size_t i = 0; i < heuristic_map.size(); i++)
    // {
    //     for (size_t j = 0; j < heuristic_map[i].size(); j++)
    //     {
    //         std::cout << heuristic_map[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // init explored_heuristic_map with INT_MAX
    std::vector<std::vector<int>> explored_heuristic_map = create_map(M, INT_MAX);
    // std::cout << "init explored_heuristic_map with INT_MAX\n";
    // for (size_t i = 0; i < explored_heuristic_map.size(); i++)
    // {
    //     for (size_t j = 0; j < explored_heuristic_map[i].size(); j++)
    //     {
    //         std::cout << explored_heuristic_map[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // init distance_map with INT_MAX
    std::vector<std::vector<int>> distance_map = create_map(M, INT_MAX);
    // std::cout << "init distance_map with INT_MAX\n";
    // for (size_t i = 0; i < distance_map.size(); i++)
    // {
    //     for (size_t j = 0; j < distance_map[i].size(); j++)
    //     {
    //         std::cout << distance_map[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // set start_node and goal_node
    explored_heuristic_map[start_node.first][start_node.second] = heuristic_map[start_node.first][start_node.second];
    std::cout << "set explored_heuristic_map start_node\n";
    distance_map[start_node.first][start_node.second] = 0;
    std::cout << "set distance_map start_node\n";
    occupancy_grid[start_node.first][start_node.second] = GridNodeType::start_node;
    std::cout << "set occupancy_grid start_node\n";
    occupancy_grid[goal_node.first][goal_node.second] = GridNodeType::goal_node;
    std::cout << "set occupancy_grid goal_node\n";

    int iteration = 0;
    while (true)
    {
        iteration++;
        std::cout << "iteration: " << iteration << "\n";

        // get current_node in explored heuristic map, which has min_distance to goal_node
        auto [current_node, min_distance] = get_min_node_in_map(explored_heuristic_map);
        std::cout << "current_node.first: " << current_node.first << "\n";
        std::cout << "current_node.second: " << current_node.second << "\n";
        std::cout << "min_distance: " << min_distance << "\n";

        // check reach goal
        if ((current_node.first == goal_node.first && current_node.second == goal_node.second) ||
            min_distance == INT_MAX)
        {
            std::cout << "reach goal_node\n";
            break;
        }

        // set current_node in occupancy_grid
        occupancy_grid[current_node.first][current_node.second] = GridNodeType::current_node;

        // reset explored node to INT_MAX
        explored_heuristic_map[current_node.first][current_node.second] = INT_MAX;

        // find neighbors in torus map
        auto neighbors = find_neighbors_in_torus_map(M, current_node);

        // check neighbor
        for (auto& neighbor : neighbors)
        {
            // skip obstacle in occupancy_grid
            if (occupancy_grid[neighbor.first][neighbor.second] == GridNodeType::free_node ||
                occupancy_grid[neighbor.first][neighbor.second] == GridNodeType::goal_node)
            {
                // calc current distance
                distance_map[neighbor.first][neighbor.second] =
                    distance_map[current_node.first][current_node.second] + 1;

                // update explored heuristic map
                explored_heuristic_map[neighbor.first][neighbor.second] =
                    heuristic_map[neighbor.first][neighbor.second];

                // store current_node in its neighbor's parent map
                parent_map[neighbor.first][neighbor.second] = current_node;
                occupancy_grid[neighbor.first][neighbor.second] = GridNodeType::neighbor_node;
            }
        }
    }

    // get route
    std::vector<std::pair<int, int>> route;
    std::vector<std::pair<int, int>> route_reverse;
    if (explored_heuristic_map[goal_node.first][goal_node.second] == INT_MAX)
    {
        std::cout << "route not found.\n";
        return route;
    }
    else
    {
        std::cout << "route found.\n";

        // get route
        route.push_back(goal_node);
        while (parent_map[(route.end() - 1)->first][(route.end() - 1)->second].first != GridNodeType::not_set_node &&
               parent_map[(route.end() - 1)->first][(route.end() - 1)->second].second != GridNodeType::not_set_node)
        {
            route.push_back(parent_map[(route.end() - 1)->first][(route.end() - 1)->second]);
        }
        std::cout << "The route found covers " << route.size() << " grid cells\n";

        // set route in grid
        for (int i = route.size() - 1; i >= 0; i--)
        {
            occupancy_grid[route[i].first][route[i].second] = GridNodeType::route_node;
            if (SHOW_ASTAR_PROCESS)
            {
                show_grid(occupancy_grid, false);
            }
            route_reverse.push_back(route[i]);
        }
        show_grid(occupancy_grid, true);
    }

    return route_reverse;
}

/**
 * @brief main process of arm obstacle navigation
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const* argv[])
{
    // create NLinkArm
    NLinkArm arm({1.0, 1.0}, {0, 0});
    // NLinkArm arm({0.5, 1.5}, {0, 0});

    // plot arm with obstacles
    arm.plot(OBSTACLES, {0, 0}, true);

    // generate joint space occupancy grid, check collision with obstacles
    std::vector<std::vector<int>> occupancy_grid = get_occupancy_grid(arm, OBSTACLES);
    show_grid(occupancy_grid, true);

    // get arm route in joint space using astar with torus grid
    std::pair<int, int> start_point_in_joint_space_index = {10, 50};
    std::pair<int, int> goal_point_in_joint_space_index = {58, 56};
    std::vector<double> goal_joints = {
        2 * M_PI * goal_point_in_joint_space_index.first / M - M_PI,
        2 * M_PI * goal_point_in_joint_space_index.second / M - M_PI};

    auto route = astar_torus(occupancy_grid, start_point_in_joint_space_index, goal_point_in_joint_space_index);

    for (auto& node : route)
    {
        double theta1 = 2 * M_PI * node.first / M - M_PI;
        double theta2 = 2 * M_PI * node.second / M - M_PI;
        arm.update_joints({theta1, theta2});
        arm.plot(OBSTACLES, goal_joints, false);
    }
    arm.plot(OBSTACLES, goal_joints, true);

    return 0;
}

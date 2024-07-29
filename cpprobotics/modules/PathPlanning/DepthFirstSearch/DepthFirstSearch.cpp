#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>

#include "matplotlibcpp.h"
#include "spline.h"

namespace plt = matplotlibcpp;
using namespace std;

class Node
{
public:
    int x;
    int y;
    double sum_cost;
    Node* p_node;

    Node(int x_, int y_, double sum_cost_ = 0, Node* p_node_ = NULL) : x(x_), y(y_), sum_cost(sum_cost_), p_node(p_node_){};
};

std::pair<std::vector<double>, std::vector<double>> calc_final_path(Node* goal, double reso)
{
    std::vector<double> rx;
    std::vector<double> ry;
    Node* node = goal;
    while (node->p_node != NULL)
    {
        node = node->p_node;
        rx.push_back(node->x * reso);
        ry.push_back(node->y * reso);
    }
    return {rx, ry};
}

std::vector<std::vector<int>> calc_obstacle_map(
    std::vector<int> ox,
    std::vector<int> oy,
    const int min_ox,
    const int max_ox,
    const int min_oy,
    const int max_oy,
    double reso,
    double vr)
{

    int xwidth = max_ox - min_ox;
    int ywidth = max_oy - min_oy;

    std::vector<std::vector<int>> obmap(ywidth, vector<int>(xwidth, 0));

    for (int i = 0; i < xwidth; i++)
    {
        int x = i + min_ox;
        for (int j = 0; j < ywidth; j++)
        {
            int y = j + min_oy;
            for (int k = 0; k < ox.size(); k++)
            {
                double d = std::sqrt(std::pow((ox[k] - x), 2) + std::pow((oy[k] - y), 2));
                if (d <= vr / reso)
                {
                    obmap[i][j] = 1;
                    break;
                }
            }
        }
    }
    return obmap;
}

bool verify_node(Node* node, vector<vector<int>> obmap, int min_ox, int max_ox, int min_oy, int max_oy)
{
    if (node->x < min_ox || node->y < min_oy || node->x >= max_ox || node->y >= max_oy)
    {
        return false;
    }

    if (obmap[node->x - min_ox][node->y - min_oy])
        return false;

    return true;
}

std::vector<Node> get_motion_model()
{
    return {
        Node(1, 0, 1),
        Node(0, 1, 1),
        Node(-1, 0, 1),
        Node(0, -1, 1),
        Node(-1, -1, std::sqrt(2)),
        Node(-1, 1, std::sqrt(2)),
        Node(1, -1, std::sqrt(2)),
        Node(1, 1, std::sqrt(2))};
}

std::pair<std::vector<double>, std::vector<double>> breadth_first_search(
    vector<double> sx,
    vector<double> sy,
    vector<double> gx,
    vector<double> gy,
    vector<double> ox_,
    vector<double> oy_,
    double reso,
    double rr)
{

    Node* nstart = new Node((int)std::round(sx[0] / reso), (int)std::round(sy[0] / reso), 0.0);
    Node* ngoal  = new Node((int)std::round(gx[0] / reso), (int)std::round(gy[0] / reso), 0.0);

    vector<int> ox;
    vector<int> oy;

    int min_ox = std::numeric_limits<int>::max();
    int max_ox = std::numeric_limits<int>::min();
    int min_oy = std::numeric_limits<int>::max();
    int max_oy = std::numeric_limits<int>::min();

    for (double iox : ox_)
    {
        int map_x = (int)std::round(iox * 1.0 / reso);
        ox.push_back(map_x);
        min_ox = std::min(map_x, min_ox);
        max_ox = std::max(map_x, max_ox);
    }

    for (double ioy : oy_)
    {
        int map_y = (int)std::round(ioy * 1.0 / reso);
        oy.push_back(map_y);
        min_oy = std::min(map_y, min_oy);
        max_oy = std::max(map_y, max_oy);
    }

    int xwidth = max_ox - min_ox;
    int ywidth = max_oy - min_oy;

    std::vector<std::vector<int>> visit_map(xwidth, vector<int>(ywidth, 0));
    std::vector<std::vector<double>> path_cost(xwidth, vector<double>(ywidth, std::numeric_limits<double>::max()));
    path_cost[nstart->x][nstart->y] = 0;

    std::vector<std::vector<int>> obmap = calc_obstacle_map(ox, oy, min_ox, max_ox, min_oy, max_oy, reso, rr);

    // NOTE: d_ary_heap should be a better choice here
    // auto cmp = [](const Node* left, const Node* right)
    // {
    //     return left->sum_cost > right->sum_cost;
    // };
    // std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> pq(cmp);

    // std::queue<Node*> pq;
    std::stack<Node*> pq;

    pq.push(nstart);
    std::vector<Node> motion = get_motion_model();

    std::vector<double> vis_x, vis_y;
    while (true)
    {
        // Node* node = pq.top();
        // Node* node = pq.front();
        Node* node = pq.top();
        if (visit_map[node->x - min_ox][node->y - min_oy] == 1)
        {
            // pq.pop();
            pq.pop();
            delete node;
            continue;
        }
        else
        {
            pq.pop();
            visit_map[node->x - min_ox][node->y - min_oy] = 1;
        }

        if (node->x == ngoal->x && node->y == ngoal->y)
        {
            ngoal->sum_cost = node->sum_cost;
            ngoal->p_node   = node;
            break;
        }

        // expand_grid search grid based on motion model
        for (int i = 0; i < motion.size(); i++)
        {
            Node* new_node = new Node(node->x + motion[i].x, node->y + motion[i].y, path_cost[node->x][node->y] + motion[i].sum_cost, node);

            if (!verify_node(new_node, obmap, min_ox, max_ox, min_oy, max_oy))
            {
                delete new_node;
                continue;
            }

            if (visit_map[new_node->x - min_ox][new_node->y - min_oy])
            {
                delete new_node;
                continue;
            }

            vis_x.push_back(new_node->x * reso);
            vis_y.push_back(new_node->y * reso);
            if (path_cost[node->x][node->y] + motion[i].sum_cost < path_cost[new_node->x][new_node->y])
            {
                path_cost[new_node->x][new_node->y] = path_cost[node->x][node->y] + motion[i].sum_cost;
                pq.push(new_node);
            }
        }

        // every 2 iteration, plot the current path
        if (pq.size() % 2 == 0)
        {
            // clear the canvas
            plt::clf();
            // plot the obstacle
            plt::plot(ox_, oy_, "ok");
            // plot the start and goal
            plt::plot(sx, sy, "og");
            plt::plot(gx, gy, "xb");
            // plot the current path
            plt::plot(vis_x, vis_y, "xc");
            plt::pause(0.001);
        }
    }

    std::vector<double> rx;
    std::vector<double> ry;
    std::tie(rx, ry) = calc_final_path(ngoal, reso);
    delete ngoal;
    delete nstart;
    return {rx, ry};
};

int main()
{

    double grid_size  = 2.0;
    double robot_size = 1.0;

    vector<double> ox, oy, gx, gy, sx, sy;
    sx.push_back(10.0);
    sy.push_back(10.0);
    gx.push_back(50.0);
    gy.push_back(50.0);

    // add edges
    for (double i = 0; i < 60; i++)
    {
        ox.push_back(i);
        oy.push_back(60.0);
    }
    for (double i = 0; i < 60; i++)
    {
        ox.push_back(60.0);
        oy.push_back(i);
    }
    for (double i = 0; i < 61; i++)
    {
        ox.push_back(i);
        oy.push_back(60.0);
    }
    for (double i = 0; i < 61; i++)
    {
        ox.push_back(0.0);
        oy.push_back(i);
    }
    for (double i = 0; i < 40; i++)
    {
        ox.push_back(20.0);
        oy.push_back(i);
    }
    for (double i = 0; i < 40; i++)
    {
        ox.push_back(40.0);
        oy.push_back(60.0 - i);
    }
    plt::backend("TkAgg");   // in case the segfault when exits
    plt::plot(ox, oy, ".k");
    plt::plot(sx, sy, "og");
    plt::plot(gx, gy, "xb");
    plt::grid(true);
    plt::axis("equal");

    std::vector<double> rx;
    std::vector<double> ry;
    std::tie(rx, ry) = breadth_first_search(sx, sy, gx, gy, ox, oy, grid_size, robot_size);
    plt::plot(rx, ry, "-r");
    plt::pause(0.001);
    plt::show();
    return 0;
}

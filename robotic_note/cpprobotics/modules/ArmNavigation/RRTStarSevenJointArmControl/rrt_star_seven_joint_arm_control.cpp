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
#include <random>
#include <vector>

#include "NLinkArm3d.hpp"

// Define a macro for positive infinity
#define FLOAT_INF std::numeric_limits<float>::infinity()

/**
 * @brief global parameters
 *
 */
bool SHOW_ANIMATION(true);
bool VERBOSE(false);
double EPS(1e-6);

/**
 * @brief class Node for RRT-star pathplanning
 *
 */
class Node
{
public:
    Eigen::VectorXd x_;                     // current node joint space configuration
    std::vector<Eigen::VectorXd> path_x_;   // path joint space configuration
    double cost_;                           // cost of current node

    std::shared_ptr<Node> parent_;   // parent node

public:
    Node(Eigen::VectorXd x)
    {
        this->x_ = x;
    }

    ~Node(){};
};

typedef std::shared_ptr<Node> Node_ptr;

/**
 * @brief RRT-Star for NLinkArm3d path planning
 *
 */
class RRTStarArm
{
public:
    Node_ptr start_node_;               // start joint space configuration, start_size == goal_size
    Node_ptr goal_node_;                // goal joint space configuration
    std::vector<Node_ptr> node_list_;   // list of node

    int dimension_;                                    // dimension of joint space configuration
    std::shared_ptr<NLinkArm3d> robot_;                // NLinkArm3d robot object
    std::vector<std::vector<double>> obstacle_list_;   // size: n X 4, [x, y, z, size], sphere

    double min_rand_;              // min_rand
    double max_rand_;              // max_rand
    double expand_dis_;            // expand distance
    double path_resolution_;       // path resolution
    int goal_sample_rate_;         // goal sample rate
    int max_iter_;                 // max iteration
    double connect_circle_dist_;   // connect circle distance

    bool verbose_;          // show verbose info
    bool show_animation_;   // show animation

public:
    /**
     * @brief Construct a new RRTStarArm object
     *
     * @param start: start joint space configuration
     * @param goal: goal joint space configuration, start_size == goal_size
     * @param robot: NLinkArm3d robot object
     * @param obstacle_list: size: n X 4, [x, y, z, size], sphere
     * @param rand_area: [min, max]
     * @param expand_dis: expand distance
     * @param path_resolution: path resolution
     * @param goal_sample_rate: goal sample rate
     * @param max_iter: max iteration
     * @param connect_circle_dist: connect circle distance
     * @param verbose: show verbose info
     * @param show_animation: show animation
     */
    RRTStarArm(
        Eigen::VectorXd start,
        Eigen::VectorXd goal,
        std::shared_ptr<NLinkArm3d> robot,
        std::vector<std::vector<double>> obstacle_list,
        std::vector<double> rand_area,
        double expand_dis = .30,
        double path_resolution = .1,
        int goal_sample_rate = 20,
        int max_iter = 300,
        double connect_circle_dist = 50.0,
        bool verbose = false,
        bool show_animation = false)
    {
        this->start_node_ = std::make_shared<Node>(start);
        this->goal_node_ = std::make_shared<Node>(goal);
        assert(this->start_node_->x_.size() == this->goal_node_->x_.size());
        this->dimension_ = start.size();
        this->node_list_.clear();

        this->robot_ = robot;
        this->obstacle_list_ = obstacle_list;

        assert(rand_area.size() == 2);
        this->min_rand_ = rand_area[0];
        this->max_rand_ = rand_area[1];

        this->expand_dis_ = expand_dis;
        this->path_resolution_ = path_resolution;
        this->goal_sample_rate_ = goal_sample_rate;
        this->max_iter_ = max_iter;
        this->connect_circle_dist_ = connect_circle_dist;

        this->verbose_ = verbose;
        this->show_animation_ = show_animation;
    }

    /**
     * @brief Destroy the RRTStarArm object
     *
     */
    ~RRTStarArm(){};

    /**
     * @brief rrt-star path planning
     * @param animation: flag for animation on or off
     * @param search_until_max_iter: search until max iteration for path improving or not
     */
    std::vector<Eigen::VectorXd> planning(bool animation = false, bool search_until_max_iter = false)
    {
        // add start node
        this->node_list_.clear();
        this->node_list_.push_back(this->start_node_);

        // rrt-star iteration
        for (size_t i = 0; i < this->max_iter_; i++)
        {
            if (this->verbose_)
            {
                std::cout << "Iter: " << i << ", number of nodes: " << this->node_list_.size() << "\n";
            }

            // get a random node
            Node_ptr rand_node = get_random_node();

            // get the nearst node index from current node list to random node
            int nearst_ind = get_nearest_node_index(this->node_list_, rand_node);

            // get steering new_node to rand_node
            Node_ptr new_node = steer(this->node_list_[nearst_ind], rand_node, this->expand_dis_);

            // check collision, choose better node to connect to new node as parent, and rewire the path
            // this part is important for RRT-Star (rechoose parent node and rewire)
            if (check_collision(new_node, *this->robot_, this->obstacle_list_))
            {
                std::vector<int> near_inds = find_near_nodes(new_node);

                // this new_node may have better parent node
                new_node = choose_parent(new_node, near_inds);
                if (new_node != nullptr)
                {
                    this->node_list_.push_back(new_node);
                    rewire(new_node, near_inds);
                }
            }

            if (!search_until_max_iter && new_node != nullptr)
            {
                int last_index = search_best_goal_node();
                if (last_index != -1)
                {
                    return generate_final_course(last_index);
                }
            }
        }

        // reach max iteration
        std::cout << "reached max iteration\n";
        int last_index = search_best_goal_node();
        if (last_index != -1)
        {
            return generate_final_course(last_index);
        }

        // didn't find path
        std::cout << "didn't find path\n";
        std::vector<Eigen::VectorXd> path;
        return path;
    }

    /**
     * @brief choose parent of the new_node
     *
     * @param new_node
     * @param near_inds
     * @return Node_ptr
     */
    Node_ptr choose_parent(Node_ptr new_node, std::vector<int> near_inds)
    {
        // check near_inds, if there is no near index, return nullptr
        if (near_inds.size() == 0)
        {
            return nullptr;
        }

        // search nearst cost in near_inds
        double cost;
        double min_cost = FLOAT_INF;
        int min_ind = -1;
        for (auto& i : near_inds)
        {
            Node_ptr near_node = this->node_list_[i];
            Node_ptr t_node = steer(near_node, new_node);
            if (check_collision(t_node, *this->robot_, this->obstacle_list_))
            {
                cost = calc_new_cost(near_node, new_node);
            }
            else
            {
                cost = FLOAT_INF;
            }

            if (cost < min_cost)
            {
                min_cost = cost;
                min_ind = i;
            }
        }

        // check min_cost
        if (min_cost == FLOAT_INF)
        {
            std::cout << "There is no good path, min_cost is INF.\n";
            return nullptr;
        }

        Node_ptr t_new_node = steer(this->node_list_[min_ind], new_node);
        t_new_node->parent_ = this->node_list_[min_ind];
        t_new_node->cost_ = min_cost;

        return t_new_node;
    }

    /**
     * @brief search best goal node
     * find the nearst node to goal node and collision free
     * @return int
     */
    int search_best_goal_node()
    {
        double dist_to_goal_list;
        std::vector<int> safe_goal_inds;

        for (size_t i = 0; i < this->node_list_.size(); i++)
        {
            dist_to_goal_list = calc_dist_to_goal(this->node_list_[i]->x_);

            if (dist_to_goal_list <= this->expand_dis_)
            {
                Node_ptr t_node = steer(this->node_list_[i], this->goal_node_);
                if (check_collision(t_node, *this->robot_, this->obstacle_list_))
                {
                    safe_goal_inds.push_back(i);
                }
            }
        }

        if (safe_goal_inds.size() == 0)
        {
            return -1;
        }

        double min_cost = FLOAT_INF;
        int min_cost_ind = -1;
        for (auto& i : safe_goal_inds)
        {
            if (this->node_list_[i]->cost_ < min_cost)
            {
                min_cost = this->node_list_[i]->cost_;
                min_cost_ind = i;
            }
        }

        return min_cost_ind;
    }

    /**
     * @brief find near nodes within dist, return the index
     *
     * @param new_node
     * @return std::vector<int>
     */
    std::vector<int> find_near_nodes(Node_ptr new_node)
    {
        // choose dist r
        int nnode = this->node_list_.size() + 1;
        double r = this->connect_circle_dist_ * std::sqrt(std::log(nnode) / nnode);
        r = std::min(r, this->expand_dis_);

        std::vector<int> near_inds;
        for (size_t i = 0; i < this->node_list_.size(); i++)
        {
            double dist = (this->node_list_[i]->x_ - new_node->x_).dot(this->node_list_[i]->x_ - new_node->x_);
            if (dist <= r * r)
            {
                near_inds.push_back(i);
            }
        }

        return near_inds;
    }

    /**
     * @brief rewire for new_node
     * Consider if the near_nodes should be reconnected to the new_node
     * so that the new_node will not only be added at the end of the node tree,
     * but also possible to be inserted inside the node tree
     * @param new_node
     * @param near_inds
     */
    void rewire(Node_ptr new_node, std::vector<int> near_inds)
    {
        for (auto& i : near_inds)
        {
            Node_ptr near_node = this->node_list_[i];
            Node_ptr edge_node = steer(new_node, near_node);
            edge_node->cost_ = calc_new_cost(new_node, near_node);

            bool no_collision = check_collision(edge_node, *this->robot_, this->obstacle_list_);
            bool improved_cost = near_node->cost_ > edge_node->cost_;

            if (no_collision && improved_cost)
            {
                this->node_list_[i] = edge_node;
                propagate_cost_to_leaves(new_node);
            }
        }
    }

    /**
     * @brief calc new cost
     *
     * @param from_node
     * @param to_node
     * @return double
     */
    double calc_new_cost(Node_ptr from_node, Node_ptr to_node)
    {
        double d, phi, theta;
        calc_distance_and_angle(from_node, to_node, d, phi, theta);
        return from_node->cost_ + d;
    }

    /**
     * @brief propagate cost to leaves recursively
     *
     * @param parent_node
     */
    void propagate_cost_to_leaves(Node_ptr parent_node)
    {
        for (auto& node : this->node_list_)
        {
            // check if parent nodes are the same
            if (node->parent_ == parent_node)
            {
                node->cost_ = this->calc_new_cost(parent_node, node);
                propagate_cost_to_leaves(node);
            }
        }
    }

    /**
     * @brief generate final reversed path
     *
     * @param goal_ind
     * @return std::vector<Eigen::VectorXd>
     */
    std::vector<Eigen::VectorXd> generate_final_course(int goal_ind)
    {
        std::vector<Eigen::VectorXd> path = {this->goal_node_->x_};
        Node_ptr node = this->node_list_[goal_ind];
        while (node->parent_ != nullptr)
        {
            path.push_back(node->x_);
            node = node->parent_;
        }
        path.push_back(node->x_);

        std::reverse(path.begin(), path.end());

        return path;
    }

    /**
     * @brief calc dist between current joint configuration and goal
     *
     * @param x
     * @return double
     */
    double calc_dist_to_goal(Eigen::VectorXd& x)
    {
        double dist = (x - this->goal_node_->x_).norm();
        return dist;
    }

    /**
     * @brief Get the random node object
     *
     * @return Node_ptr
     */
    Node_ptr get_random_node()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(this->min_rand_, this->max_rand_);

        if (std::rand() % 100 > this->goal_sample_rate_)
        {
            Eigen::VectorXd rand_x(this->dimension_);
            for (size_t i = 0; i < this->dimension_; i++)
            {
                rand_x[i] = dist(gen);
            }

            Node_ptr rand_node = std::make_shared<Node>(rand_x);
            return rand_node;
        }
        else
        {
            Node_ptr rand_node = std::make_shared<Node>(this->goal_node_->x_);
            return rand_node;
        }
    }

    /**
     * @brief get a steered new_node from from_node to to_node
     *
     * @param from_node
     * @param to_node
     * @param extend_length
     * @return Node_ptr
     */
    Node_ptr steer(Node_ptr& from_node, Node_ptr& to_node, float extend_length = FLOAT_INF)
    {
        // init new_node
        Node_ptr new_node = std::make_shared<Node>(from_node->x_);

        // calc d, phi and theta to to_node
        double d, phi, theta;
        calc_distance_and_angle(new_node, to_node, d, phi, theta);

        // init path_x to new_node
        new_node->path_x_.clear();
        new_node->path_x_.push_back(new_node->x_);

        // update extend_length
        if (extend_length > d)
        {
            extend_length = d;
        }

        // get n_expand, discretize expand length by path resolution
        int n_expand = std::floor(extend_length / this->path_resolution_);

        // get unit vec from start to end
        Eigen::VectorXd start_x = from_node->x_;
        Eigen::VectorXd end_x = to_node->x_;
        Eigen::VectorXd v = end_x - start_x;
        Eigen::VectorXd u = v / std::sqrt(v.dot(v));

        // add every step to path_x
        for (size_t i = 0; i < n_expand; i++)
        {
            new_node->x_.array() += u.array() * this->path_resolution_;
            new_node->path_x_.push_back(new_node->x_);
        }

        // update d, phi, theta with updated new_node
        // add to_node joint configuration to path_x
        calc_distance_and_angle(new_node, to_node, d, phi, theta);
        if (d <= this->path_resolution_)
        {
            new_node->path_x_.push_back(to_node->x_);
        }

        // update new_node parent
        new_node->parent_ = from_node;

        return new_node;
    }

    /**
     * @brief Get the nearest node index
     *
     * @param node_list
     * @param rand_node
     * @return int: nearest node index
     */
    static int get_nearest_node_index(std::vector<Node_ptr>& node_list, Node_ptr& rand_node)
    {
        double min_dist = FLOAT_INF;
        int min_dist_index = -1;
        for (size_t i = 0; i < node_list.size(); i++)
        {
            // calc joint space dist
            double dist = (rand_node->x_ - node_list[i]->x_).dot(rand_node->x_ - node_list[i]->x_);

            // update min dist and index
            if (dist < min_dist)
            {
                min_dist = dist;
                min_dist_index = i;
            }
        }

        return min_dist_index;
    }

    /**
     * @brief calc distance and angle from node to node in joint space
     *
     * @param from_node
     * @param to_node
     * @return d
     * @return phi
     * @return theta
     */
    static void calc_distance_and_angle(
        Node_ptr& from_node,
        Node_ptr& to_node,
        double& d_,
        double& phi_,
        double& theta_)
    {
        // only consider joint 0 - 2
        double dx = to_node->x_[0] - from_node->x_[0];
        double dy = to_node->x_[1] - from_node->x_[1];
        double dz = to_node->x_[2] - from_node->x_[2];

        double d = (to_node->x_ - from_node->x_).dot(to_node->x_ - from_node->x_);
        d = std::sqrt(d);

        double phi = std::atan2(dy, dx);
        double theta = std::atan2(std::hypot(dx, dy), dz);

        d_ = d;
        phi_ = phi;
        theta_ = theta;
    }

    /**
     * @brief check collision between robot and obstacle
     *
     * @param node
     * @param robot
     * @param obstacle_list
     * @return true: collision free
     * @return false: collision
     */
    static bool check_collision(Node_ptr node, NLinkArm3d& robot, const std::vector<std::vector<double>>& obstacle_list)
    {
        // iterate all obstacles
        for (size_t i = 0; i < obstacle_list.size(); i++)
        {
            // get obstacle param
            double ox = obstacle_list[i][0];
            double oy = obstacle_list[i][1];
            double oz = obstacle_list[i][2];
            double osize = obstacle_list[i][3];

            // check node path joint configuration
            for (size_t j = 0; j < node->path_x_.size(); j++)
            {
                // get arm point in this joint configuration
                Eigen::VectorXd x = node->path_x_[j];
                Eigen::VectorXd x_list;
                Eigen::VectorXd y_list;
                Eigen::VectorXd z_list;
                robot.get_points(x, x_list, y_list, z_list);

                // get dist vector from arm point to obstacle
                Eigen::VectorXd dx_list = ox - x_list.array();
                Eigen::VectorXd dy_list = oy - y_list.array();
                Eigen::VectorXd dz_list = oz - z_list.array();
                Eigen::VectorXd d_list =
                    dx_list.cwiseProduct(dx_list) + dy_list.cwiseProduct(dy_list) + dz_list.cwiseProduct(dz_list);

                // check dist
                if (d_list.minCoeff() <= osize * osize)
                {
                    return false;   // collision
                }
            }
        }

        return true;   // collision free
    }
};

/**
 * @brief main process for rrt-star planning for seven-joint-arm control
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const* argv[])
{
    // create dh params for arm
    // theta for active joints, rotation at z axis
    // alpha, rotation at x axis
    // a, trans along x
    // d, trans along z
    std::vector<Eigen::Vector4d> dh_params_list = {
        {0., M_PI / 2.,  0.,      .333  },
        {0., -M_PI / 2., 0.,      0.    },
        {0., M_PI / 2.,  0.0825,  0.3160},
        {0., -M_PI / 2., -0.0825, 0.    },
        {0., M_PI / 2.,  0.,      0.3840},
        {0., M_PI / 2.,  0.088,   0.    },
        {0., 0.,         0.,      0.107 }
    };

    // create NLinkArm3d
    auto seven_joint_arm = std::make_shared<NLinkArm3d>(dh_params_list);

    // create obstacle_list {x, y, z, radius}
    std::vector<std::vector<double>> obstacle_list = {
        {-.3, -.3, .7, .1 },
        {.0,  -.3, .7, .1 },
        {.2,  -.1, .3, .15},
    };

    // create start and goal joint configuration
    Eigen::VectorXd start(seven_joint_arm->link_list_.size());
    Eigen::VectorXd goal(seven_joint_arm->link_list_.size());
    for (size_t i = 0; i < seven_joint_arm->link_list_.size(); i++)
    {
        start(i) = 0.0;
        goal(i) = 1.5;
    }

    // create rand_area
    // std::vector<double> rand_area = {0.0, 2.0};
    std::vector<double> rand_area = {0.0, 3.14};

    // create RRTStarArm object
    auto rrt_star_arm = RRTStarArm(
        start,
        goal,
        seven_joint_arm,
        obstacle_list,
        rand_area,
        0.3,
        0.1,
        20,
        200,
        50.0,
        VERBOSE,
        SHOW_ANIMATION);

    // run rrt-star plan
    while (true)
    {
        std::vector<Eigen::VectorXd> path = rrt_star_arm.planning(SHOW_ANIMATION);
        std::cout << "path_size: " << path.size() << "\n";
        if (path.size() == 0)
        {
            std::cout << "Can not find path\n";
            continue;
        }
        else
        {
            std::cout << "Found path!\n";

            if (SHOW_ANIMATION)
            {
                seven_joint_arm->plot_arm_obstacle_path(obstacle_list, start, goal, path);
            }

            break;
        }
    }

    return 0;
}

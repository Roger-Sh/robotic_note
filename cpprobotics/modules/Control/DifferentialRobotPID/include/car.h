#include <Eigen/Dense>
#include <iostream>
using namespace Eigen;

class Car
{
public:
    Car(double x, double y) :
        xx(MatrixXd(3, 1)),
        xx_dot(MatrixXd(3, 1)),
        wheel_speed(MatrixXd(2, 1)),
        b(10),
        r(5),
        car_dims(MatrixXd(5, 3)),
        car_points(MatrixXd(5, 3))
    {
        xx << x, y, 0;
        xx_dot << 0, 0, 0;
        wheel_speed << 0, 0;
        car_dims << -b, -b, 1, 0, -b, 1, b, 0, 1, 0, b, 1, -b, b, 1;
        get_transformed_pts();
    }
    void set_wheel_velocity(double lw_speed, double rw_speed)
    {
        wheel_speed << rw_speed, lw_speed;
        xx_dot = forward_kinematics();
    }
    void set_robot_velocity(double linear_velocity, double angular_velocity)
    {
        xx_dot << linear_velocity, 0, angular_velocity;
        wheel_speed = inverse_kinematics();
    }
    void update_state(double dt)
    {
        Matrix3d A;
        A << 1, 0, 0, 0, 1, 0, 0, 0, 1;
        MatrixXd B(3, 2);
        B << sin(xx(2, 0) + M_PI / 2) * dt, 0, cos(xx(2, 0) + M_PI / 2) * dt, 0, 0, dt;
        MatrixXd vel(2, 1);
        vel << xx_dot(0, 0), xx_dot(2, 0);
        xx = A * xx + B * vel;
    }
    void update(double dt)
    {
        wheel_speed = wheel_speed.cwiseMin(2);
        wheel_speed = wheel_speed.cwiseMax(-2);
        xx_dot      = forward_kinematics();
        update_state(dt);
        wheel_speed = inverse_kinematics();
    }
    std::tuple<MatrixXd, MatrixXd> get_state()
    {
        return std::make_tuple(xx, xx_dot);
    }
    MatrixXd forward_kinematics()
    {
        Matrix<double, 3, 2> kine_mat;
        kine_mat << r / 2, r / 2, 0, 0, r / (2 * b), -r / (2 * b);
        return kine_mat * wheel_speed;
    }
    MatrixXd inverse_kinematics()
    {
        Matrix<double, 2, 3> ikine_mat;
        ikine_mat << 1 / r, 0, b / r, 1 / r, 0, -b / r;
        return ikine_mat * xx_dot;
    }
    void get_transformed_pts()
    {
        Matrix3d rot_mat;
        rot_mat << cos(xx(2, 0)), sin(xx(2, 0)), xx(0, 0), -sin(xx(2, 0)), cos(xx(2, 0)), xx(1, 0), 0, 0, 1;
        MatrixXd transformed_pts = (car_dims * rot_mat.transpose());
        car_points               = transformed_pts;   // Uncomment this line
    }
    MatrixXd get_points()
    {
        get_transformed_pts();
        return car_points;
    }

private:
    MatrixXd xx;
    MatrixXd xx_dot;
    MatrixXd wheel_speed;
    double b;
    double r;
    MatrixXd car_dims;
    MatrixXd car_points;
};

#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;

class KinematicModel
{
public:
    double x, y, psi, v, L, dt;

public:
    KinematicModel();

    KinematicModel(double x, double y, double psi, double v, double l, double dt);

    vector<double> getState();

    void updateState(double accel, double delta_f);

    vector<MatrixXd> stateSpace(double ref_delta, double ref_yaw);
};

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
#define PI 3.1415926

class MyReferencePath
{
public:
    vector<vector<double>> refer_path;
    vector<double> refer_x, refer_y;

public:
    MyReferencePath();
    vector<double> calcTrackError(vector<double> robot_state);
    double normalizeAngle(double angle);
};
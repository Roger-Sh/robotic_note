#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    while (true) {
        std::vector<double> t = iota(0, pi / 50, 10 * pi);
        std::vector<double> st = transform(t, [](auto x) { return sin(x); });
        std::vector<double> ct = transform(t, [](auto x) { return cos(x); });
        auto l = plot3(st, ct, t);

        std::cout << "before show\n";
        show();
        std::cout << "after show\n";
    }

    return 0;
}
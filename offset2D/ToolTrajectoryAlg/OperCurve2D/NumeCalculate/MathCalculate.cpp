#include "MathCalculate.h"
void MathCalculate::PreTest()
{
    double x = 116.30298076541203695;
    x = x * M_PI / 180;
    double staVal = std::cos(x);
    vector<double>vErr;
    int n = 1000;
    double val = x;
    for (int i = 0; i < n; ++i) {
        double x1 = std::sqrt(std::sin(val) * std::sin(val));
        double x2 = std::sqrt(std::tan(val) * std::tan(val));
        double x3 = std::sqrt(std::sqrt(std::sqrt((x1 / x2) * (x1 / x2)) *
                                        std::sqrt((x1 / x2) * (x1 / x2))) *
                              std::sqrt(std::sqrt((x1 / x2) * (x1 / x2)) *
                                        std::sqrt((x1 / x2) * (x1 / x2))));
        double x4 = std::acos(x3);
        double x5 = std::cos(x4);
        vErr.push_back(abs(staVal) - abs(x5));
        val = std::acos(x5);
    }
    double val_1 = staVal;
    for (int i = 0; i < n; ++i) {
        val_1 = std::acos(val_1);
        double x1 = std::cos(val_1);
        val_1 = x1;
    }
    double pre1 = staVal - val;
    double pre2 = staVal - val_1;
}
double MathCalculate::CalSqrt(double x)
{
    double err = 1e-16;
    long double next = x;
    while(std::abs(next - x / next) > err) {
        next = (next + x / next) / 2;
    }
    return next;
}

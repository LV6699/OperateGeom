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
bool MathCalculate::IsNumericInRange(double left,double right,
                                     double val,double pre)
{
    return ((val + pre) >= left) && ((val - pre) <= right);
}
bool MathCalculate::IsNumericEqual(double val1, double val2,
                                   double pre)
{
    return std::abs(val1 - val2) <= pre;
}
double MathCalculate::VectorExtremum(const vector<double>&v,bool isMax)
{
    if(isMax){
        auto max_iter = std::max_element(v.begin(), v.end());        return *max_iter;
    }
    auto min_iter = std::min_element(v.begin(), v.end());
    return *min_iter;
}
///根据弓高误差计算圆离散成线段的对应圆角度
double MathCalculate::AngleBaseBowHeight(double R,double h)
{
    if(h >= R - PreErr5_6)
        return 360;
    return std::acos(1 - (h / R)) * 180 / M_PI;
}
bool MathCalculate::IsValueIn2DArray(const vector<int>&v,int value)
{
    for(auto &d : v)
        if(d == value)
            return true;
    return false;
}
bool MathCalculate::IsValueIn3DArray(const vector<vector<int>>&vv, int value)
{
    for(auto &v : vv)
        if(IsValueIn2DArray(v,value))
            return true;
    return false;
}
double MathCalculate::ArrayValueAverage(const vector<double>&v)
{
    if(v.empty())
        return 0;
    double s = 0;
    for(auto& d : v)
        s += d;
    return s / v.size();
}
int MathCalculate::BezierCoefficient(int n, int k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    return BezierCoefficient(n - 1, k - 1) +
            BezierCoefficient(n - 1, k);
}
double MathCalculate::ArcLenthTranslateAngle(double arcLen,double R)
{
    return (360 * arcLen) / (2 * M_PI * R);
}
double MathCalculate::NumericalDecimalPlace(double value,int number)
{
    double factor = std::pow(10.0, number);
    return std::round(value * factor) / factor;
}
#ifndef MATHCALCULATE_H
#define MATHCALCULATE_H
#include "../../CommonFile/Header.h"
/// template <typename T>
class MathCalculate {
  public:
    MathCalculate() {}
    void PreTest();
    double CalSqrt(double x);
    bool IsLegalValue(double value)
    {
        if (value == 0)
            return true;
        if (std::isinf(value))
            return false;
        if (std::isnan(value))
            return false;
#ifdef _WIN32
        if (!_finite(value)) {
            return false;
        }
#else
        if (!finite(value))
            return false;
#endif
        if (std::abs(value) <= PreErr_80)
            return false;
        return true;
    }
    bool IsNumericInRange(double left, double right, double val, double pre)
    {
        return ((val + pre) >= left) && ((val - pre) <= right);
    }
    bool IsNumericEqual(double val1, double val2, double pre)
    {
        return std::abs(val1 - val2) <= pre;
    }
    double VectorExtremum(const vector<double> &v, bool isMax)
    {
        if (isMax) {
            auto max_iter = std::max_element(v.begin(), v.end());
            return *max_iter;
        }
        auto min_iter = std::min_element(v.begin(), v.end());
        return *min_iter;
    }
    /// 根据弓高误差计算圆离散成线段的对应圆角度
    double AngleBaseBowHeight(double R, double h)
    {
        if (h >= R - PreErr5_6)
            return 360;
        return std::acos(1 - (h / R)) * 180 / M_PI;
    }
    bool IsValueIn2DArray(const vector<int> &v, int value)
    {
        for (auto &d : v)
            if (d == value)
                return true;
        return false;    }
    bool IsValueIn3DArray(const vector<vector<int>> &vv, int value)
    {
        for (auto &v : vv) {
            if (IsValueIn2DArray(v, value))
                return true;
        }
        return false;
    }
    double ArrayValueAverage(const vector<double> &v)
    {
        if (v.empty())
            return 0;
        double s = 0;
        for (auto &d : v) {
            s += d;
        }
        return s / v.size();
    }
    int BezierCoefficient(int n, int k)
    {
        if (k > n)
            return 0;
        if (k == 0 || k == n)
            return 1;
        return BezierCoefficient(n - 1, k - 1) + BezierCoefficient(n - 1, k);
    }
    double ArcLenthToAngle(double arcLen, double R)
    {
        return (360 * arcLen) / (2 * M_PI * R);
    }
    double NumericalDecimalPlace(double value, int number)
    {
        double factor = std::pow(10.0, number);
        return std::round(value * factor) / factor;
    }
};
#endif // MATHCALCULATE_H
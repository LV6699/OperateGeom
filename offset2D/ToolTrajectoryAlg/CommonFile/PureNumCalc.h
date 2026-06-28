#ifndef PURENUM_H
#define PURENUM_H
#include <cmath>
#include <algorithm>
#include <vector>
#include "NumPreErr.h"
namespace PureNume {
static bool IsLegalNum(double v){
    if (v == 0){return true;}
    if (std::isinf(v)){return false;}
    if (std::isnan(v)){return false;}
#ifdef _WIN32
    if (!_finite(v)) {return false;}
#else
    if (!finite(v)){return false;}
#endif
    if (std::abs(v) <= PreErr_80){return false;}
    return true;
}
static bool IsEqualNum(double v1,double v2,double e){
    return std::abs(v1-v2) <= e;
}
static size_t PreId(size_t l,size_t i) {
    if(l <= 0){return 0;}
    if(i <= 0){return l - 1;}
    return i - 1;
}
static size_t NexId(size_t l,size_t i){
    if(l <= 0){return 0;}
    if(i >= l - 1){return 0;}
    return i + 1;
}
static double CircleLenToAgl(double l, double R)
{
    double agl = (360 * l) / (2 * Num_PI * R);
    return agl;
}
static double NumDecimalPlace(double value, int number)
{
    double factor = std::pow(10.0, number);
    return std::round(value * factor) / factor;
}
/// 根据弓高误差计算圆离散成线段的对应圆角度
static double AglBaseBowH(double R, double h) 
{
    if(h >= R - PreErr5_6){return 360;}
    return std::acos(1 - (h / R)) * 180 / Num_PI;
}
static double TwoAglIncludedAgl(const double& sAgl,const double& eAgl,bool isCW)
{
    double agl = 0;
    if(isCW){
        if(sAgl < eAgl){
            agl = sAgl + 360.0 - eAgl;
        }else{
            agl = sAgl - eAgl;
        }
    }else{
        if(sAgl > eAgl){
            agl = eAgl + 360.0 - sAgl;
        }else{
            agl = eAgl - sAgl;
        }
    }
    if(agl >= 360.0){agl = 0;}
    return agl;
}
static double ArcShiftAngle(double sAng,double eAng,double chAng,bool isCW)
{
    double agl = 0;
    if(isCW){
        if(sAng < eAng){
            if(sAng >= (360.0 - eAng)){
                agl = sAng - chAng;
            }else{
                agl = eAng + chAng;
            }
        }else{
            agl = sAng - chAng;
        }
    }
    else{
        if(eAng < sAng){
            if(eAng >= (360.0 - sAng)){
                agl = eAng - chAng;
            }else{
                agl = sAng + chAng;
            }
        }
        else{
            agl = sAng + chAng;
        }
    }
    return agl;
}
static bool AglIsBetweenAgls(double sAgl,double eAgl,
                             bool isCW,double theta,double pre)
{
    bool isIn = false;
    if(!isCW){
        if(sAgl > eAgl){
            if((theta > eAgl) && (theta < sAgl)){
                isIn = false;
            }else{
                isIn = true;
            }
        }else{
            if((theta + pre >= sAgl) &&
                    (theta - pre <= eAgl)){
                isIn = true;
            }else{
                isIn = false;
            }
        }
    }else{
        if(sAgl < eAgl){
            if((theta > sAgl) && (theta < eAgl)){
                isIn = false;
            }else{
                isIn = true;
            }
        }else{
            if((theta - pre <= sAgl) &&
                    (theta + pre >= eAgl)){
                isIn = true;
            }else{
                isIn = false;
            }
        }
    }
    return isIn;
}
template <typename T>
static void DeleteIndexElems(const std::vector<size_t>& ids,std::vector<T>& v) {
    if(v.empty() || ids.empty()){return;}
    std::vector<T> v_;
    for(size_t i = 0;i < v.size();++i){
        bool is = false;
        for(auto& d : ids){
            if(d == i){is = true;break;}
        }
        if(!is){v_.emplace_back(v[i]);}
    }
    v = v_;
}
template <typename T>
static bool IsIn2DArray(const T& o,const std::vector<T>& v) {
    for(auto& d : v){if(o == d){return true;}}
    return false;
}
template <typename T>
static bool IsInArray(const T& o,const std::vector<T>& v) {
    for(auto& d : v){if(o == d){return true;}}
    return false;
}
template <typename T>
static void NumRemoveDuplicates(std::vector<T>& v) {
    std::vector<T>result;
    /*std::unordered_set<T>seen;
    result.reserve(v.size());
    for (const auto& d : v) {
        if (seen.insert(d).second) {
            result.push_back(d);
        }
    }*/
}
template <typename T>
static T ArrayAverage1D(const std::vector<T>& v) {
    T s = 0;
    for(auto& d : v){s += d;}
    return s / v.size();
}
//计算角度误差为a 时的正弦值平方,比如角度误差a 为3,转换为弧度a_rad=3.0*PI/180.0=0.0523599rad,
//计算正弦值a_sin=sin(a_rad)=0.05233596,计算平方e=a_sin*a_sin;
static double AglToSinSquare(double a)
{
    double a_rad = a * _RadAlg;
    double a_sin = std::sin(a_rad);
    return a_sin * a_sin;
}
}
namespace pnum = PureNume;
#endif // PURENUM_H

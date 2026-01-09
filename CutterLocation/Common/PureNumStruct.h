#ifndef PURENUMSTRUCT_H
#define PURENUMSTRUCT_H

#include "../../OffsetStruct/CommonFile/NumPreErr.h"

namespace PureNum{

static bool LeftInc(const double& v0,const double& v1,const double& v2)
{return v0 > v1 && v1 > v2;}
static bool LeftInc1(const double& v0,const double& v1,const double& v2)
{return v0 >= v1 && v1 > v2;}
static bool LeftInc2(const double& v0,const double& v1,const double& v2)
{return v0 > v1 && v1 >= v2;}
static bool LeftInc12(const double& v0,const double& v1,const double& v2)
{return v0 >= v1 && v1 >= v2;}
static bool RightInc(const double& v0,const double& v1,const double& v2)
{return v0 < v1 && v1 < v2;}
static bool RightInc1(const double& v0,const double& v1,const double& v2)
{return v0 <= v1 && v1 < v2;}
static bool RightInc2(const double& v0,const double& v1,const double& v2)
{return v0 < v1 && v1 <= v2;}
static bool RightInc12(const double& v0,const double& v1,const double& v2)
{return v0 <= v1 && v1 <= v2;}

static void ResetZ(double cur,double& t,double& res){
    if(cur > t){
        t = cur;
        if(cur > res){
            res = cur;
        }
    }
}
static bool IsInRange(double v,double min,double max,double e){
    return v + e >= min && v - e <= max;
}

static double Square(double v) {return v * v;}

static double IsNear(double v1,double v2,double e){return std::abs(v2-v1)<=e;}

static double Max(double v1,double v2){return v1 > v2 ? v1 : v2;}
static double Min(double v1,double v2){return v1 < v2 ? v1 : v2;}
static double ThreeMax(double v1,double v2,double v3){
    double max = v1 > v2 ? v1 : v2;
    return max > v3 ? max : v3;
}
static double ThreeMin(double v1,double v2,double v3){
    double min = v1 < v2 ? v1 : v2;
    return min < v3 ? min : v3;
}

}

namespace pnum = PureNum;


#endif // PURENUMSTRUCT_H

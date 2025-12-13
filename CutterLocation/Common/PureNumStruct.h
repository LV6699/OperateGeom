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



}

namespace pnum = PureNum;


#endif // PURENUMSTRUCT_H

#ifndef BASECALC_H
#define BASECALC_H

#include "../Common/ViewTool.h"

class BaseCalc
{
public:
    BaseCalc(){}



    static grm::ProjRes GetProjPoint(const oft::Point& p,
                                     const oft::Point& sp,
                                     const oft::Point& ep){
        if(p.IsSameCoord2D(sp,PreErr_10)){
            return grm::ProjRes(sp,0);
        }
        auto v = ep - sp;
        auto u = p - sp;
        double num = v.Dot2D(u) / v.Dot2D(v);
        auto pr = sp + v*num;
        return grm::ProjRes(pr,num);
    }

    static bool InSegRangle(const oft::Point& p,const oft::Point& sp,
                            const oft::Point& ep,double e){

    }



};

#endif // BASECALC_H

#ifndef BASECALC_H
#define BASECALC_H

#include "../Common/ViewTool.h"

class BaseCalc
{
public:
    BaseCalc(){}



    static grm::ProjRes GetProjPoint(const ofts::Point& p,
                                     const ofts::Point& sp,
                                     const ofts::Point& ep){
        if(p.IsSameCoord2D(sp,PreErr_10)){
            return grm::ProjRes(sp,0);
        }
        auto v = ep - sp;
        auto u = p - sp;
        double num = v.Dot2D(u) / v.Dot2D(v);
        auto pr = sp + v*num;
        return grm::ProjRes(pr,num);
    }

    static bool InSegRangle(const ofts::Point& p,const ofts::Point& sp,
                            const ofts::Point& ep,double e){

    }

    static double PtToSegDistSqua2D(const ofts::Point& p,
                                    const ofts::Point& sp,
                                    const ofts::Point& ep) {
      auto v = ep - sp;
      auto v1 = p - sp;
      double lSq = v.LengthSquare2D();
      if (lSq < PreErr_20) {
        return p.DistSquare2D(sp);
      }
      double t = v1.Dot2D(v) / lSq;
      if (t < PreErr_12) {
        return p.DistSquare2D(sp);
      } else if (t >= 1.0) {
        return p.DistSquare2D(ep);
      }
      auto pro = sp + v * t;
      return p.DistSquare2D(pro);
    }
};

#endif // BASECALC_H

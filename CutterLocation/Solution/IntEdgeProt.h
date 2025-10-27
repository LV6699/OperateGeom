#ifndef INTEDGEPROT_H
#define INTEDGEPROT_H

#include "OperTriaCl.h"
#pragma optimize("", off)
#pragma GCC optimize ("o0")

namespace grm{
class IntEdgeProt
{
public:
    IntEdgeProt(){}

    static double PlaneEndToolCase(const DefTool& T,const oft::Point& sp,
                                   const oft::Point& ep,const oft::Point& p)
    {
        double RR = T.R()*T.R();
        double yp = p.Y()*p.Y();
        if(RR < yp){
            return Min_Val; ///无交点
        }
        double H = ep.Z() - sp.Z();
        double l = sp.Distance2D(ep);
        double K = H / l;
        double xf = -std::sqrt(RR - yp);
        double zp = sp.Z() + K*(p.X()-xf);
        if(zp > 100){
            int tem = 1;
        }
        return zp;
    }

    static double BallEndToolCase(const DefTool& T,const oft::Point& sp,
                                  const oft::Point& ep,const oft::Point& p)
    {
        double H = ep.Z() - sp.Z();
        double l = sp.Distance3D(ep);
        double len = sp.Distance2D(ep);

        return 0;

    }

    static double  EdgeProtectCase(const DefTool& T,const oft::Point& p0,
                                   const oft::Point& p1,const oft::Point& p)
    {

        if(std::abs(p0.X() - p1.X()) < PreErr_8 &&
                std::abs(p0.Y() - p1.Y()) < PreErr_8){  ///处理竖直边
            return Min_Val;
        }
        auto sp = p0,ep = p1;
        if(sp.Z() > ep.Z()){std::swap(sp,ep);}
        double z = Min_Val;

        switch (T._type) {
        case ToolType::PlaneEnd:{
            z = PlaneEndToolCase(T,sp,ep,p);
            break;
        }
        case ToolType::BallNoseEnd:{
            break;
        }
        case ToolType::RoundNoseEnd:{

            break;
        }
        default: break;
        }



        return z;

    }
    static double  TriangleEdgeCase(const DefTool& T,const Triangle& t,
                                    const oft::Point& p)
    {
        double z0 = EdgeProtectCase(T,t.P0(),t.P1(),p);
        double z1 = EdgeProtectCase(T,t.P1(),t.P2(),p);
        double z2 = EdgeProtectCase(T,t.P2(),t.P0(),p);
        return LimVal::ThreeMax(z0,z1,z2);

    }




};
}

#endif // INTEDGEPROT_H

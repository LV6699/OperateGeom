#ifndef TOOLPARAM_H
#define TOOLPARAM_H

#include "../../CommonFile/ModelStruct.h"

namespace grm {

class ToolParam
{
public:
    ToolParam(){}
    
    static void SetToolBaseParam(double R,double r,double agl,DefTool& T)
    {
        T._R = R;
        T._RR = R*R;
        switch (T._type) {
        case ToolType::PlaneEnd: {
            break;
        }
        case ToolType::BallNoseEnd: {
            T._cr = r;
            T._rr = r*r;
            T._R_r = R - r;
            break;
        }
        case ToolType::RoundNoseEnd: {
            T._cr = r;
            T._rr = r*r;
            T._R_r = R - r;
            break;
        }
        case ToolType::TaperEnd: {
            T._br = r;
            T._br2 = r*r;
            T._R_br = R - r;
            T._taper_agl = agl;
            T._taper_k = std::tan(agl*_RadAlg);
            T._h = T._R_br / T._taper_k;
            T._taper_l = T._R_br / std::tan(agl*_RadAlg);
            T._full_taper_l = T._R / std::tan(agl*_RadAlg);
            break;
        }
        case ToolType::TaperBall: {
            T._br = r;
            T._br2 = r*r;
            T._R_br = R - r;
            T._taper_agl = agl;
            T._taper_k = std::tan(agl*_RadAlg);
            T._h = T._R_br / T._taper_k;
            T._taper_l = T._R_br / std::sin(agl*_RadAlg);
            T._full_taper_l = T._R / std::sin(agl*_RadAlg);
            break;
        }
        case ToolType::V_Cutter: {
            T._R_br = R ;
            T._taper_agl = agl;
            T._taper_k = std::tan(agl*_RadAlg);
            T._h = T._R_br / T._taper_k;
            T._taper_l = T._R_br / std::sin(agl*_RadAlg);
            T._full_taper_l = T._R / std::sin(agl*_RadAlg);
            break;
        }
        default:
            throw "T type error!";
        }
    }
    
    
    
};

}

#endif // TOOLPARAM_H

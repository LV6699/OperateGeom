#ifndef TOOLPARAM_H
#define TOOLPARAM_H

#include "../../CommonFile/ModelStruct.h"

namespace grm {

class ToolParam
{
public:
    ToolParam(){}
    
    static void SetToolBaseParam(double R,double r,double agl,DefTool& tool)
    {
        tool._R = R;
        tool._RR = R*R;
        switch (tool._type) {
        case ToolType::PlaneEnd: {
            break;
        }
        case ToolType::BallNoseEnd: {
            tool._cr = r;
            tool._rr = r*r;
            tool._R_r = R - r;
            break;
        }
        case ToolType::RoundNoseEnd: {
            tool._cr = r;
            tool._rr = r*r;
            tool._R_r = R - r;
            break;
        }
        case ToolType::TaperEnd: {
            tool._br = r;
            tool._br2 = r*r;
            tool._R_br = R - r;
            tool._taper_agl = agl;
            tool._taper_k = std::tan(agl*_RadAlg);
            tool._h = tool._R_br / tool._taper_k;
            tool._taper_l = tool._R_br / std::sin(agl*_RadAlg);
            tool._full_taper_l = tool._R / std::sin(agl*_RadAlg);
            break;
        }
        default:
            throw "tool type error!";
        }
    }
    
    
    
};

}

#endif // TOOLPARAM_H

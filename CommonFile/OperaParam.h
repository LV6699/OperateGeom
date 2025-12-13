#ifndef OPERAPARAM_H
#define OPERAPARAM_H

namespace grm{

enum class ToolType{
    PlaneEnd = 0,  ///平底刀
    BallNoseEnd,   ///圆笔刀
    RoundNoseEnd,  ///球刀
    V_Cutter       ///v刀
};

class  DefTool{
public:
    DefTool(){}
    DefTool(ToolType t,double R,double cr):_type(t),_R(R),_cr(cr){
        if(t == ToolType::PlaneEnd){cr = 0;}
        else if(t == ToolType::BallNoseEnd){cr = R;}
        else if(t == ToolType::V_Cutter){cr = 0;}
    }
    void SetR(double r){_R = r;}
    void SetCR(double r){_cr = r;}
    void CalRR(){_RR = _R*_R;}

    double R()const{return _R;}
    double CR()const{return _cr;}
    double RadSub()const{return _R - _cr;}
    double RR()const{return _RR;}

public:
    ToolType _type;
    double _R = 0;
    double _cr = 0;
    double _RR = 0;

};


}



#endif // OPERAPARAM_H

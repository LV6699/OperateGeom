#ifndef OPERAPARAM_H
#define OPERAPARAM_H

namespace grm{

enum class ToolType{
    PlaneEnd = 0,
    RoundNoseEnd,
    BallNoseEnd,
    V_Cutter
};

class  DefTool{
public:
    DefTool(){}
    DefTool(double R,double cr):_R(R),_cr(cr){}
    void SetR(double r){_R = r;}
    void SetCR(double r){_cr = r;}

    double R()const{return _R;}
    double CR()const{return _cr;}
    double RadSub()const{return _R - _cr;}

public:
    ToolType _type;
    double _R = 0;
    double _cr = 0;

};


}



#endif // OPERAPARAM_H

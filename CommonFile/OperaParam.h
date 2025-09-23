#ifndef OPERAPARAM_H
#define OPERAPARAM_H

namespace grm{
class  DefTool{
public:
    DefTool(){}
    DefTool(double R,double cr):_R(R),_cr(cr){}
    void SetRadius(double r){_R = r;}
    void SetCornerR(double r){_cr = r;}

    double Radius()const{return _R;}
    double CornerR()const{return _cr;}

public:
    double _R = 0;
    double _cr = 0;

};


}



#endif // OPERAPARAM_H

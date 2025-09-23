FkdrRgCT98Q3-oPB1XDSQfUVeboA
Page 1#ifndef CORNERMACHAUX_H
#define CORNERMACHAUX_H
#include"../../../CommonFile/Header.h"
class CornerMachAux
{
public:
    CornerMachAux(){}
    void GetAdjoinElementAngle(const DefElem& ele1
                               const DefElem& ele2
                               double& angle);
};
#endif // CORNERMACHAUX_H
FgPtppM9u_g3TEZItDArF0fXv3f9
Page 1#ifndef EQUCURVESOLVE_H
#define EQUCURVESOLVE_H
#include"../../../CommonFile/Header.h"
class EquCurveSolve
{
public:
    EquCurveSolve(){}
    void CreateDoubleArc(const DefElem& preOriEle
                         ProfType loopType double R
                         vector<DefElem>&vEquElem);
    void NotExistEquidCurve(const DefElem &preOriEle
                            const DefArc &oriArc
                            ProfType loopType double R
                            vector<DefElem>&vEquElem);
};
#endif // EQUCURVESOLVE_H
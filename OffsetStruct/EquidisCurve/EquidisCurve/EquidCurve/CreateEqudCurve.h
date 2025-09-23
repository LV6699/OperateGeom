#ifndef CREATEEQUDCURVE_H
#define CREATEEQUDCURVE_H
#include "EquCurveSolve.h"
class CreateEqudCurve
{
public:
    CreateEqudCurve(){}
    void EquidCurvePretreatment(DefLoop oriLoop,
                                double offsetVal,
                                bool &isIterminal,
                                bool &isValOffset,
                                DefLoop &resLoop);
    void CreateEquidistanceCurve(DefLoop oriLoop,
                                 double offsetVal,
                                 bool& isValOffset,
                                 DefLoop& resLoop);
};
#endif // CREATEEQUDCURVE_H
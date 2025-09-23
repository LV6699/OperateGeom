#ifndef CREATECURVELOGIC_H
#define CREATECURVELOGIC_H
#include"../../../CommonFile/Header.h"
class CreateCurveLogic
{
public:
    CreateCurveLogic(){}
    void OperateInvalidArc(vector<DefElem>& elems);
    void CalculateEquidistanceCurve(const vector<DefElem>& oriElems,
                                    ProfType proType,
                                    double offsetVal,
                                    vector<DefElem>& elems);
    void ClosedEquidistanceCurve(const DefLoop &oriLoop,
                                 vector<DefElem> &equEles,
                                 double offsetVal,
                                 ProfType proType,
                                 vector<DefElem> &elems);
};
#endif // CREATECURVELOGIC_H
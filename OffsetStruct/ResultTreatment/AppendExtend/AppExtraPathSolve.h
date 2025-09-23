#ifndef APPEXTRAPATHSOLVE_H
#define APPEXTRAPATHSOLVE_H
#include"../../CommonFile/Header.h"
class AppExtraPathSolve
{
public:
    AppExtraPathSolve();
    void AdjoinElementAppendPath(DefLoop &loop, int index,
                                 double radius, double offsetVal,
                                 vector<DefElem> &vElem);
    void CreateSegSegExtraPath(double radius,
                               double offsetVal,
                               const DefSeg& seg1,
                               const DefSeg& seg2,
                               OftBasePara basePara,
                               const double angle,
                               vector<DefElem>&vElem);
    void CreateSegArcExtraPath(double radius,
                               double offsetVal,
                               const DefSeg& seg,
                               const DefArc& arc,
                               bool isLineFont,
                               OftBasePara basePara,
                               const double angle,
                               vector<DefElem>&vElem);
    void CreateArcArcExtraPath(double radius,double offsetVal,
                               const DefArc& arc1,
                               const DefArc& arc2,
                               OftBasePara basePara,
                               const double angle,
                               vector<DefElem>&vElem);
};
#endif // APPEXTRAPATHSOLVE_H
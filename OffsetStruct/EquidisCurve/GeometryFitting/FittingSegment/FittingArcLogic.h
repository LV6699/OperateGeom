FmYsy9Z-zO6yvNrR1Jj66H0iM92T
Page 1#ifndef FITTINGARCLOGIC_H
#define FITTINGARCLOGIC_H
#include"../../../CommonFile/Header.h"
using namespace GeomFitting;
class FittingArcLogic
{
public:
    FittingArcLogic(){}
    void LoopSegmentFittingArc(const DefLoop& loop
                               vector<FittArc>& vFitArc
                               DefLoop& resLoop);
};
#endif // FITTINGARCLOGIC_H
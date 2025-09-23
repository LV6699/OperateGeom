#ifndef ELEPRESOLVE_H
#define ELEPRESOLVE_H
#include"../../CommonFile/Header.h"
class ElementPreSolve
{
public:
    ElementPreSolve(){}
    void CheckLoopArcRadius(DefLoop& loop);
    void HandleAdjacentParallSeg(DefLoop& loop);
    void HandleAdjacentParallArc(DefLoop& loop);
    void CircleStructureArcLoop(DefLoop& loop);
    void PreviousElemEndpoint(const DefLoop &loop,
                                 int i, Point& P);
    void ResetArcCenter(DefArc &arc);
    void ResetArcEndpoint(bool isLast,
                          DefArc& arc);
    void SetLineHorVerQuality(Point &lSp, Point &lEp,
                              bool isChEp, double pre);
};
#endif // ELEPRESOLVE_H
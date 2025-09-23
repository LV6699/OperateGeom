#ifndef LOOPROTDIRE_H
#define LOOPROTDIRE_H
#include"../../CommonFile/Header.h"
class LoopDirection
{
public:
    LoopDirection(){}
    void GetLoopRotDirection(const DefLoop& loop,
                             RotDir& rot);
    void ResetEleCoincideEndpoint(DefLoop& loop);
    void DelSlopIdenticalLine(DefLoop& loop);
    void GetMaxtXCoodEndPnEleIdx(const DefLoop& loop,int& index);
    void LoopDirectionPreprocess(DefLoop &loop);
    void LoopRotateDirection(const DefLoop& loop, RotDir &rot);
    RotDir LoopRotateDirection(const DefLoop& loop_);
    bool ApplicableLimitCoordPoint(const DefLoop& loop,
                                   const 
vector<std::pair<int,Point>>&vIdCoordPt,
                                   Point &P1,Point &P2,Point &P3);
    bool SearchSmallerPoint(const DefLoop& loop, int index,
                            const Point &P, double stepVal,
                            Point &prePt, Point &nexPt);
    bool ExploreSmallerPoint(const DefLoop &loop, int index,
                             const Point &P,
                             Point &prePt, Point &nexPt);
    void CloseArcEndpointBaseAngle(DefArc arc,
                                   bool isCloseEp,
                                   double stepAgl,
                                   Point& P);
    void ApproachCurveEndpoint(const DefElem& ele,
                                 double stepVal,
                                 bool isAngle,
                                 bool isNearSp,
                                 Point& P);
};
#endif // LOOPROTDIRE_H
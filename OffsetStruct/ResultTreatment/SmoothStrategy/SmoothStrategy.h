#ifndef SmoothStrategy_H
#define SmoothStrategy_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT SmoothStrategy
{
public:
    SmoothStrategy(){}
    void CalSigAnglBisectorStrechLenth(const Point& l1Sp,
                                       const Point& l1Ep,
                                       const Point& l2Sp,
                                       const Point& l2Ep,
                                       const double& strechLimit,
                                       DefArc& arc);
    void CalculationArcArcRotDir(const DefArc& arc1,
                                 const DefArc& arc2,
                                 RotDir& rotDir);
    bool CircleTranslateArcSmooth(vector<DefLoop>& vLoop,
                                  DefLoop& loop);
    void ArrayLoopElementSmooth(const double R, bool isAddOriLoop,
                                vector<DefLoop> &vLoop);
};
#endif // SmoothStrategy_H
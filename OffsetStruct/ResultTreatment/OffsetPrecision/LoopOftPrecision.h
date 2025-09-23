#ifndef LOOPOFTPRECISION_H
#define LOOPOFTPRECISION_H
#include "../../CommonFile/Header.h"
class LoopOftPrecision {
public:
    LoopOftPrecision(){}
    void LoopCheckStruct(const DefLoop &oriLoop,
                         const DefLoop &loop,
                         bool isStepAngle,
                         double segStep,
                         double arcStep,
                         OffsetResCheck::CheckLoop &checkLoop);
    void ResultCheckStruct(const DefLoop &oriLoop,
                           const vector<DefLoop> &vLoop, double refValue,
                           double &error);
    void AnalysisCheckLoop(OffsetResCheck::CheckLoop &checkLoop, bool isMax);
    void LoopCheckDistanceError(OffsetResCheck::CheckLoop& checkLoop,
                                double refValue,
                                double &error);
};
#endif // LOOPOFTPRECISION_H
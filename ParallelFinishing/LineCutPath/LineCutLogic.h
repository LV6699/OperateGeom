#ifndef LINECUTLOGIC_H
#define LINECUTLOGIC_H
#include"../UnitStruct/GeomUnitStruct.h"
class LineCutLogic
{
public:
    LineCutLogic(){}
    // void LoopToToolLocationLoop(const std::vector<std::vector<GeomUnit::Rectangle>> &vRecs,
    //                             const ofts::DefLoop& loop, const double l,
    //                             ofts::DefLoop& resloop); // 娌℃湁浣跨敤锛屾殏鏃跺睆钄
    // void GenerateLoopLinePath(ofts::DefLoop loop,
    //                           double dist, double agl, ofts::DefLoop& res); // 娌℃湁浣跨敤锛屾殏鏃跺睆钄
    // void LoopsClipLinePath(const std::vector<ofts::DefLoop>& loops,
    //                        const ofts::DefLoop &loop, ofts::DefLoop &res); // 娌℃湁浣跨敤锛屾殏鏃跺睆钄
    void AreaLineIntersectPoint(const ofts::GeomArea &area,
                                double dist, double agl,
                                std::vector<GeomUnit::IntPnt> &intPts);
};
#endif // LINECUTLOGIC_H
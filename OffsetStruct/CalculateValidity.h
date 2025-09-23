#ifndef CALCULATEVALIDITY_H
#define CALCULATEVALIDITY_H
#include"CommonFile/Header.h"
class CalculateValidity
{
public:
    CalculateValidity(){}
    void IsCurrentValidOffset(OffsetParam offsetPara,
                              vector<DefLoop> &vOftloop,
                              const DefLoop& oriLoop,
                              const DefLoop& loop,
                              double offsetVal,
                              bool& isValCal);
    void IsValEquidistanceCurveLenth(const DefLoop& oriLoop,
                                     const DefLoop& loop,
                                     bool& isValCal);
    bool IsVallidLocationRelation(const DefLoop& oriLoop,
                                  const DefLoop& loop);
    bool LoopMinBoundingBox(const DefLoop& loop);
    bool IsMeetMinBoxMeasure(const DefLoop& loop);
};
#endif // CALCULATEVALIDITY_H
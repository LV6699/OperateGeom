#ifndef CALCULATEVALIDITY_H
#define CALCULATEVALIDITY_H
#include"CommonFile/Header.h"
class CalculateValidity
{
public:
    CalculateValidity(){}
    void IsCurrentValidOffset(ofts::OffsetParam param,
                              std::vector<ofts::DefLoop> &oftloops,
                              const ofts::DefLoop& oloop,
                              const ofts::DefLoop& loop,
                              double offsetVal,
                              bool& isValCal);
    bool IsVallidLocationRelation(const ofts::DefLoop& oriLoop,
                                  const ofts::DefLoop& loop);
    bool LoopMinBoundingBox(const ofts::DefLoop& loop);
    bool IsMeetMinBoxMeasure(const ofts::DefLoop& loop);
};
#endif // CALCULATEVALIDITY_H

#ifndef EQUIDLOOP_H
#define EQUIDLOOP_H
#include"../../CommonFile/DataStructure.h"
class EquidLoop
{
public:
    EquidLoop(){}

    bool EquidCurvePreproc(ofts::DefLoop& oloop,
                           double dist,bool& isValid,
                           ofts::DefLoop& rloop);
    void OperateInvalidArc(std::vector<ofts::DefElem>& cs);
    void CalculateEquidCurve(const std::vector<ofts::DefElem>& ecs,
                             bool leftward,double dist,
                             std::vector<ofts::DefElem>& cs);
    void ClosedEquidCurve(const ofts::DefLoop& oloop,double dist,
                          std::vector<ofts::DefElem>& ecurves,
                          std::vector<ofts::DefElem>& curves);
    ofts::DefLoop ExicuteEquidCurve(const ofts::OffsetParam& param,
                                    ofts::DefLoop& oloop,
                                    double dist,bool& isValid);
    ofts::DefLoop GetEquidistanceCurve(const ofts::OffsetParam& param,
                                       ofts::DefLoop oloop,
                                       double dist,bool& isValid);
};
#endif // EQUIDLOOP_H

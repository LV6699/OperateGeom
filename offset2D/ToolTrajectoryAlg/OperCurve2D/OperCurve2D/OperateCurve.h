#ifndef OPERATEELEM_H
#define OPERATEELEM_H
#include"../../CommonFile/DataStructure.h"
class OperateCurve
{
public:
    OperateCurve(){}
    bool AdjoinCurveRotDire(const ofts::DefElem& c1,
                            const ofts::DefElem& c2);
    bool AdjoinSegRotDire(const ofts::DefSeg& seg1,
                          const ofts::DefSeg& seg2);
    bool AdjoinSegArcRotDire(const ofts::DefSeg& seg,
                             const ofts::DefArc& arc,
                             bool isLineFont);
    bool AdjoinArcRotDire(const ofts::DefArc& arc1,
                          const ofts::DefArc& arc2);
    void DelInvalidGeomCurves(std::vector<ofts::DefElem>& cs,
                              double segErr,double aEnd,
                              bool isR = false,double minR = PreErr_8,
                              bool isBh = false,double bH = PreErr_8,
                              bool aIsDecNum = false,int aNum = 6);
    //ofts::DefSeg ArcPositionTangent(const ofts::DefArc& arc,const ofts::Point& p);
};
#endif // OPERATEELEM_H

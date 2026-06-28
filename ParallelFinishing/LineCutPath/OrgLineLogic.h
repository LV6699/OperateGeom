#ifndef ORGLINELOGIC_H
#define ORGLINELOGIC_H
#include"../UnitStruct/GeomUnitStruct.h"
class OrgLineLogic
{
public:
    OrgLineLogic(){}
    ofts::DefElem IntPtToElem(const GeomUnit::IntPnt * const p1,
                        const GeomUnit::IntPnt * const p2)
    {
        return ofts::DefElem(LINETYPE,ofts::DefSeg(p1->P(),p2->P()));
    }
    void AddLoop(vector<ofts::DefLoop>& vLoop,ofts::DefLoop& loop){
        if(!loop.IsEmpty()){
            vLoop.emplace_back(loop);
            loop.Clear();
        }
    }
    void LineCutPointToLoop(const ofts::DefLoop &loop_,
                            vector<GeomUnit::IntPnt> &intPts,
                            vector<GeomUnit::IntLine> &intls,
                            GeomUnit::IntPnt *cur,
                            vector<ofts::DefLoop> &vLoop);
    void LineCutPointToLoop(const ofts::GeomArea& area,
                            ofts::ParalParam param,
                            vector<GeomUnit::IntPnt>& intPts,
                            vector<GeomUnit::IntLine>& intls,
                            GeomUnit::IntPnt* cur,
                            vector<ofts::DefLoop>& rloops);
    void SingleDirectionLoop(const ofts::GeomArea &area,double agl,
                             vector<GeomUnit::IntPnt>& intPts,
                             vector<ofts::DefLoop>& vLoop);
};
#endif // ORGLINELOGIC_H
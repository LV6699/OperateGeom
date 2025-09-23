#ifndef ORGLINELOGIC_H
#define ORGLINELOGIC_H
#include"../UnitStruct/GeomUnitStruct.h"
class OrgLineLogic
{
public:
    OrgLineLogic(){}
    OftStr::DefElem IntPtToElem(const GeomUnit::IntPnt * const p1,
                        const GeomUnit::IntPnt * const p2)
    {
        return OftStr::DefElem(LINETYPE,OftStr::DefSeg(p1->P(),p2->P()));
    }
    void AddLoop(vector<OftStr::DefLoop>& vLoop,OftStr::DefLoop& loop){
        if(!loop.IsEmpty()){
            vLoop.emplace_back(loop);
            loop.ClearElem();
        }
    }
    void LineCutPointToLoop(const OftStr::DefLoop &loop_,
                            vector<GeomUnit::IntPnt> &intPts,
                            vector<GeomUnit::IntLine> &intls,
                            GeomUnit::IntPnt *cur,
                            vector<OftStr::DefLoop> &vLoop);
    void LineCutPointToLoop(const OftStr::GeomArea& area,
                            vector<GeomUnit::IntPnt>& intPts,
                            vector<GeomUnit::IntLine>& intls,
                            GeomUnit::IntPnt* cur,
                            vector<OftStr::DefLoop>& vLoop);
    void SingleDirectionLoop(const OftStr::GeomArea &area,double agl,
                             vector<GeomUnit::IntPnt>& intPts,
                             vector<OftStr::DefLoop>& vLoop);
};
#endif // ORGLINELOGIC_H
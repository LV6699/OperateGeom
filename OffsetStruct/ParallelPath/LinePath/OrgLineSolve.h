#ifndef ORGLINESOLVE_H
#define ORGLINESOLVE_H
#include"../UnitStruct/GeomUnitStruct.h"
class OrgLineSolve
{
public:
    OrgLineSolve(){}
    void PointsBoundary(const OftStr::DefLoop& loop,
                        const GeomUnit::IntPnt * const pre,
                        const GeomUnit::IntPnt * const nex,
                        bool isNex,
                        vector<OftStr::DefElem> &eles);
    OftStr::DefElem IntPtElem(const GeomUnit::IntPnt * const p1,
                      const GeomUnit::IntPnt * const p2)
    {
        return OftStr::DefElem(LINETYPE,OftStr::DefSeg(p1->P(),p2->P()));
    }
};
#endif // ORGLINESOLVE_H
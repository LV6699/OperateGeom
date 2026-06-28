#ifndef ORGLINESOLVE_H
#define ORGLINESOLVE_H
#include"../UnitStruct/GeomUnitStruct.h"
class OrgLineSolve
{
public:
    OrgLineSolve(){}
    void PointsBoundary(const ofts::DefLoop& loop,
                        const GeomUnit::IntPnt * const pre,
                        const GeomUnit::IntPnt * const nex,
                        bool isNex,
                        vector<ofts::DefElem> &eles);
    ofts::DefElem IntPtElem(const GeomUnit::IntPnt * const p1,
                      const GeomUnit::IntPnt * const p2)
    {
        return ofts::DefElem(LINETYPE,ofts::DefSeg(p1->P(),p2->P()));
    }
};
#endif // ORGLINESOLVE_H
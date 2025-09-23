#ifndef CREATEHELIX_H
#define CREATEHELIX_H
#include"../../CommonFile/Header.h"
class CreateHelix
{
public:
    CreateHelix(){}
    void LoopSpecifyPositionHelix(const GeomArea &oriLoop,
                                  const DefLoop& loop,
                                  const DefElem& element,
                                  const Point& posPnt,
                                  const OffsetParam &oftPara,
                                  CylinderHelix helix,
                                  DefLoop& resLoop);
    void CreateHelixCurves(const Point& posPnt,
                           CylinderHelix helix,
                           DefLoop& resLoop);
};
#endif // CREATEHELIX_H
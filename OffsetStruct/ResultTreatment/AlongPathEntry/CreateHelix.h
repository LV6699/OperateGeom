#ifndef CREATEHELIX_H
#define CREATEHELIX_H
#include"../../CommonFile/Header.h"
class CreateHelix
{
public:
    CreateHelix(){}
    void LoopSpecifyPositionHelix(const GeomArea &geoArea,
                                  const DefLoop &loop,
                                  MillDirction millDir,
                                  double toolRadius,
                                  CylinderHelix helix,
                                  DefLoop& resLoop);
    void CreateHelixCurves(const Point& posPnt,
                           CylinderHelix helix,
                           DefLoop& resLoop);
};
#endif // CREATEHELIX_H
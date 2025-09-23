#ifndef ALONGPATHENTRY_H
#define ALONGPATHENTRY_H
#include "CreateHelix.h"
class AlongPathEntry
{
public:
    AlongPathEntry(){}
    void LoopOverlapSpecifyDistance(DefLoop oriLoop,
                                    double overDis,
                                    DefLoop &overLoop,
                                    DefLoop& loop);
    void CreateAlongPathCutEntry(const GeomArea &geoArea,
                                 const DefLoop& oriLoop,
                                 MillDirction millDir,
                                 double toolRadius,
                                 CylinderHelix helix,
                                 bool isOverlap,
                                 double overDis, bool &isHelixPath,
                                 DefLoop& resLoop);
    void CreateAlongPathObliqueCurve(const DefLoop &loop,
                                     CylinderHelix helix,
                                     double iniZCoord,
                                     DefLoop &resLoop);
};
#endif // ALONGPATHENTRY_H
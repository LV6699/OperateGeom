#if 0
#ifndef GEOMTOSHAPES_H
#define GEOMTOSHAPES_H
#include"../ToolTrajectoryAlg/CommonFile/DataStructure.h"
#include"../ViewWind/mainwindow.h"
class GeomToShapes
{
public:
    GeomToShapes(){}
    void ElementToShape(const DefElem& ele,
                        TopoDS_Shape &shape);
};
#endif // GEOMTOSHAPES_H
#endif

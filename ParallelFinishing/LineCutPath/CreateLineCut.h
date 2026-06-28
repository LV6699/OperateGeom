#ifndef CREATELINECUT_H
#define CREATELINECUT_H
///#include <tbb/tbb.h>
#include"../UnitStruct/GeomUnitStruct.h"
class CreateLineCut
{
public:
    CreateLineCut(){}
    void BoundaryLineCutPath(ofts::GeomArea &area,
                             ofts::ParalParam param,
                             std::vector<ofts::DefLoop>& loops);
    void GeomAreaLineCutPath(ofts::GeomArea& area,
                             ofts::ParalParam param,
                             std::vector<ofts::DefLoop>& loops);
    void CreateLineCutPath(const ofts::GeomArea& area,
                           ofts::ParalParam param,
                           std::vector<ofts::DefLoop>& vResLoop);
};
#endif // CREATELINECUT_H
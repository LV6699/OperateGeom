#ifndef TEMCLOSELOOP_H
#define TEMCLOSELOOP_H
#include "../offset2D/ToolTrajectoryAlg/CommonFile/DataStructure.h"
class TemCloseLoop
{
public:
TemCloseLoop(){}
void TemLoopData(ofts::DefLoop& loop);
void TemLoopsData(std::vector<ofts::DefLoop>& loops);
void TemLoopsData1(std::vector<ofts::DefLoop>& loops);
void TemLoopsData2(std::vector<ofts::DefLoop>& loops);
void TemRegionData(ofts::GeomArea& region);
void TemCloseLoopData(ofts::DefLoop& loop_);
void TemCloseLoop2Data(ofts::DefLoop& loop,
ofts::DefLoop& loop1);
void TemRegionData1(ofts::GeomArea& region);
};
#endif // TEMCLOSELOOP_H
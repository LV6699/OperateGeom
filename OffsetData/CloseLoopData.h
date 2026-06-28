#ifndef CLOSELOOPDATA_H
#define CLOSELOOPDATA_H
#include "../offset2D/ToolTrajectoryAlg/CommonFile/DataStructure.h"
class CloseLoopData
{
public:
CloseLoopData(){}
void MulLoopTest(ofts::DefLoop& loop);
void DefineAreaTest(std::vector<ofts::DefLoop>& loops);
void ClipSample(std::vector<ofts::DefLoop>& sloops,std::vector<ofts::DefLoop>& cloops);
void OutputStruct(ofts::GeomArea& region);
void ParallelData(ofts::GeomArea& region);








};
#endif // CLOSELOOPDATA_H
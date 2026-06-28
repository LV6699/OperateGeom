#ifndef OPENLOOPDATA_H
#define OPENLOOPDATA_H
#include "../offset2D/ToolTrajectoryAlg/CommonFile/DataStructure.h"
class OpenLoopData
{
public:
OpenLoopData(){}
void OpenLoopOffset1(ofts::DefLoop& loop);
void OpenLoopOffset2(ofts::DefLoop& loop);
void OpenLoopOffset3(ofts::DefLoop& loop);
void OpenLoopOffset4(ofts::DefLoop& loop);
void OpenLoopOffset5(ofts::DefLoop& loop);
//AreaData
void GetOpenAreaObstruct(std::vector<ofts::DefLoop>&loops);
void BndIslMixLoop(ofts::GeomArea& area);
void BndIslMixLoop1(ofts::GeomArea& area);
void BndIslMixLoop2(ofts::GeomArea& area);
void BndIslMixLoop3(ofts::GeomArea& area);
};
#endif // OPENLOOPDATA_H
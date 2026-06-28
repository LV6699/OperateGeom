#ifndef HORARCSTEER_H
#define HORARCSTEER_H
#include"../../CommonFile/DataStructure.h"
class HorArcSteer
{
public:
    HorArcSteer(){}
    void HorizontalArcSteer(OffsetStruct::DefLoop& loop,
                            OffsetStruct::MillDirction millDir,
                            bool isEntry, double R, double agl,
                            OffsetStruct::DefArc &arc);
};
#endif // HORARCSTEER_H

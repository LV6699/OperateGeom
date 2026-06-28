#ifndef CLIPPLANEREGION_H
#define CLIPPLANEREGION_H
#include"../../CommonFile/DataStructure.h"
class ClipPlaneRegion
{
public:
    ClipPlaneRegion(){}
    void IntRegionLoopClip(const std::vector<ofts::DefLoop>& intBs,
                           const std::vector<ofts::DefLoop>& intIs,
                           const ofts::DefLoop& loop,
                           std::vector<ofts::DefLoop>& rloops);
    void RegionLoopsBoolInt(const ofts::GeomArea& region,
                            const std::vector<ofts::DefLoop>& inloops,
                            std::vector<ofts::DefLoop>& rloops);
};
#endif // CLIPPLANEREGION_H

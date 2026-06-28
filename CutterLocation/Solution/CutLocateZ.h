#ifndef CUTTERLOCATIONZ_H
#define CUTTERLOCATIONZ_H

#include "IntEdgeProt.h"
#include "IntVertexProt.h"

#pragma optimize("", off)
#pragma GCC optimize("o0")

namespace grm {
class CutLocateZ {
public:
    CutLocateZ() {}

    static double ProtectFaceZ(const Triangle& t,const ofts::Point& p)
    {
        double z = Min_Value;
        double t1 = t.N().X() * (t.P0().X() - p.X());
        double t2 = t.N().Y() * (t.P0().Y() - p.Y());
        z = t.P0().Z() + (t1 + t2) / t.N().Z();
        return z;
    }

    double CurrentLocation(const MeshMap& m, ofts::Point& op);
    void CutterLocation(MeshMap& m);
};

void GetPtAllLocation(const MeshMap& m, const ofts::Point& p,
                      std::vector<ClRelItem>& rels);
void ClRelItemToShape(ClRelItem& item);

}  // namespace grm

#endif  // CUTTERLOCATIONZ_H





















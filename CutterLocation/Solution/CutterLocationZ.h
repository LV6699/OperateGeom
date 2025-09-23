#ifndef CUTTERLOCATIONZ_H
#define CUTTERLOCATIONZ_H

#include "OperTriaCl.h"

namespace grm{
class CutterLocationZ
{
public:
    CutterLocationZ(){}

    double PointClZ(const MeshMap& m,const oft::Point& p);
    double ClBaseTrias(const MeshMap& m,const oft::Point& p)
    {
        OperTriaCl ot;
        double z = Min_Val,t1 = 0,t2 = 0;

        for(size_t i = 0;i < m.TrianglesCl().size();++i){
            const auto& t = m._tris[i];
            if(!t.IsInXYRange(p)){continue;}
            t1 = t.N().X() * (t.P0().X() - p.X());
            t2 = t.N().Y() * (t.P0().Y() - p.Y());
            if(t.P0().Z() + t1 + t2 > z){
                z = t.P0().Z() + t1 + t2;
            }
        }
        return z;
    }
    void GetPointCl(MeshMap& m)
    {
        auto& pts = m._clPts;
        for(size_t i = 0;i < pts.size();++i){
            for(size_t j = 0;j < pts[i].size();++j){
                pts[i][j].SetZ(ClBaseTrias(m,pts[i][j]));
            }
        }

    }

    double ClZ(const MeshMap& m,const oft::Point& p);








};

}

#endif // CUTTERLOCATIONZ_H







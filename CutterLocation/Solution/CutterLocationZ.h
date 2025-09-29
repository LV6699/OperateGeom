#ifndef CUTTERLOCATIONZ_H
#define CUTTERLOCATIONZ_H

#include "OperTriaCl.h"
#pragma optimize("", off)
#pragma GCC optimize ("o0")

namespace grm{
class CutterLocationZ
{
public:
    CutterLocationZ(){}

    double PointClZ(const MeshMap& m,const oft::Point& p);

    double CutterLocation(const Triangle& t,const oft::Point& p){
        double z = Min_Val;
#if 0
        ///需要保证点在t的xy平面内
        if(t.IsVertical()){return p.Z();}
        /// 计算三角形所在平面的方程：Ax + By + Cz + D = 0
        oft::Point ab = oft::Point(t._p1.X() - t._p0.X(),t._p1.Y() -
                                   t._p0.Y(),t._p1.Z() - t._p0.Z());
        oft::Point ac = oft::Point(t._p2.X() - t._p0.X(),t._p2.Y() -
                                   t._p0.Y(),t._p2.Z() - t._p0.Z());
        oft::Point normal = ab.Cross(ac);
        double A = normal.X(),B = normal.Y(),C = normal.Z(),
                D = -A * t._p0.X() - B * t._p0.Y() - C * t._p0.Z();
        double z = (-D - A * p.X() - B * p.Y()) / C;
#else
        double t1 = t.N().X() * (t.P0().X() - p.X());
        double t2 = t.N().Y() * (t.P0().Y() - p.Y());
        z = t.P0().Z() + (t1 + t2) / t.N().Z();
#endif
        return z;
    }

    double CutterLocation(const MeshMap& m,const oft::Point& p)
    {
        OperTriaCl ot;
        double z = p.Z(),z_ = p.Z();

        for(size_t i = 0;i < m.TrianglesCl().size();++i){
            const auto& t = m._tris[i];
            if(!t.IsInRange(p) || t.N().Z() < PreErr_8){continue;}
            z_ = CutterLocation(t,p);
            if(z < z_){z = z_;}
        }
        return z;
    }
    void CutterLocation(MeshMap& m)
    {
        double z = 0;
        auto& pts = m._clPts;
        for(size_t i = 0;i < pts.size();++i){
            for(size_t j = 0;j < pts[i].size();++j){
                z = CutterLocation(m,pts[i][j]);
                pts[i][j].SetZ(z);
            }
        }

    }

    double ClZ(const MeshMap& m,const oft::Point& p);








};

}

#endif // CUTTERLOCATIONZ_H







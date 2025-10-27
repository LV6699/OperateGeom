#ifndef CUTTERLOCATIONZ_H
#define CUTTERLOCATIONZ_H

#include "IntEdgeProt.h"
#pragma optimize("", off)
#pragma GCC optimize ("o0")

namespace grm{
class CutterLocationZ
{
public:
    CutterLocationZ(){}
    double OftTriangleCase(const Triangle& t,const oft::Point& p){
        double z = Min_Val;
        double t1 = t.N().X() * (t.P0().X() - p.X());
        double t2 = t.N().Y() * (t.P0().Y() - p.Y());
        z = t.P0().Z() + (t1 + t2) / t.N().Z();
        return z;
    }
    double VertexProtectCase(const DefTool& T,const oft::Point& v,
                             const oft::Point& p)
    {
        ///if(v.DistSquare2D(p) < 14){
        ///int tem = 1 ;
        ///}
        double RR = T._R * T._R;
        double dd = std::pow(p.X()-v.X(),2) + std::pow(p.Y()-v.Y(),2);
        if(dd > RR){return Min_Val;}
        double h = std::sqrt(RR - dd);
        double zq = Min_Val;
        switch (T._type) {
        case ToolType::PlaneEnd:{
            zq = v.Z();break;
        }
        case ToolType::BallNoseEnd:{
            zq = v.Z() + h;break;
        }
        case ToolType::RoundNoseEnd:{
            double d = std::sqrt(dd);
            if(dd <= T.RadSub()*T.RadSub()){
                zq = v.Z();
            }else if(pnum::RightInc2(T.RadSub(),d,T.R())){
                double rr = T._cr*T._cr;
                double t = d - T.R() + T.CR();
                double t1 = std::sqrt(rr - t*t);
                zq = v.Z() + t1;
            }
            break;
        }
        default: break;
        }
        ///if(zq > 0){
        ///int tem = 1;
        ///}
        return zq;
    }

    double TriangleVertexCase(const DefTool& T,const Triangle& t,
                              const oft::Point& p){
        double z = VertexProtectCase(T,t.P0(),p);
        double z1 = VertexProtectCase(T,t.P1(),p);
        double z2 = VertexProtectCase(T,t.P2(),p);
        return LimVal::ThreeMax(z,z1,z2);
    }
    double CutterLocation(const MeshMap& m,const oft::Point& p)
    {
        OperTriaCl ot;
        double oz = p.Z(),oz_,vz = p.Z(),vz_ = p.Z(),
                ez = p.Z(),ez_ = p.Z();
        const auto& ts = m._trisCl;
        ///oft::Point op(-32,14);
        for(size_t i = 0;i < ts.size();++i){/***/
            if(/**p.IsSamePoint2D(op,PreErr_4) &&*/ i == 386){
                int tem = 1;
            }
            const auto& t = ts[i];
            if(t.IsInRange(p) && t.N().Z() > PreErr_8){
                oz_ = OftTriangleCase(t,p);
                if(oz_ > oz){
                    oz = oz_;
                }
            }
            vz_ = TriangleVertexCase(m.Tool(),t,p);
            if(vz_ > vz){
                vz = vz_;
            }
            ez_ = IntEdgeProt::TriangleEdgeCase(m.Tool(),t,p);
            if(ez_ > ez){
                ez = ez_;
            }
        }
        return LimVal::ThreeMax(oz,vz,ez);
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









};

}

#endif // CUTTERLOCATIONZ_H







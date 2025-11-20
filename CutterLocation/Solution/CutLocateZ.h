#ifndef CUTTERLOCATIONZ_H
#define CUTTERLOCATIONZ_H

#include "IntEdgeProt.h"

#pragma optimize("", off)
#pragma GCC optimize ("o0")

namespace grm{

class CutLocateZ
{
public:
    CutLocateZ(){}

    static double OftTriangleCase(const Triangle& t,const oft::Point& p){
        double z = Min_Val;
        double t1 = t.N().X() * (t.P0().X() - p.X());
        double t2 = t.N().Y() * (t.P0().Y() - p.Y());
        z = t.P0().Z() + (t1 + t2) / t.N().Z();
        return z;
    }
    static double VertexProtectCase(const DefTool& T,const oft::Point& v,
                                    const oft::Point& p)
    {
        ///if(v.DistSquare2D(p) < 14){
        ///int tem = 1 ;
        ///}
        double RR = T._R * T._R;
        double dd = p.DistSquare2D(v);
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
    static double TriangleVertexCase(const DefTool& T,const Triangle& t,
                                     const oft::Point& p){
        double z = VertexProtectCase(T,t.P0(),p);
        double z1 = VertexProtectCase(T,t.P1(),p);
        double z2 = VertexProtectCase(T,t.P2(),p);
        return LimVal::ThreeMax(z,z1,z2);
    }
    static double CutterLocation(const MeshMap& m,oft::Point& p)
    {
        OperTriaCl ot;
        double z1 = p.Z(),z2 = p.Z(),z3 = p.Z();

        const auto& ts = m.Triangles();
        const auto& clts = m.TrianglesCl();
        const auto& T = m.Tool();
        oft::Point op(7,19);size_t id = 589;
        for(size_t i = 0;i < clts.size();++i){/***/
            const auto& t = clts[i];/**
            if(p.IsSamePoint2D(op,PreErr_4) && i == id){
                ViewTool::DisplayPoint(p);
                ViewTool::DisplayTriangle(t,_colors[3]);
                int tem = 1;
            }*/
            if(t.IsInRange(p) && t.N().Z() > PreErr_8){
                double z = OftTriangleCase(t,p);
                if(z > z1){z1 = z;}
            }
        }
        for(size_t i = 0;i < ts.size();++i){/***/
            const auto& t = ts[i];/**
            if(p.IsSamePoint2D(op,PreErr_4) && i == id){
                ViewTool::DisplayPoint(p);
                ViewTool::DisplayTriangle(t,_colors[3]);
                int tem = 1;
            }*/
            double z = TriangleVertexCase(T,t,p);
            if(z > z2){z2 = z;}
            z = IntEdgeProt::TriangleEdgeCase(T,t,p);
            if(z > z3){z3 = z;}
        }
        return LimVal::ThreeMax(z1,z2,z3);
    }
    static void CutterLocation(MeshMap& m)
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

void GetPtAllLocation(const MeshMap& m, const oft::Point &p,
                      std::vector<ClRelItem> &rels);
void ClRelItemToShape(ClRelItem& item);


}




#endif // CUTTERLOCATIONZ_H




























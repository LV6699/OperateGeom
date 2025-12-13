#ifndef OPERTRIACL_H
#define OPERTRIACL_H

#include "../Common/ViewTool.h"

#pragma optimize("", off)

namespace grm{

class OperTriaCl
{
public:
    OperTriaCl(){}

    ofts::Point GetVertProtFace(const DefTool& T, const Triangle& t,
                                const ofts::Point& p) {
      double x = 0, y = 0, z = 0;
      switch (T._type) {
        case ToolType::PlaneEnd:{
            const auto& nor_xy = t.Nxy();
            x = T._R * nor_xy.X();
            y = T._R * nor_xy.Y();
            z = T._R * nor_xy.Z();
            break;
        }
        case ToolType::BallNoseEnd:{
            const auto& n = t.N();
            x = n.X() * T._R;
            y = n.Y() * T._R;
            z = n.Z() * T._R - T._R;
            break;
        }
        case ToolType::RoundNoseEnd:{
            double v = T._R - T._cr;
            const auto& n = t.N();
            const auto& nor_xy = t.Nxy();
            x = n.X() * T._cr + nor_xy.X() * v;
            y = n.Y() * T._cr + nor_xy.Y() * v;
            z = n.Z() * T._cr + nor_xy.Z() * v - T._cr;
            break;
        }
        case ToolType::V_Cutter:{
            break;
        }
        default: {break;}
        }
        ofts::Point res(p.X() + x,p.Y() + y,p.Z() + z);
        return res;
    }
    Triangle GetTriangleProtFace(const DefTool& t,const Triangle& ti){
        auto p0 = GetVertProtFace(t,ti,ti.P0());
        auto p1 = GetVertProtFace(t,ti,ti.P1());
        auto p2 = GetVertProtFace(t,ti,ti.P2());
        Triangle tcl(p0,p1,p2);
        tcl.IniOrigin(ti);
        return tcl;
    }
    void CalTrianglesProtFace(MeshMap& m){
        const auto& tool = m.Tool();
        const auto& ts = m.Triangles();
        auto& tscl = m._trisCl;
        tscl.reserve(ts.size());

        for(size_t i = 0;i < ts.size();++i){
            //if(i ==589){
            //int tem = 1;
            //}
            auto tcl = GetTriangleProtFace(tool,ts[i]);
            tscl.emplace_back(tcl);
        }
        std::cout<<"ts.size():"<<ts.size()<<endl;
    }
    bool IsPtInRangeEdge(const ofts::Point &p0, const ofts::Point &p1,
                         const ofts::Point &p, double e) 
    {
        return false;
                            
    }
};

}

#endif // OPERTRIACL_H

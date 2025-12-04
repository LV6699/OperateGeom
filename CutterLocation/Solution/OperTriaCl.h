#ifndef OPERTRIACL_H
#define OPERTRIACL_H

#include "../Common/ViewTool.h"

#pragma optimize("", off)

namespace grm{

class OperTriaCl
{
public:
    OperTriaCl(){}

    oft::Point CalVertexCl(const DefTool& T,const Triangle& t,
                           const oft::Point& p){
        double x = 0,y = 0,z = 0;
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
            const auto& n = t.N();
            const auto& nor_xy = t.Nxy();
            x = n.X() * T._cr + nor_xy.X() * (T._R - T._cr);
            y = n.Y() * T._cr + nor_xy.Y() * (T._R - T._cr);
            z = n.Z() * T._cr + nor_xy.Z() * (T._R - T._cr) - T._cr;
            break;
        }
        case ToolType::V_Cutter:{
            break;
        }
        default: {break;}
        }/**
        double v = t._R - t._cr;
        x = p.X() + v * nor_xy.X() + t._cr * ti.N().X();
        y = p.Y() + v * nor_xy.Y() + t._cr * ti.N().Y();
        z = p.Z() + v * nor_xy.Z() + t._cr * ti.N().Z();*/
        oft::Point res(p.X() + x,p.Y() + y,p.Z() + z);
        return res;
    }
    Triangle CalTriangleCl(const DefTool& t,const Triangle& ti){
        auto p0 = CalVertexCl(t,ti,ti.P0());
        auto p1 = CalVertexCl(t,ti,ti.P1());
        auto p2 = CalVertexCl(t,ti,ti.P2());
        Triangle tcl(p0,p1,p2);
        tcl.IniOrigin(ti);
        return tcl;
    }
    void CalTrianglesCl(MeshMap& m){
        const auto& tool = m.Tool();
        const auto& ts = m.Triangles();
        auto& tscl = m._trisCl;
        tscl.reserve(ts.size());

        for(size_t i = 0;i < ts.size();++i){
            //if(i ==589){
            //int tem = 1;
            //}
            auto tcl = CalTriangleCl(tool,ts[i]);
            tscl.emplace_back(tcl);
        }
        std::cout<<"ts.size():"<<ts.size()<<endl;
    }

};

}

#endif // OPERTRIACL_H

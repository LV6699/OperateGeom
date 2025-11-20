#ifndef OPERTRIACL_H
#define OPERTRIACL_H

#include "../Common/ViewTool.h"

#pragma optimize("", off)

namespace grm{

class OperTriaCl
{
public:
    OperTriaCl(){}

    oft::Point CalVertexCl(const DefTool& t,const Triangle& ti,
                                 const oft::Point& p){
        const auto& nor_xy = ti.Nxy();
//        oft::Point rp;
//        switch (t._type) {
//        case ToolType::PlaneEnd:{

//        }
//        case ToolType::PlaneEnd:{

//        }
//        case ToolType::PlaneEnd:{

//        }
//        default:break;

//        }
        double v = t._R - t._cr;
        double x = p.X() + v * nor_xy.X() + t._cr * ti.N().X();
        double y = p.Y() + v * nor_xy.Y() + t._cr * ti.N().Y();
        double z = p.Z() + v * nor_xy.Z() + t._cr * ti.N().Z();
        return oft::Point(x,y,z);
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

#ifndef OPERTRIACL_H
#define OPERTRIACL_H

#include "../Common/ClStruct.h"

#pragma optimize("", off)

namespace grm{

class OperTriaCl
{
public:
    OperTriaCl(){}

    const oft::Point CalVertexCl(const DefTool& t,const Triangle& ti,
                                 const oft::Point& p){
        double v = t._R - t._cr;
        double x = p.X() + v * ti.Nxy().X() + t._cr * ti.N().X();
        double y = p.Y() + v * ti.Nxy().Y() + t._cr * ti.N().Y();
        double z = p.Z() + v * ti.Nxy().Z() + t._cr * ti.N().Z();
        return oft::Point(x,y,z);
    }
    Triangle CalTriangleCl(const DefTool& t,const Triangle& ti){
        auto p0 = CalVertexCl(t,ti,ti.P0());
        auto p1 = CalVertexCl(t,ti,ti.P1());
        auto p2 = CalVertexCl(t,ti,ti.P2());
        return Triangle(p0,p1,p2);
    }
    void CalTrianglesCl(MeshMap& m){
        const auto& tool = m.Tool();
        auto& ts = m._tris;
        auto& tscl = m._trisCl;
        tscl.reserve(ts.size());

        for(size_t i = 0;i < ts.size();++i){
            tscl.emplace_back(CalTriangleCl(tool,ts[i]));
        }
        std::cout<<"ts.size():"<<ts.size()<<endl;
    }

};

}

#endif // OPERTRIACL_H

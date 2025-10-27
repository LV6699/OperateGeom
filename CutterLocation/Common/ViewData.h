#ifndef VIEWDATA_H
#define VIEWDATA_H

#include "ClStruct.h"
#include "../../OperateView/GeomToShape.h"

namespace grm{

class ToOcc
{
public:
    ToOcc(){}

    gp_Pnt OccPt(const oft::Point& p){return gp_Pnt(p.X(),p.Y(),p.Z());}

    static TopoDS_Shape EdgeToShape(const ClEdge& e){
        if(e._sp == nullptr || e._ep == nullptr){return TopoDS_Shape();};
        TopoDS_Shape s;
        GeomToShape().SegmentToShape(*e._sp,*e._ep,s);
        return s;

    }
    static TopoDS_Shape EdgesToShape(const std::vector<std::vector<ClEdge>>&es){
        TopoDS_Shape s;
        std::vector<TopoDS_Shape>shapes;
        shapes.reserve(es.size());
        for(const auto& d : es){
            for(const auto& e : d){
                shapes.emplace_back(EdgeToShape(e));
            }
        }
        GeomToShape().ShapesToShape(shapes,s);
        return s;
    }
    static TopoDS_Shape TriangleToShape(const Triangle& t)
    {
        GeomToShape gt;
        TopoDS_Shape s,s0,s1,s2;
        gt.SegmentToShape(t.P0(),t.P1(),s0);
        gt.SegmentToShape(t.P1(),t.P2(),s1);
        gt.SegmentToShape(t.P2(),t.P0(),s2);
        gt.ShapesToShape(std::vector<TopoDS_Shape>{s0,s1,s2},s);
        return s;
    }
    static TopoDS_Shape TrianglesToShape(const vector<Triangle>& ts)
    {
        vector<TopoDS_Shape>shapes;
        shapes.resize(ts.size());
        GeomToShape gs;
        for(const auto& t : ts){
            shapes.push_back(TriangleToShape(t));
        }
        TopoDS_Shape s;
        gs.ShapesToShape(shapes,s);
        return s;
    }
    static TopoDS_Shape PointsToShape(const vector<vector<oft::Point>>& pts)
    {
        vector<TopoDS_Shape>shapes;
        shapes.resize(pts.size());

        for(auto& a : pts){
            for(auto& p : a){
                BRepBuilderAPI_MakeVertex v(gp_Pnt(p.X(),p.Y(),p.Z()));
                TopoDS_Shape s = v;
                shapes.emplace_back(s);
            }
        }
        TopoDS_Shape s;
        GeomToShape().ShapesToShape(shapes,s);
        return s;
    }

};

}
#endif // VIEWDATA_H

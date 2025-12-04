#ifndef VIEWTOOL_H
#define VIEWTOOL_H

#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>
#include"../../ViewWindow/MainWindow.h"
#include"../Common/ClStruct.h"

#pragma optimize("", off)
#pragma GCC optimize ("o0")

class ViewTool
{
public:
    ViewTool(){}
    static gp_Pnt OccPt(const oft::Point& p){return gp_Pnt (p.X(),p.Y(),p.Z());}
    static void RemoveAShape(Handle(AIS_Shape)& a)
    {
        _mainwind->myOccView->getContext()->Remove(a,true);
        Handle(AIS_Shape) a_;a = a_;
    }
    static void DisplayAShape(const Handle(AIS_Shape)&aShape,bool isClear)
    {
        if(isClear){_mainwind->myOccView->getContext()->RemoveAll(isClear);}
        _mainwind->myOccView->getContext()->Display(aShape,true);
        ///_mainwind->myOccView->myView->SetProj(V3d_Zpos);
        ///_mainwind->myOccView->myView->SetProj(V3d_XposYnegZpos);
        ///if(isClear){_mainwind->myOccView->CreateViewDir();}
        ///_mainwind->myOccView->update();
    }
    static Handle(AIS_Shape) ShapeToAis(const AIS_Shape& s,
                                        const Quantity_Color& c,double w)
    {
        Handle(AIS_Shape) aShape = new AIS_Shape(s);
        aShape->SetColor(c);
        aShape->SetWidth(w);
        return aShape;
    }
    static TopoDS_Shape ShapesToShape(const std::vector<TopoDS_Shape>& shapes)
    {
        TopoDS_Compound comp;
        BRep_Builder build;
        build.MakeCompound(comp);
        for(const auto &d : shapes){
            if(!d.IsNull()){
                build.Add(comp,d);
            }
        }
        TopoDS_Shape shape = comp;
        return shape;
    }
    static TopoDS_Shape PointToShape(const oft::Point& p)
    {
        BRepBuilderAPI_MakeVertex v(OccPt(p));
        TopoDS_Shape shape = v;
        return shape;
    }
    static TopoDS_Shape SegmentToShape(const oft::Point& sp,
                                       const oft::Point& ep)
    {
        if(sp.IsSameCoord3D(ep,PreErr_8)){return TopoDS_Shape();}
        TopoDS_Edge e = BRepBuilderAPI_MakeEdge(OccPt(sp),OccPt(ep));
        TopoDS_Shape shape = e;
        return shape;

    }
    static TopoDS_Shape EdgeToShape(const grm::ClEdge& e){
        if(e._sp == nullptr || e._ep == nullptr){return TopoDS_Shape();};
        TopoDS_Shape s = SegmentToShape(*e._sp,*e._ep);
        return s;
    }
    static TopoDS_Shape EdgesToShape(const std::vector<std::vector<grm::ClEdge>>&es){
        TopoDS_Shape s;
        std::vector<TopoDS_Shape>shapes;
        shapes.reserve(es.size());/**
        for(int i = 0;i < es.size();i++){
            for(int j = 0;j < es[i].size();j++){
                ///auto& e = es[i][j];
                ///shapes.emplace_back(EdgeToShape(e));
                ///std::cout<<i<<","<<j<<std::endl;
            }
        }*/
        for(const auto& d : es){
            for(const auto& e : d){
                shapes.emplace_back(EdgeToShape(e));
            }
        }
        s = ShapesToShape(shapes);
        return s;
    }
    static TopoDS_Shape CircleToShape(const oft::DefCircle& circle)
    {
        double r = circle._cirR;
        gp_Pnt cp = OccPt(circle._cirCp);
        gp_Circ c(gp_Ax2(cp, gp_Dir(0, 0, 1)), r);
        Handle_Geom_Circle geomCircle = new Geom_Circle(c);
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(geomCircle);
        TopoDS_Shape shape = edge;
        return shape;
    }
    static TopoDS_Shape TriangleToShape(const grm::Triangle& t)
    {
        auto s0 = SegmentToShape(t.P0(),t.P1());
        auto s1 = SegmentToShape(t.P1(),t.P2());
        auto s2 = SegmentToShape(t.P2(),t.P0());
        auto s = ShapesToShape(std::vector<TopoDS_Shape>{s0,s1,s2});
        return s;
    }
    static TopoDS_Shape TrianglesToShape(const std::vector<grm::Triangle>& ts)
    {
        std::vector<TopoDS_Shape>shapes;
        shapes.resize(ts.size());
        for(const auto& t : ts){
            shapes.emplace_back(TriangleToShape(t));
        }
        TopoDS_Shape s = ShapesToShape(shapes);
        return s;
    }
    static TopoDS_Shape PointsToShape(const std::vector<std::vector<oft::Point>>& pts)
    {
        std::vector<TopoDS_Shape>shapes;
        shapes.resize(pts.size());

        for(auto& a : pts){
            for(auto& p : a){
                BRepBuilderAPI_MakeVertex v(gp_Pnt(p.X(),p.Y(),p.Z()));
                TopoDS_Shape s = v;
                shapes.emplace_back(s);
            }
        }
        TopoDS_Shape s = ShapesToShape(shapes);
        return s;
    }
    static void DisplayPoint(const oft::Point& p,Quantity_Color c =
            Quantity_Color(0.6,0.6,0,Quantity_TOC_RGB),bool isClear = false)
    {
        auto s = PointToShape(p);
        auto as = ShapeToAis(s,c,1);
        DisplayAShape(as,isClear);
    }
    static void DisplaySegment(const oft::Point& sp,const oft::Point& ep,
                               Quantity_Color c = Quantity_Color(0,0,0.7,Quantity_TOC_RGB),
                               double w = 1, bool isLable = false,
                               int index = 0, bool isClear = false)
    {
        auto s = SegmentToShape(sp,ep);
        auto as = ShapeToAis(s,c,w);
        DisplayAShape(as,isClear);
    }
    static void DisplayTriangle(const grm::Triangle& t,
                                Quantity_Color c = Quantity_Color(0,0,0.7,Quantity_TOC_RGB),
                                double w = 1, bool isLable = false,
                                int index = 0, bool isClear = false)
    {
        auto s = TriangleToShape(t);
        auto as = ShapeToAis(s,c,w);
        DisplayAShape(as,isClear);
    }









};

#endif // VIEWTOOL_H
















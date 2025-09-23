#if 1
#include "DisplayGeom.h"
#include "GeomToShape.h"
#include <AIS_Line.hxx>

using namespace std;
using namespace grm;
using namespace oft;

#pragma optimize("", off)
#pragma GCC optimize ("O0")


Handle(AIS_Shape) DisplayGeom::ShapeToAis(const AIS_Shape& s,
                                          const Quantity_Color& c,double w)
{
    Handle(AIS_Shape) aShape = new AIS_Shape(s);
    aShape->SetColor(c);
    aShape->SetWidth(w);
    return aShape;
}
void DisplayGeom::DisplayAShape(const Handle(AIS_Shape)&aShape,bool isClear)
{
    if(isClear){_mainwind->myOccView->getContext()->RemoveAll(isClear);}
    _mainwind->myOccView->getContext()->Display(aShape,true);
    ///_mainwind->myOccView->myView->SetProj(V3d_Zpos);
    ///_mainwind->myOccView->myView->SetProj(V3d_XposYnegZpos);
    ///if(isClear){_mainwind->myOccView->CreateViewDir();}
    _mainwind->myOccView->update();
}
void DisplayGeom::DisplayShape(const AIS_Shape& s,
                               const Quantity_Color& c,
                               double w,bool isClear)
{
    Handle(AIS_Shape) aShape = ShapeToAis(s,c,w);
    DisplayAShape(aShape,isClear);
}
void DisplayGeom::DisplayLable(TCollection_ExtendedString& info,
                               const gp_Pnt& pos, bool isUp)
{
    Quantity_Color c(0.05,0.05,0.05,Quantity_TOC_RGB);
    Handle(AIS_TextLabel) l = new AIS_TextLabel();
    Handle(Prs3d_TextAspect) aspect = l->Attributes()->TextAspect();
    aspect->SetColor(c);
    aspect->SetFont("Arial");
    aspect->SetHeight(11);
    l->SetText(info);
    l->SetColor(c);
    l->SetPosition(pos);
    if(isUp){
        _mainwind->myOccView->getContext()->Display(l,false);
    }
    l = nullptr;
}
void DisplayGeom::DisplayPoint(const Point& P,
                               const Quantity_Color &c, bool isLable,
                               int index, bool isClear)
{
    gp_Pnt p_(P.X(),P.Y(),P.Z());
    BRepBuilderAPI_MakeVertex vertex(gp_Pnt (p_.X(),p_.Y(),p_.Z()));
    ///Quantity_Color c(0.8,0.8,0,Quantity_TOC_RGB);
    TopoDS_Shape shape = vertex;
    Handle(AIS_Shape)aShape = ShapeToAis(shape,c,1);
    DisplayAShape(aShape,isClear);
    if(isLable){
        string s = to_string(index);
        TCollection_ExtendedString info = s.c_str();
        gp_Pnt pos(P.X(),P.Y(),P.Z() + 0.01);
        DisplayLable(info,pos);
    }
    _mainwind->myOccView->fitAll();
}
void DisplayGeom::DisplaySegment(const Point& sp,
                                 const Point& ep,
                                 Quantity_Color &c,
                                 double w, bool isLable,
                                 int index, bool isClear)
{
    TopoDS_Shape shape;
    GeomToShape().SegmentToShape(sp,ep,shape);
    DisplayShape(shape,c,w,isClear);
}
void DisplayGeom::DisplayArc(const DefArc& arc,Quantity_Color &c,
                             double w, bool isLable,
                             int index, bool isClear)
{
    TopoDS_Shape shape;
    GeomToShape().ArcToShape(arc,shape);
    Handle(AIS_Shape) aShape = ShapeToAis(shape,c,w);
    DisplayAShape(aShape,isClear);
}
void DisplayGeom::DisplayElement(const DefElem& ele,
                                 Quantity_Color &c,
                                 double w, bool isClear)
{
    TopoDS_Shape shape;
    GeomToShape().ElementToShape(ele,shape);
    Handle(AIS_Shape)aShape = ShapeToAis(shape,c,w);
    DisplayAShape(aShape,isClear);
    _mainwind->myOccView->fitAll();
}
void DisplayGeom::DisplayLoop(const DefLoop& loop,
                              Quantity_Color &c,
                              double w, bool isIndex, bool isClear)
{
    TopoDS_Shape shape;
    GeomToShape().LoopToShape(loop,shape);
    Handle(AIS_Shape)aShape = ShapeToAis(shape,c,w);
    DisplayAShape(aShape,isClear);
    if(isIndex){/**
        ElemCalculate elecal;
        vector<pair<TCollection_ExtendedString,gp_Pnt>>infos;
        for (int i = 0; i < loop.ElemNum(); ++i) {
            Point p;
            elecal.ElementMidpoint(loop.IndexElem(i),p);
            string s = to_string(i);
            infos.push_back(std::make_pair(s.c_str(),gp_Pnt(p.X(),p.Y(),p.Z()
                                                            +0.05)));
        }
        for(auto& d : infos){
            DisplayLable(d.first,d.second);
        }*/
    }
    _mainwind->myOccView->fitAll();
}
void DisplayGeom::DisplayGeomArea(const GeomArea& area,bool isClear)
{
    if(isClear){_mainwind->myOccView->getContext()->RemoveAll(true);}
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color red(0.7,0,0,Quantity_TOC_RGB);
    Quantity_Color purple(0.6,0,0.6,Quantity_TOC_RGB);
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    Quantity_Color brown(0.5,0.2,0.0,Quantity_TOC_RGB);
    Quantity_Color cyan(0.,1,1,Quantity_TOC_RGB);
    Quantity_Color grey(0.02,0.06,0.06,Quantity_TOC_RGB);
    Quantity_Color grey_(0.1,0.2,0.2,Quantity_TOC_RGB);
    TopoDS_Shape s,s1;
    GeomToShape().LoopToShape(area._bndLoop,s);
    GeomToShape().LoopsToShape(area._vIslLoop,s1);
    DisplayShape(s,grey,1,false);
    DisplayShape(s1,grey,1.5,false);
    _mainwind->myOccView->fitAll();
}
void DisplayGeom::DisplayLoops(const std::vector<DefLoop>&vLoop,
                               Quantity_Color &c,
                               double w,bool isIndex, bool isClear)
{
    if(isClear){_mainwind->myOccView->getContext()->RemoveAll(true);}
    for(const auto& d : vLoop){DisplayLoop(d,c,w,isIndex,false);}
}
void DisplayGeom::DisplayPathNode(const shared_ptr<OffsetNode>& node,
                                  bool isClear, bool isClassify)
{
    if(node == nullptr)
        return;
    double w = 1;
    Quantity_Color c;
    GeomToShape().GetShapeColor(isClassify,false,node->_geneType,w,c);
    DisplayLoops(node->_vLoopData,c,w,false,isClear);
    for(auto& d : node->_vOffsetNode){
        DisplayPathNode(d,isClear,isClassify);
    }
}
void DisplayGeom::DisplayOffsetResult(const GeomArea& area,
                                      const shared_ptr<OffsetNode>& pathNode,
                                      bool isClear,bool isClassify)
{
    DisplayGeomArea(area,isClear);
    DisplayPathNode(pathNode,false,isClassify);
}

void DisplayGeom::DisplayModelDescrete(const TrianRes& tris,Quantity_Color c)
{
    vector<TopoDS_Shape>shapes;
    shapes.reserve(tris._triangles.size());
    for(const auto& d : tris._triangles){
        const auto& p0 = tris._vertexs[d[0]];
        const auto& p1 = tris._vertexs[d[1]];
        const auto& p2 = tris._vertexs[d[2]];
        gp_Pnt p0_(p0.X(),p0.Y(),p0.Z());
        gp_Pnt p1_(p1.X(),p1.Y(),p1.Z());
        gp_Pnt p2_(p2.X(),p2.Y(),p2.Z());
        TopoDS_Edge e0 = BRepBuilderAPI_MakeEdge(p0_,p1_);
        TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(p1_,p2_);
        TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(p2_,p0_);
        shapes.emplace_back(e0);
        shapes.emplace_back(e1);
        shapes.emplace_back(e2);
    }
    TopoDS_Shape shape;
    GeomToShape().ShapesToShape(shapes,shape);
    ///Quantity_Color grey(0.02,0.06,0.06,Quantity_TOC_RGB);
    DisplayShape(shape,c,1,false);
}

void DisplayGeom::DisplayTriangles(const vector<Triangle>& ts,Quantity_Color &c,
                                   double w,bool isClear)
{
    vector<TopoDS_Shape>shapes;
    shapes.reserve(ts.size());
    GeomToShape gs;
    for(const auto& t : ts){
        shapes.push_back(gs.TriangleToShape(t));
    }
    TopoDS_Shape shape;
    gs.ShapesToShape(shapes,shape);
    DisplayShape(shape,c,w,isClear);
}














#endif

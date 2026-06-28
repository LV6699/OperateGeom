#include "ViewGeometry.h"
#include "GeomToShapes.h"
#include <AIS_Line.hxx>
using namespace std;
using namespace OffsetStruct;
#pragma optimize("", off)
#pragma GCC optimize ("O0")
Handle(AIS_Shape) ViewGeometry::ShapeToAis(const AIS_Shape& s,
                                           const Quantity_Color& c,double w)
{
    Handle(AIS_Shape) aShape = new AIS_Shape(s);
    aShape->SetColor(c);
    aShape->SetWidth(w);
    return aShape;
}
void ViewGeometry::DisplayShape(const Handle(AIS_Shape)&aShape,bool isClear)
{
    if(isClear){_mainwind->myOccView->getContext()->RemoveAll(true);}
    _mainwind->myOccView->getContext()->Display(aShape,true);
    ///_mainwind->myOccView->myView->SetProj(V3d_Zpos);
    _mainwind->myOccView->CreateViewDir();
    _mainwind->myOccView->update();
}
void ViewGeometry::DisplayLable(TCollection_ExtendedString& info,
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
void ViewGeometry::DisplayPoint(const Point& P,
                                const Quantity_Color &c, bool isLable,
                                int index, bool isClear)
{
    gp_Pnt p_(P.X(),P.Y(),P.Z());
    BRepBuilderAPI_MakeVertex vertex(gp_Pnt (p_.X(),p_.Y(),p_.Z()));
    ///Quantity_Color c(0.8,0.8,0,Quantity_TOC_RGB);
    TopoDS_Shape shape = vertex;
    Handle(AIS_Shape)aShape = ShapeToAis(shape,c,1);
    DisplayShape(aShape,isClear);
    if(isLable){
        string s = to_string(index);
        TCollection_ExtendedString info = s.c_str();
        gp_Pnt pos(P.X(),P.Y(),P.Z() + 0.01);
        DisplayLable(info,pos);
    }
}
void ViewGeometry::DisplaySegment(const Point& sp,
                                  const Point& ep,
                                  Quantity_Color &c,
                                  double w, bool isLable,
                                  int index, bool isClear)
{
    if(sp.DistSquare2D(ep) < PreErr5_12)
        return;
    gp_Pnt p0(sp.X(),sp.Y(),sp.Z()),p1(ep.X(),ep.Y(),ep.Z());
    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(p0,p1);
    Handle(AIS_Shape)aShape = ShapeToAis(shape,c,w);/**
    if(isDotted){
        Handle(Prs3d_LineAspect) lineAspect = new Prs3d_LineAspect(c,Aspect_TOL_DASH,w);
        lineAspect->Aspect()->SetMarkerScale(1.0);
        lineAspect->SetTypeOfLine(Aspect_TOL_DOTDASH);
        aShape->Attributes()->SetLineAspect(lineAspect);
    }*/
    DisplayShape(aShape,isClear);
    if(isLable){
        string s = to_string(index);
        TCollection_ExtendedString info = s.c_str();
        gp_Pnt pos((sp.X()+ep.X())/2,(sp.Y()+ep.Y())/2,
                   (sp.Z()+ep.Z())/2 + 0.01);
        DisplayLable(info,pos);
    }
}
void ViewGeometry::DisplayElement(const DefElem& ele,
                                  Quantity_Color &c,
                                  double w, bool isClear)
{
    GeomToShape geo;
    TopoDS_Shape shape;
    geo.ElementToShape(ele,shape);
    Handle(AIS_Shape)aShape = ShapeToAis(shape,c,w);
    DisplayShape(aShape,isClear);
}
void ViewGeometry::DisplayLoop(const DefLoop& loop,
                               Quantity_Color &c,
                               double w, bool isIndex, bool isClear)
{
    GeomToShape geo;
    vector<TopoDS_Shape>shapes;
    for (int i = 0; i < loop.Size(); ++i) {
        TopoDS_Shape s;
        geo.ElementToShape(loop.IndexElem(i),s);
        shapes.push_back(s);
    }
    TopoDS_Compound comp;
    BRep_Builder build;
    build.MakeCompound(comp);
    for(auto &d : shapes){
        if(d.IsNull())
            continue;
        build.Add(comp,d);
    }
    Handle(AIS_Shape)aShape = ShapeToAis(comp,c,w);
    DisplayShape(aShape,isClear);
    if(isIndex){
        ElemCalculate elecal;
        vector<pair<TCollection_ExtendedString,gp_Pnt>>infos;
        for (int i = 0; i < loop.Size(); ++i) {
            Point p;
            elecal.ElementMidpoint(loop.IndexElem(i),p);
            string s = to_string(i);
            infos.push_back(std::make_pair(s.c_str(),gp_Pnt(p.X(),p.Y(),p.Z()+0.05)));
        }
        for(auto& d : infos){
            DisplayLable(d.first,d.second);
        }
    }
}
void ViewGeometry::ViewGeometryArea(const GeomArea& biLoop,
                                    Quantity_Color &c,
                                    double w,bool isClear)
{
    if(isClear){_mainwind->myOccView->getContext()->RemoveAll(true);}
    double w1 = 1;
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    DisplayLoop(biLoop._bndLoop,black,w1,isClear);
    for (int i = 0; i < biLoop._vIslLoop.size(); ++i) {
        DisplayLoop(biLoop._vIslLoop[i],c,w,false);
    }
}
void ViewGeometry::DisplayLoops(const std::vector<DefLoop>&vLoop,
                                Quantity_Color &c,
                                double w,bool isIndex, bool isClear)
{
    for(const auto& d : vLoop){
        DisplayLoop(d,c,w,isIndex,isClear);
    }
}
void ViewGeometry::DisplayRootNodeShape(const GeomArea& biLoop,
                                        const shared_ptr<OffsetNode>& rootNode,
                                        Quantity_Color &c, double w,
                                        bool isClear,bool isClassify)
{
}

#include "CreateTool.h"
#include "../../OperateView/DisplayGeom.h"

bool GetArc(const gp_Dir& n,const gp_Pnt& sp,const gp_Pnt& ep,
            const gp_Pnt& cp,bool iscw,TopoDS_Edge& edge)
{
    gp_Ax2 sys(cp, n);
    double r = sp.Distance(cp);
    double r1 = ep.Distance(cp);
    gp_Circ circle(sys,r);
    auto sp1 = sp,ep1 = ep;
    if(!iscw){std::swap(sp1,ep1);}
    GC_MakeArcOfCircle arcMaker(circle, sp1, ep1,true);
    Handle(Geom_TrimmedCurve)trim = arcMaker.Value();
    edge = BRepBuilderAPI_MakeEdge(trim);
    return true;
}

TopoDS_Shape CreateTool::GetToolShape(const ofts::Point& p,const grm::DefTool& t)
{
    gp_Dir xDir(0, 1, 0);
    gp_Pnt p0(p.X(),p.Y(),p.Z());
    grm::ToolType type = t._type;
    BRepBuilderAPI_MakeWire wireBuilder;
    if(type == grm::ToolType::PlaneEnd){
        auto p1 = p0;p1.SetX(p0.X() + t.R());
        auto p2 = p1;p2.SetZ(p2.Z() + t._lenth);
        auto p3 = p2;p3.SetX(p2.X() - t.R());
        std::vector<gp_Pnt>ps{p0,p1,p2,p3,p0};
        for (size_t i = 0; i < ps.size() - 1; i++) {
            wireBuilder.Add(BRepBuilderAPI_MakeEdge(ps[i], ps[i+1]));
        }
    }else if(type == grm::ToolType::BallNoseEnd){
        auto ep = p0;ep.SetX(p0.X() + t.R());ep.SetZ(ep.Z() + t.R());
        auto cp = p0;cp.SetZ(p0.Z() + t.R());
        TopoDS_Edge edge;
        if(!GetArc(xDir,p0,ep,cp,false,edge)){
            return TopoDS_Shape();
        }
        auto p1 = ep;p1.SetZ(ep.Z() + t._lenth - t.R());
        auto p2 = p1;p2.SetX(p2.X() - t.R());
        wireBuilder.Add(edge);
        std::vector<gp_Pnt>ps{ep,p1,p2,p0};
        for (size_t i = 0; i < ps.size() - 1; i++) {
            wireBuilder.Add(BRepBuilderAPI_MakeEdge(ps[i], ps[i+1]));
        }
    }else if(type == grm::ToolType::RoundNoseEnd){
        auto sp = p0;sp.SetX(sp.X() + t._br);
        auto cp = sp;cp.SetZ(cp.Z() + t._cr);
        auto ep = cp;ep.SetX(ep.X() + t._cr);
        TopoDS_Edge edge;
        if(!GetArc(xDir,p0,ep,cp,false,edge)){
            return TopoDS_Shape();
        }
        auto p1 = ep;p1.SetZ(ep.Z() + t._lenth - t._cr);
        auto p2 = p1;p2.SetX(p0.X());
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p0, sp));
        wireBuilder.Add(edge);
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(ep, p1));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p1, p2));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p2, p0));
        
    }else if(type == grm::ToolType::V_Cutter){
        auto p1 = p0;p1.SetX(p1.X() + t.R());p1.SetZ(p1.Z() + t._h);
        auto p2 = p1;p2.SetZ(p2.Z() + t._lenth - t._h);
        auto p3 = p2;p3.SetX(p3.X() - t.R());
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p0, p1));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p1, p2));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p2, p3));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p3, p0));
    }else if(type == grm::ToolType::TaperBall){
        auto ep = p0;ep.SetX(ep.X() + t._br);ep.SetZ(ep.Z() + t._br_down_h);
        auto cp = p0;cp.SetZ(cp.Z() + t._cr);
        TopoDS_Edge edge;
        if(!GetArc(xDir,p0,ep,cp,false,edge)){
            return TopoDS_Shape();
        }
        auto p1 = p0;p1.SetX(p1.X() + t.R());p1.SetZ(p1.Z() + t._h);
        auto p2 = p1;p2.SetZ(p2.Z() + t._lenth - t._h);
        auto p3 = p2;p3.SetX(p3.X() - t.R());
        wireBuilder.Add(edge);
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(ep, p1));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p1, p2));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p2, p3));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p3, p0));
        /*DisplayGeom().DisplayShape(edge,_colors[0],1);
        DisplayGeom().DisplayPoint(ofts::Point(ep.X(),ep.Y(),ep.Z()),_colors[1]);
        DisplayGeom().DisplayPoint(ofts::Point(p1.X(),p1.Y(),p1.Z()),_colors[2]);
        DisplayGeom().DisplayPoint(ofts::Point(p2.X(),p2.Y(),p2.Z()),_colors[3]);
        DisplayGeom().DisplayPoint(ofts::Point(p3.X(),p3.Y(),p3.Z()),_colors[4]);*/
    }else if(type == grm::ToolType::TaperEnd){
        auto p1 = p0;p1.SetX(p1.X() + t._br);
        auto p2 = p0;p2.SetX(p2.X() + t.R());p2.SetZ(p2.Z() + t._h);
        auto p3 = p2;p3.SetX(p3.X() - t.R());
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p0, p1));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p1, p2));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p2, p3));
        wireBuilder.Add(BRepBuilderAPI_MakeEdge(p3, p0));
    }
    if (!wireBuilder.IsDone()) {
        return TopoDS_Shape();
    }
    TopoDS_Wire wire = wireBuilder.Wire();
    //DisplayGeom().DisplayShape(wire,_colors[4],1);
    //_mainwind->myOccView->fitAll();
    BRepBuilderAPI_MakeFace faceBuilder(wire);
    if (!faceBuilder.IsDone()) {
        return TopoDS_Shape();
    }
    TopoDS_Face face = faceBuilder.Face();
    //DisplayGeom().DisplayShape(face,_colors[4],1);
    //_mainwind->myOccView->fitAll();
    gp_Ax1 rotationAxis(p0, gp_Dir(0,0,1));
    BRepPrimAPI_MakeRevol revol(face, rotationAxis, 2 * M_PI);
    if (!revol.IsDone()) {
        return TopoDS_Shape();
    }
    auto shape = revol.Shape();
    return shape;    
}



























































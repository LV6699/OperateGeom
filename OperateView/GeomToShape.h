#ifndef GEOMTOSHAPE_H
#define GEOMTOSHAPE_H

#include"../OffsetStruct/CommonFile/DataStructure.h"
#include"../ViewWindow/MainWindow.h"
#include"../CutterLocation/Solution/CutterLocationZ.h"

class GeomToShape
{
public:
    GeomToShape(){}
    void GetShapeColor(bool isClassify,bool isOriProf,
                       ToolPathType pathType,double &width,
                       Quantity_Color& color);
    gp_Pnt ToOccPt(const oft::Point& p){return gp_Pnt (p.X(),p.Y(),p.Z());}
    void PointToShape(const oft::Point& p,TopoDS_Shape &shape)
    {
        BRepBuilderAPI_MakeVertex v(ToOccPt(p));
        shape = v;
    }
    void SegmentToShape(const oft::Point& sp,const oft::Point& ep,
                        TopoDS_Shape &shape)
    {
        if(sp.IsSameCoord3D(ep,PreErr5_8))
            return;
        TopoDS_Edge e = BRepBuilderAPI_MakeEdge(ToOccPt(sp),ToOccPt(ep));
        shape = e;
    }
    void ArcToShape(const oft::DefArc& arc_,
                    TopoDS_Shape &shape)
    {
        auto arc = arc_;
        arc.ChoiceSetRadius(PreErr5_2);
        const oft::Point& sp = arc._arcSp;
        const oft::Point& ep = arc._arcEp;
        const oft::Point& cp = arc._arcCp;
        if(sp.IsSameCoord3D(ep,PreErr5_8) ||
                sp.IsSameCoord3D(cp,PreErr5_8))
            return;
        const OffsetEnum::RotDir& rot = arc._arcRot;
        const double& r = arc._arcR;
        gp_Pnt sp_ = ToOccPt(sp);
        gp_Pnt ep_ = ToOccPt(ep);
        gp_Pnt cp_ = ToOccPt(cp);
        bool isCW = rot == CLOCKWISE ? true : false;
#if 1
        gp_Circ circle(gp_Ax2(cp_, gp_Dir(0, 0, 1)), r);
        if(isCW){
            std::swap(sp_, ep_);
        }
        GC_MakeArcOfCircle arcMaker(circle, sp_, ep_, true);
        Handle(Geom_TrimmedCurve) trimmedCurve = arcMaker.Value();
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(trimmedCurve);
#else
        ///确定圆的轴系统
        ///计算平面法向量 (通过起点-圆心和终点-圆心的叉积)
        gp_Vec v1(cp_, sp_);
        gp_Vec v2(cp_, ep_);
        gp_Dir normal = v1.Crossed(v2).Normalized();
        ///创建轴系统 (圆心位置，法向量方向)
        gp_Ax2 axis(cp_, normal);
        ///创建圆
        gp_Circ circle(axis, r);
        ///计算起点和终点参数
        double u1 = ElCLib::Parameter(circle, sp_);
        double u2 = ElCLib::Parameter(circle, ep_);
        ///调整参数顺序以匹配旋转方向
        if (isCW) {
            if (u2 > u1) u2 -= 2 * M_PI;
        }else {
            if (u2 < u1) u2 += 2 * M_PI;
        }
        ///创建圆弧段
        Handle(Geom_TrimmedCurve) arc__ = GC_MakeArcOfCircle(circle, u1, u2, !
                                                             isCW);
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(arc__);///创建拓扑边
#endif
        shape = edge;
    }
    void CircleToShape(const oft::DefCircle& circle,
                       TopoDS_Shape &shape)
    {
        double r = circle._cirR;
        gp_Pnt cp = ToOccPt(circle._cirCp);
        gp_Circ c(gp_Ax2(cp, gp_Dir(0, 0, 1)), r);
        Handle_Geom_Circle geomCircle = new Geom_Circle(c);
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(geomCircle);
        shape = edge;
    }
    void ElementToShape(const oft::DefElem& ele,
                        TopoDS_Shape &shape)
    {
        switch (ele.ElemeType()) {
        case OffsetEnum::LINETYPE:
            SegmentToShape(ele._segment._segSp,ele._segment._segEp,shape);break;
        case OffsetEnum::ARCTYPE:
            ArcToShape(ele._arc,shape);break;
        case OffsetEnum::CIRCLETYPE:
            CircleToShape(ele._circle,shape);break;
        default:
            break;
        }
    }
    void ShapesToShape(const std::vector<TopoDS_Shape>& shapes,
                       TopoDS_Shape &shape)
    {
        TopoDS_Compound comp;
        BRep_Builder build;
        build.MakeCompound(comp);
        for(const auto &d : shapes){
            if(!d.IsNull()){
                build.Add(comp,d);
            }
        }
        shape = comp;
    }
    void LoopToShape(const oft::DefLoop& loop,TopoDS_Shape &shape);
    void LoopsToShape(const std::vector<oft::DefLoop> &loops, TopoDS_Shape &shape);
    void GeomAreaToShape(const oft::GeomArea& area,TopoDS_Shape& shape);

    TopoDS_Shape TriangleToShape(const grm::Triangle& t)
    {
        TopoDS_Shape s,s0,s1,s2;
        SegmentToShape(t.P0(),t.P1(),s0);
        SegmentToShape(t.P1(),t.P2(),s1);
        SegmentToShape(t.P2(),t.P0(),s2);
        ShapesToShape(std::vector<TopoDS_Shape>{s0,s1,s2},s);
        return s;
    }









    
};

#endif


#if 0
#include "GeomToShape.h"
#include<ElCLib.hxx>
void GeomToShape::GetShapeColor(bool isClassify,bool isOriProf,
                                ToolPathType pathType,double &width,
                                Quantity_Color& color)
{
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
    if(isOriProf){
        color = grey;
        width = 1;
        switch (pathType) {
        case OriginBndPath:{
            break;
        }
        case OriginIslPath:{
            width = 1.5;
            break;
        }
        case BlockPath:{
            width = 1.5;
            color = grey_;
            ///width = 1.5;
            break;
        }
        default:
            color = brown;
            width = 1.5;
            break;
        }/**
            if(pathType == OriginBndPath ||
                    pathType == OriginIslPath){
                color = red;
            }
            else if(pathType == BlockPath){
                color = orange;
            }*/
        return;
    }
    width = 1.5;
    if(!isClassify){
        color = green;
        return;
    }
    switch (pathType) {
    case BndOffsetPath:
        color = blue;
        break;
    case BndOftLocalPath:
        color = red;
        break;
    case IslOffsetPath:
        color = purple;
        break;
    case IslOftLocalPath:
        color = red;
        break;
    case BndIslMerPath:
        color = green;
        break;
    case IslIslMerPath:
        color = orange;
        break;
    case BlockPath:
        color = orange;
        break;
    case SimulateTool:
        color = green;
        break;
    case UnknowPath:
        color = cyan;
        width = 1;
        break;
    default:
        break;
    }
}
/**
void GeomToShape::ElementToShape(const DefElem& ele,
                                 TopoDS_Shape &shape)
{
    if(ele._type == LINETYPE){
        if(ele._segment._segSp.IsSameCoord3D(
                    ele._segment._segEp,PreErr5_6))
            return;
        gp_Pnt sp(ele._segment._segSp._x,
                  ele._segment._segSp._y,ele._segment._segSp._z);
        gp_Pnt ep(ele._segment._segEp._x,
                  ele._segment._segEp._y,ele._segment._segEp._z);
        TopoDS_Edge ost_Edge = BRepBuilderAPI_MakeEdge(ep,sp);
        shape = ost_Edge;
    }
    else if(ele._type == ARCTYPE){
        auto arc = ele._arc;
        arc.OptInitR(PreErr5_2);
        Point& sp = arc._arcSp;
        Point& ep = arc._arcEp;
        Point& cp = arc._arcCp;
        if(sp.IsSameCoord3D(ep,PreErr5_8) ||
                sp.IsSameCoord3D(cp,PreErr5_8))
            return;
        RotDir& rot = arc._arcRot;
        double& r = arc._arcR;
        gp_Pnt sp_(sp._x,sp._y,sp._z);
        gp_Pnt ep_(ep._x,ep._y,ep._z);
        gp_Pnt cp_ (cp._x,cp._y,cp._z);
        bool isCW = rot == CLOCKWISE ? true : false;
#if 0
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
        Handle(Geom_TrimmedCurve) arc_ = GC_MakeArcOfCircle(circle, u1, u2, !isCW);
        ///创建拓扑边
        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(arc_);
#endif
        shape = edge;
    }
    else if(ele._type == CIRCLETYPE){
        double r = ele._circle._cirR;
        gp_Pnt cp (ele._circle._cirCp._x,
                   ele._circle._cirCp._y,
                   ele._circle._cirCp._z);
        gp_Circ circle(gp_Ax2(cp, gp_Dir(0, 0, 1)), r);
        Handle_Geom_Circle geomCircle = new Geom_Circle(circle);
        TopoDS_Edge circleEdge = BRepBuilderAPI_MakeEdge(geomCircle);
        shape = circleEdge;
    }
}*/
void GeomToShape::LoopToShape(const DefLoop &loop, TopoDS_Shape &shape)
{
    std::vector<TopoDS_Shape>shapes;
    shapes.reserve(loop.Size());
    for(auto& d : loop.m_vElem){
        TopoDS_Shape s;
        ElementToShape(d,s);
        if(!s.IsNull()){
            shapes.emplace_back(s);
        }
    }
    ShapesToShape(shapes,shape);
}
void GeomToShape::LoopsToShape(const std::vector<DefLoop> &loops,
                               TopoDS_Shape &shape)
{
    std::vector<TopoDS_Shape>shapes;
    shapes.reserve(loops.size());
    for(auto& d : loops){
        TopoDS_Shape s;
        LoopToShape(d,s);
        if(!s.IsNull()){
            shapes.emplace_back(s);
        }
    }
    ShapesToShape(shapes,shape);
}
void GeomToShape::GeomAreaToShape(const GeomArea& area,TopoDS_Shape& shape)
{
    TopoDS_Shape s,s1;
    LoopToShape(area._bndLoop,s);
    LoopsToShape(area._vIslLoop,s1);
    ShapesToShape(std::vector<TopoDS_Shape>{s,s1},shape);
}
#endif

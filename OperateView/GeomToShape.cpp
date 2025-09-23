#if 1
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
        }
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
void GeomToShape::LoopToShape(const oft::DefLoop& loop,TopoDS_Shape &shape)
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
void GeomToShape::LoopsToShape(const std::vector<oft::DefLoop> &loops, TopoDS_Shape &shape)
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
void GeomToShape::GeomAreaToShape(const oft::GeomArea& area,TopoDS_Shape& shape)
{
    TopoDS_Shape s,s1;
    LoopToShape(area._bndLoop,s);
    LoopsToShape(area._vIslLoop,s1);
    ShapesToShape(std::vector<TopoDS_Shape>{s,s1},shape);
}



#endif

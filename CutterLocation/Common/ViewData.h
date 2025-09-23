#ifndef VIEWDATA_H
#define VIEWDATA_H

#include "ClStruct.h"
#include "../../OperateView/GeomToShape.h"

namespace grm{

class ViewData
{
public:
    ViewData(){}

    static TopoDS_Shape EdgeToShape(const ClEdge& e){
        if(e._sp == nullptr || e._ep == nullptr){return TopoDS_Shape();};
        TopoDS_Shape s;
        GeomToShape().SegmentToShape(*e._sp,*e._ep,s);
        return s;

    }
    static void EdgesToShape(const std::vector<std::vector<ClEdge>>&es,
                             TopoDS_Shape& shape){
        std::vector<TopoDS_Shape>shapes;
        shapes.reserve(es.size());
        for(const auto& d : es){
            for(const auto& e : d){
                shapes.emplace_back(EdgeToShape(e));
            }
        }
        GeomToShape().ShapesToShape(shapes,shape);
    }


};

}
#endif // VIEWDATA_H

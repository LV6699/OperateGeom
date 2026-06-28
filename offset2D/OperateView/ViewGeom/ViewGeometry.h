#ifndef ViewGeometry_H
#define ViewGeometry_H
#include"../../ToolTrajectoryAlg/CommonFile/DataStructure.h"
#include"../ViewWind/mainwindow.h"
class ViewGeometry
{
public:
    ViewGeometry(){}
    Handle(AIS_Shape)ShapeToAis(const AIS_Shape& s,
                                const Quantity_Color& c,double w);
    void DisplayShape(const Handle(AIS_Shape)&aShape,bool isClear = false);
    void DisplayLable(TCollection_ExtendedString& info, const gp_Pnt& pos,bool isUp = true);
    void DisplayPoint(const  OffsetStruct::Point& P,const Quantity_Color &c,
                      bool isLable = false,
                      int index = -1,bool isClear = false);
    void DisplaySegment(const  OffsetStruct::Point& sp,
                        const  OffsetStruct::Point& ep,
                        Quantity_Color &c,
                        double w, bool isLable = false,
                        int index = false,
                        bool isClear = false);
    void DisplayElement(const OffsetStruct::DefElem& ele,Quantity_Color& c,
                        double w,bool isClear = false);
    void DisplayLoop(const  OffsetStruct::DefLoop& loop, Quantity_Color& c,
                     double w,bool isIndex = false,bool isClear = false);
    void DisplayGeomArea(const  OffsetStruct::GeomArea& biLoop,
                         Quantity_Color &c,double w,
                         bool isClear = false);
    void DisplayLoops(const std::vector< OffsetStruct::DefLoop>&vLoop,
                      Quantity_Color& c,double w,
                      bool isIndex = false,bool isClear = false);
    void DisplayRootNodeShape(const  OffsetStruct::GeomArea& biLoop,
                              const shared_ptr< OffsetStruct::OffsetNode>& rootNode,
                              Quantity_Color& c,double w,bool isClear = false,
                              bool isClassify = false);
};
#endif // ViewGeometry_H

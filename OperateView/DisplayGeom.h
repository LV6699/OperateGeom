#if 1
#ifndef DISPLAYGEOM_H
#define DISPLAYGEOM_H

//#include"../CommonFile/ModelStruct.h"
//#include"../ViewWindow/MainWindow.h"
#include"../ViewWindow/OccHeader.h"
#include"../OffsetStruct/CommonFile/DataStructure.h"
//#include"../CutterLocation/Solution/CutLocateZ.h"

class DisplayGeom
{
public:
    DisplayGeom(){}
    Handle(AIS_Shape)ShapeToAis(const AIS_Shape& s,
                                const Quantity_Color& c,double w);
    void RemoveAShape(Handle(AIS_Shape)& a);

    void DisplayAShape(const Handle(AIS_Shape)&aShape,bool isClear = false);

    void DisplayShape(const AIS_Shape& s,
                      const Quantity_Color& c,
                      double w,bool isClear = false);

    void DisplayLable(TCollection_ExtendedString& info,
                      const gp_Pnt& pos,bool isUp = true);

    void DisplayPoint(const ofts::Point& P,const Quantity_Color &c,
                      bool isLable = false,
                      int index = -1,bool isClear = false);

    void DisplaySegment(const ofts::Point& sp,
                        const ofts::Point& ep,
                        Quantity_Color &c,
                        double w, bool isLable = false,
                        int index = false,
                        bool isClear = false);
    void DisplayArc(const ofts::DefArc& arc,Quantity_Color &c,
                    double w, bool isLable = false,
                    int index = false, bool isClear = false);
    void DisplayElement(const ofts::DefElem& ele,Quantity_Color& c,
                        double w,bool isClear = false);

    void DisplayLoop(const ofts::DefLoop& loop, Quantity_Color& c,
                     double w,bool isIndex = false,bool isClear = false);

    void DisplayGeomArea(const ofts::GeomArea& area,
                         bool isClear = false);
    void DisplayLoops(const std::vector<ofts::DefLoop>&vLoop,
                      Quantity_Color& c,double w,
                      bool isIndex = false,bool isClear = false);
    void DisplayPathNode(const std::shared_ptr<ofts::OffsetNode>& node,
                         bool isClear,bool isClassify);
    void DisplayOffsetResult(const ofts::GeomArea& area,
                             const std::shared_ptr<ofts::OffsetNode>& pathNode,
                             bool isClear = false,
                             bool isClassify = false);

};


#endif // DISPLAYGEOM_H
#endif














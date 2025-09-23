#if 1
#ifndef DISPLAYGEOM_H
#define DISPLAYGEOM_H

#include"../OffsetStruct/CommonFile/DataStructure.h"
#include"../CommonFile/ModelStruct.h"
#include"../ViewWindow/MainWindow.h"
#include"../CutterLocation/Solution/CutterLocationZ.h"

class DisplayGeom
{
public:
    DisplayGeom(){}
    Handle(AIS_Shape)ShapeToAis(const AIS_Shape& s,
                                const Quantity_Color& c,double w);
    void DisplayAShape(const Handle(AIS_Shape)&aShape,bool isClear = false);
    void DisplayShape(const AIS_Shape& s,
                      const Quantity_Color& c,
                      double w,bool isClear = false);
    void DisplayLable(TCollection_ExtendedString& info, const gp_Pnt& pos,bool
                      isUp = true);
    void DisplayPoint(const oft::Point& P,const Quantity_Color &c,
                      bool isLable = false,
                      int index = -1,bool isClear = false);
    void DisplaySegment(const oft::Point& sp,
                        const oft::Point& ep,
                        Quantity_Color &c,
                        double w, bool isLable = false,
                        int index = false,
                        bool isClear = false);
    void DisplayArc(const oft::DefArc& arc,Quantity_Color &c,
                    double w, bool isLable = false,
                    int index = false, bool isClear = false);
    void DisplayElement(const oft::DefElem& ele,Quantity_Color& c,
                        double w,bool isClear = false);

    void DisplayLoop(const oft::DefLoop& loop, Quantity_Color& c,
                     double w,bool isIndex = false,bool isClear = false);

    void DisplayGeomArea(const oft::GeomArea& area,
                         bool isClear = false);
    void DisplayLoops(const std::vector<oft::DefLoop>&vLoop,
                      Quantity_Color& c,double w,
                      bool isIndex = false,bool isClear = false);
    void DisplayPathNode(const std::shared_ptr<oft::OffsetNode>& node,
                         bool isClear,bool isClassify);
    void DisplayOffsetResult(const oft::GeomArea& area,
                             const std::shared_ptr<oft::OffsetNode>& pathNode,
                             bool isClear = false,
                             bool isClassify = false);

    void DisplayModelDescrete(const grm::TrianRes& tris, Quantity_Color c);

    void DisplayTriangles(const vector<grm::Triangle>& ts,Quantity_Color &c,
                          double w,bool isClear = false);

};


#endif // DISPLAYGEOM_H
#endif














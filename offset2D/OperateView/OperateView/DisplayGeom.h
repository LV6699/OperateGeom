#if 0
#ifndef DISPLAYGEOM_H
#define DISPLAYGEOM_H
#include"../../ToolTrajectoryAlg/CommonFile/Header.h"
#if 1
#include"../../../../ViewWindow/MainWindow/mainwindow.h"
#else
#include"../../../ViewWindow/MainWindow/mainwindow.h"
#endif
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
    void DisplayLable(TCollection_ExtendedString& info, const gp_Pnt& pos,bool isUp = true);
    void DisplayPoint(const Point& P,const Quantity_Color &c,
                      bool isLable = false,
                      int index = -1,bool isClear = false);
    void DisplaySegment(const Point& sp,
                        const Point& ep,
                        Quantity_Color &c,
                        double w, bool isLable = false,
                        int index = false,
                        bool isClear = false);
    void DisplayArc(const DefArc& arc,Quantity_Color &c,
                    double w, bool isLable = false,
                    int index = false, bool isClear = false);
    void DisplayElement(const DefElem& ele,Quantity_Color& c,
                        double w,bool isClear = false);
    void DisplayLoop(const DefLoop& loop, Quantity_Color& c,
                     double w,bool isIndex = false,bool isClear = false);
    void DisplayGeomArea(const GeomArea& area,
                         bool isClear = false);
    void DisplayLoops(const std::vector<DefLoop>&vLoop,
                      Quantity_Color& c,double w,
                      bool isIndex = false,bool isClear = false);
    void DisplayPathNode(const shared_ptr<OffsetNode>& node,
                         bool isClear,bool isClassify);
    void DisplayOffsetResult(const GeomArea& area,
                             const shared_ptr<OffsetNode>& pathNode,
                             bool isClear = false,
                             bool isClassify = false);
};
#endif // DISPLAYGEOM_H
#endif

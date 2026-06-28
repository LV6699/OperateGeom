#ifndef TRANSTOSHAPE_H
#define TRANSTOSHAPE_H
#include"../MainWindow/mainwindow.h"
class TransToShape
{
public:
    TransToShape(){}
    void TranslatePointToShape(const Point& P,Quantity_Color color,
                               Handle(AIS_Shape)&shape);
    void TranslateEleToAIS_Shape(DefElem ele,
                                 Quantity_Color color,
                                 double width,bool& isSucccess,
                                 Handle(AIS_Shape)&shape);
    void TranslateLoopToShape(const DefLoop& loop,Quantity_Color color,
                               vector<Handle(AIS_Shape)>&vShape);
    void GetShapeColor(bool isClassify, bool isOriProf,
                       ToolPathType pathType,double &width,
                       Quantity_Color& color);
public slots:
    void GetLoopDisplayShape(const DefLoop& loop, bool isClassify,
                             bool isOriProf,
                             ToolPathType pathType,
                             int nodeIdx, LoopShape& loopShape);
    void GetBndIslLoopShape(const GeomArea &bndIslLoop,
                            ToolPathType pathType,bool isOriLoop,
                            bool isClassify,
                            vector<Handle(AIS_Shape)>&vBndShape,
                            vector<vector<Handle(AIS_Shape)>>&vIslShape);
    void GetPairLoopDisplay(const GeomArea &bndIslLoop, bool isOriLoop,
                         bool isClear);
    void GetLoopsDisplayShape(const vector<DefLoop>&vLoop,
                              ToolPathType pathType, int nodeIdx, bool isOriProf,
                              bool isClassify,
                              NodeShape& curNodeShape);
    void GetOffsetNodeLoopShape(const shared_ptr<OffsetNode> &rootNode,
                                bool isClassify,int curNodeIdx);
    void GetOffsetNodeLoopShape(const shared_ptr<OffsetNode> &rootNode,
                                bool isClassify, int curNodeIdx,
                                vector<NodeShape> &vNodeShape);
};
#endif // TRANSTOSHAPE_H
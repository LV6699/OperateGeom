#ifndef DISPLAYTOOLPATH_H
#define DISPLAYTOOLPATH_H
#include "TransToShape.h"
class DisplayToolPath
{
public:
    DisplayToolPath(){}
    void DisplayVectShape(const vector<Handle(AIS_Shape)>&vShape,
                          bool isClear);
    void DisplayLoop(const DefLoop& loop,bool isClear,
                     bool isClassify,bool isOriProf);
    void DisplayBndIslShape(const vector<Handle(AIS_Shape)>&vBndShape,
                            const vector<vector<Handle(AIS_Shape)>>&vIslShape,
                            bool isClear);
    void DisplayBndIslLoop(const GeomArea& bndIslLoop,
                           bool isOriLoop, bool isClear);
    void DisplayVectLoop(const vector<DefLoop>&vLoop,
                         ToolPathType pathType, int nodeIdx,
                         bool isOriProf, bool isClassify, bool isClear);
    void DisplayPoint(const Point& P);
    void DisplayElement(const DefElem& ele);
    void DisplayRootNodeShape(bool isClear,bool isUpdVie);
    void DisplayLoop(const DefLoop &loop);
    void HandleDisplayRootNodeShape(const GeomArea& bndIslLoop,
                                    const shared_ptr<OffsetNode>& rootNode,
                                    bool isClear,
                                    bool isClassify);
    void DisplayArrayLoop(const vector<DefLoop>&vLoop);
    void DisplayPathCutEntryPoint(const shared_ptr<OffsetNode>& rootNode);
};
#endif // DISPLAYTOOLPATH_H
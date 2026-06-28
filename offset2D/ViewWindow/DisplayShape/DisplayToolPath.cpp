#include "DisplayToolPath.h"
#if DebugModel
#pragma optimize("", off)
#endif
shared_ptr<TransToShape> view_tpsha = make_shared<TransToShape>();
void DisplayToolPath::DisplayVectShape(const vector<Handle(AIS_Shape)>&vShape,
                                       bool isClear)
{
    auto myOccView = _mainwind->get_OCCView();
    if(isClear){
        myOccView->myRemoveall();
    }
    for(int i = 0;i < vShape.size();i ++){
        myOccView->getContext()->Display(vShape[i],false);
    }
}
void DisplayToolPath::DisplayLoop(const DefLoop& loop,bool isClear,
                                  bool isClassify,bool isOriProf)
{
}
void DisplayToolPath::DisplayBndIslShape(const vector<Handle(AIS_Shape)>&vBndShape,
                                         const vector<vector<Handle(AIS_Shape)>>&vIslShape,
                                         bool isClear)
{
    auto myOccView = _mainwind->get_OCCView();
    if(isClear){
        myOccView->myRemoveall();
    }
    for (int i = 0; i < vBndShape.size(); ++i) {
        DisplayVectShape(vBndShape,false);
    }
    for (int i = 0; i < vIslShape.size(); ++i) {
        DisplayVectShape(vIslShape[i],false);
    }
    myOccView->getContext()->UpdateCurrentViewer();
}
void DisplayToolPath::DisplayBndIslLoop(const GeomArea& bndIslLoop,
                                        bool isOriLoop,bool isClear)
{
    view_tpsha->GetPairLoopDisplay(bndIslLoop,isOriLoop,isClear);
}
void DisplayToolPath::DisplayVectLoop(const vector<DefLoop>&vLoop,
                                      ToolPathType pathType,
                                      int nodeIdx,
                                      bool isOriProf,
                                      bool isClassify,
                                      bool isClear)
{
    NodeShape nodeShape;
    view_tpsha->GetLoopsDisplayShape(vLoop,pathType,nodeIdx,isOriProf,
                                     isClassify,nodeShape);
    auto myOccView = _mainwind->get_OCCView();
    if(isClear){
        myOccView->myRemoveall();
    }
    for(int lo = 0 ;lo < nodeShape.m_vLoopShape.size();lo ++){
        LoopShape loopShape = nodeShape.m_vLoopShape[lo];
        for(int i = 0;i < loopShape.m_vShape.size();i ++){
            myOccView->getContext()->Display(loopShape.m_vShape[i].m_occShape,
                                             false);
        }
    }
    myOccView->getContext()->UpdateCurrentViewer();
}
void DisplayToolPath::DisplayRootNodeShape(bool isClear,bool isUpdVie)
{
    auto myOccView = _mainwind->get_OCCView();
    if(isClear){
        myOccView->myRemoveall();
    }
    myOccView->myView->FitAll(0.05);
    for(int n = 0;n < glb_vNodeOcc.size();n ++){
        NodeShape curNode = glb_vNodeOcc[n];
        for(int lo = 0 ;lo < curNode.m_vLoopShape.size();lo ++){
            LoopShape loopShape = curNode.m_vLoopShape[lo];
            for(int i = 0;i < loopShape.m_vShape.size();i ++){
                myOccView->getContext()->Display(loopShape.m_vShape[i].m_occShape,
                                                 isUpdVie);
            }
        }
        ///usleep(100);
        ///myOccView->getContext()->UpdateCurrentViewer();
        ///myOccView->myView->FitAll(0.05);
    }
    myOccView->getContext()->UpdateCurrentViewer();
    myOccView->myView->FitAll(0.05);
}
void DisplayToolPath::DisplayLoop(const DefLoop &loop)
{
    TransToShape trans;
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->FitAll(0.05);
    Quantity_Color color(0.,0,1,Quantity_TOC_RGB);
    vector<Handle(AIS_Shape)>vShape;
    trans.TranslateLoopToShape(loop,color,vShape);
    for (int i = 0; i < vShape.size(); ++i) {
        myOccView->getContext()->Display(vShape[i],true);
        myOccView->myView->FitAll(0.05);
    }
}
void DisplayToolPath::DisplayPoint(const Point& P)
{
    TransToShape trans;
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->FitAll(0.05);
    Handle(AIS_Shape)shape;
    trans.TranslatePointToShape(P,Quantity_NOC_BLUE,shape);
    myOccView->getContext()->Display(shape,true);
}
void DisplayToolPath::DisplayElement(const DefElem& ele)
{
    TransToShape trans;
    Handle(AIS_Shape) shape;
    bool isSucccess = false;
    Quantity_Color color(0.8,0,0,Quantity_TOC_RGB);
    trans.TranslateEleToAIS_Shape(ele,color,2,
                                  isSucccess,shape);
    shape->SetColor(color);
    shape->SetWidth(1);
    if(!isSucccess)
        return;
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->FitAll(0.05);
    myOccView->getContext()->Display(shape,true);
}
void DisplayToolPath::DisplayPathCutEntryPoint(const shared_ptr<OffsetNode>& rootNode)
{
    if(rootNode == nullptr){
        return;
    }
    ElemCalculate numcal;
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->FitAll(0.05);
    for (int i = 0; i < rootNode->_loops.size(); ++i) {
        DefLoop loop = rootNode->_loops[i];
        Point startPt = numcal.LoopEndpoint(loop,false);
        //DisplayPoint(startPt);
        DisplayLoop(loop);
        int tem = 1;
    }
    for (int n = 0; n < rootNode->_nodes.size(); ++n) {
        DisplayPathCutEntryPoint(rootNode->_nodes[n]);
    }
}
void DisplayToolPath::DisplayArrayLoop(const vector<DefLoop>&vLoop)
{
    TransToShape trans;
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->FitAll(0.05);
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        vector<Handle(AIS_Shape)>vShape;
        trans.TranslateLoopToShape(vLoop[lo],Quantity_NOC_BLUE,vShape);
        for (int i = 0; i < vShape.size(); ++i) {
            myOccView->getContext()->Display(vShape[i],true);;
        }
    }
}
void DisplayToolPath::HandleDisplayRootNodeShape(const GeomArea& bndIslLoop,
                                                 const shared_ptr<OffsetNode>&rootNode,
                                                 bool isClear,
                                                 bool isClassify)
{
    glb_bndIslShape.m_bndShape.m_vShape.clear();
    glb_bndIslShape.m_vIslShape.clear();
    glb_vNodeOcc.clear();
    auto myOccView = _mainwind->get_OCCView();
    myOccView->myView->SetProj(V3d_Zpos);
    if(isClear){
        myOccView->myRemoveall();
    }
    view_tpsha->GetPairLoopDisplay(bndIslLoop,true,false);
    view_tpsha->GetOffsetNodeLoopShape(rootNode,isClassify,0);
    DisplayRootNodeShape(false,false);
}
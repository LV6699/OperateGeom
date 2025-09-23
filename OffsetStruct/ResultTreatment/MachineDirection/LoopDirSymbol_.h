#include "LoopDirSymbol.h"
void LoopDirSymbol::SetNodeDirectionSymbol(double toolRadius,
                                           const shared_ptr<OffsetNode>& 
rootNode,
                                           shared_ptr<OffsetNode>& symNode)
{
    vector<DefLoop>vLoop;
    NodePathDirectionSymbol(toolRadius,rootNode,vLoop);
    if(!vLoop.empty())
        symNode = make_shared<OffsetNode>(BndOffsetPath,vLoop);
    ///rootNode->_vOffsetNode.push_back(symNode);
}
void LoopDirSymbol::NodePathDirectionSymbol(const double toolRadius,
                                            const shared_ptr<OffsetNode>& 
rootNode,
                                            vector<DefLoop>& vLoop)
{
    if(rootNode == nullptr){
        return;
    }
    for (int lo = 0; lo < rootNode->_vLoopData.size(); ++lo) {
        if(rootNode->_vLoopData[lo].m_vElem.empty()){
            continue;
        }
        DefLoop loop;
        DefSeg seg1,seg2;
        bool isExiSym = false;
        DefElem ele = rootNode->_vLoopData[lo].m_vElem[0];
        double z = ElemCalculate().LoopZCoordinate(rootNode->_vLoopData[lo]);
        ElementDirectionSymbol(ele,toolRadius,isExiSym,seg1,seg2);
        if(!isExiSym){
            continue;
        }
        seg1._segSp.SetZ(z);
        seg1._segEp.SetZ(z);
        seg2._segSp.SetZ(z);
        seg2._segEp.SetZ(z);
        loop.m_vElem.push_back(DefElem(LINETYPE,seg1));
        loop.m_vElem.push_back(DefElem(LINETYPE,seg2));
        vLoop.push_back(loop);
    }
    for (int i = 0; i < rootNode->_vOffsetNode.size(); ++i) {
        NodePathDirectionSymbol(toolRadius,rootNode->_vOffsetNode[i],vLoop);
    }
}
void LoopDirSymbol::ElementDirectionSymbol(const DefElem ele,
                                           const double toolRadius,
                                           bool& isExiSym,
                                           DefSeg& segment1,
                                           DefSeg& segment2)
{
    Point midPt,movePt;
    ElemCalculate numcal;
    GeometryLogic golg;
    GeomCalculate gocal;
    MathCalculate matcal;
    DefElem preEle,nexEle;
    numcal.ElementMidpoint(ele,midPt);
    numcal.PointSplitElement(ele,midPt,true,preEle);    numcal.PointSplitElement(ele,midPt,false,nexEle);
    bool isCloseSp = false;
    double stepVal = toolRadius / 2.5;
    if(ele.ElemeType() == ARCTYPE){
        stepVal = matcal.ArcLenthTranslateAngle(stepVal,preEle.
                                                _arc.ArcRadius());
    }
    ///numcal.ElementEndpointStepVal(ele,stepVal,stepVal);;
    golg.ApproachElementEndpoint(preEle,stepVal,true,isCloseSp,movePt);
    ///movePt和midPt组成新的线段,线段起点是movePt,线段终点是midPt.
    Point P1,P2;
    double angle = 35;
    ///double len = dis / std::cos(angle * M_PI / 180);
    gocal.PointAroundFixedRotate(movePt,midPt,angle,CLOCKWISE,P1);
    gocal.PointAroundFixedRotate(movePt,midPt,angle,ANTICLOCKWISE,P2);
    segment1.SetSegStartPt(midPt);
    segment1.SetSegEndPt(P1);
    segment2.SetSegStartPt(midPt);
    segment2.SetSegEndPt(P2);
    isExiSym = true;
}
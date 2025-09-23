/**
  @author: lvdong
  @date: 2023-07-12
**/
#include "PointInPolygon.h"
#if DebugModel
#pragma optimize("", off)
#endif
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
void NotIntersectCase(const DefLoop &loop,int i,IntInfo& intInfo,
                      int &totalIntNum,bool &isInPoly)
{
    PntInPolySolve insol;
    bool isMaxCoord = false;
    if(intInfo._isComNexSp){
        insol.IsElemEndpointMaxCoord(loop.IndexElem(i),true,isMaxCoord);
    }
    else{
        if(i == loop.ElemNum() - 1 && intInfo._isComLastEp){
            insol.IsElemEndpointMaxCoord(loop.IndexElem(i),false,isMaxCoord);
        }
    }
    if(isMaxCoord){
        totalIntNum ++;
        isInPoly = !isInPoly;
    }
    intInfo._hasComPreEp = false;
    intInfo._isComNexSp = false;
    //laterStatus = isInPoly;
    //hasPreChStatus = preStatus == laterStatus ? false : true;
}
void TangencyCase(const DefLoop &loop,const Point &P1,
                  const Point &P2,vector<int>vIndex,
                  bool &isInPoly)
{
    if(!isInPoly)
        return;
    GeomTangency gotan;PointOnElement pton;
    for (int i = 0; i < vIndex.size(); ++i) {
        if(!loop.IsValIndex(vIndex[i])){
            continue;
        }
        if(loop.IndexElem(vIndex[i]).ElemeType() != ARCTYPE){
            return;
        }
        bool isTan = false;
        DefArc arc = loop.IndexElem(vIndex[i]).ElemArc();
        arc.ChoiceSetRadius(PreErr5_3);
        gotan.IsLineCircleTangency(P1,P2,arc.ArcCenterPt(),
                                   arc.ArcRadius(),PreErr5_10,isTan);
        if(!isTan)
            return;
        bool isOnSeg1 = false,isOnSeg2 = false;
        pton.IsPointOnSegment(P1,P2,arc.ArcStartPt(),
                              InPol_IsArcExtrPt_Err,
                              PreErr_10,true,isOnSeg1);        if(isOnSeg1)
            return;
        pton.IsPointOnSegment(P1,P2,arc.ArcEndPt(),
                              InPol_IsArcExtrPt_Err,
                              PreErr_10,true,isOnSeg2);
        if(isOnSeg2)
            return;
    }
    isInPoly = false;
}
void HorizontalLineCase(const Point& lsp,
                        const Point& lep,
                        double y,
                        IntInfo& intInfo,
                        int& totalIntNum,
                        bool& isInPoly)
{
    if(intInfo._isComNexSp){
        return;
    }else if(std::abs((lsp.Y() + lep.Y())/2 - y) > PreErr5_8){
        intInfo._hasComPreEp = true;
        intInfo._isComNexSp = true;
    }
    totalIntNum ++;
    isInPoly = !isInPoly;
    intInfo._hasComPreEp = true;
    intInfo._isComNexSp = true;
}
void PointInPolygon::IsPointInsideLoop(const DefLoop &loop,
                                       const Point &P,
                                       double bordPre,
                                       bool &isOnBorder,
                                       bool &isInPoly)
{
    isOnBorder = false;
    isInPoly = false;
    LimitCoordinate limco;
#if 0
    if(P.X() == 7.105427357601002e-15 && P.Y() == 24.1){
        DisplayGeom().DisplayLoop(loop,_colors.back(),1,true,true);
        DisplayGeom().DisplayPoint(P,_colors[1]);
        _mainwind->myOccView->fitAll();
        string str = OperateOftString().LoopElementString(loop);
        std::cout<<str<<endl;
    }
#endif
    if(!limco.IsPointInLoopCoord(loop,P,bordPre * PreErr_5_))
        return;
    GeomCalculate go;GeomIntersect goInt;PntInPolySolve insol;
    TupleQuality tuple;GeoContainSolve consol;
    consol.IsPointOnLoopBorder(loop,P,bordPre,isOnBorder);
    if(isOnBorder)
        return;
    Point raypt(20000,P.Y());
    vector<int>vIndex;
    int totalIntNum = 0;
    IntInfo intInfo;
    for (int i = 0; i < loop.Size(); ++i){/**
        if(i == 30){
            int tem = 0;        }*/
        const auto& ele = loop[i];
        GeomIntRes intRes;
        switch (ele._elemType) {
        case LINETYPE:{
            const auto& sp = ele._segment._segSp;
            const auto& ep = ele._segment._segEp;
            if(tuple.IsHorizontalLine(sp,ep,InPol_HorOrdi_Err))
                continue;
            goInt.CalculateSegSegIntersect(P,raypt,sp,ep,true,
                                           PreErr5_8,PreErr_10,intRes);
            if(!intRes._isExistInt){
                NotIntersectCase(loop,i,intInfo,totalIntNum,isInPoly);
                continue;
            }
            vIndex.push_back(i);
            insol.IntersectSegmentCase(loop,i,sp,ep,intRes._intPnt1,
                                       vIndex,intInfo,totalIntNum,isInPoly);
            break;
        }
        case ARCTYPE:{
            const auto& arc = ele._arc;
            goInt.CalculateSegmentArcInt(P,raypt,arc,PreErr5_12,
                                         PreErr5_8,PreErr_10,intRes);
            if(!intRes._isExistInt){
                NotIntersectCase(loop,i,intInfo,
                                 totalIntNum,isInPoly);
                continue;
            }
            vIndex.push_back(i);
            insol.IntersectArcCase(loop,P,raypt,i,arc,intRes,
                                   vIndex,intInfo,totalIntNum,
                                   isInPoly);
            break;
        }
        case CIRCLETYPE:{
            double distance = P.Distance2D(ele.ElemCircle().CircleCenter());
            if(distance < ele.ElemCircle().CircleRadius() + bordPre){
                isInPoly = true;
                return;
            }
            break;
        }
        default:{
            break;
        }
        }
    }
    ///TangencyCase(loop,P,raypt,vIndex,isInPoly);
}
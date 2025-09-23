/**
  @author: lvdong
  @date: 2023-07-12
**/
#include "PntInPolySolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void PntInPolySolve::IsArcEndpointMaxCoord(const DefArc& arc,
                                           bool isSp,bool& isMaxCoord)
{
    GeomCalculate gocal;GeometryLogic golg;
    Point P = isSp == true ? arc.ArcStartPt() : arc.ArcEndPt();;
    double aglAbs,stepAlg = InPol_ArcExtrStep_Err;
    gocal.ArcCenterAngle(arc,aglAbs);
    if(aglAbs < (2 * stepAlg)){
        stepAlg = aglAbs / 2;
    }
    Point endpoint;
    golg.CloseArcEndpointBaseAngle(arc,!isSp,stepAlg,endpoint);
    isMaxCoord = P.Y() > endpoint.Y() ? true : false;
}
void PntInPolySolve::IsElemEndpointMaxCoord(const DefElem& ele,bool isSp,
                                            bool& isMaxCoord)
{
    /**
    需要考虑下列情况:
    比较时可以等于,为了防止当下列情况,比如线段与i-1、i、i+1,其中i是水平线段,
    i-1、i+1是圆弧,与i-1相切,与i+1相交端点且该端点具有最大 y值,理论上点不在
    环内,然而与i+1是有效相交,因此会出现误判.*/
    isMaxCoord = false;
    if(ele._elemType == LINETYPE){
        if(isSp){
            isMaxCoord = ele.ElemSeg().SegStartPt().Y() >
                    ele.ElemSeg().SegEndPt().Y() ? true : false;
        }
        else{
            isMaxCoord = ele.ElemSeg().SegEndPt().Y() >
                    ele.ElemSeg().SegStartPt().Y() ? true : false;
        }
        return;
    }
    IsArcEndpointMaxCoord(ele._arc,isSp,isMaxCoord);
}
bool PntInPolySolve::IsElemNotHorLine(const DefElem& ele)
{
    TupleQuality tupqua;
    if(ele._elemType != LINETYPE)
        return true;
    return !tupqua.IsHorizontalLine(ele._segment._segSp,
                                    ele._segment._segEp,
                                    PreErr5_5);
}
void PntInPolySolve::PreElemMaxOrdinateCase(const DefLoop &loop,
                                            int i, vector<int> &indexs,
                                            int &totalIntNum,
                                            bool &isInPoly)
{    int pre = ElemCalculate().PreviousIndex(loop.m_vElem.size(),i);
    if(IsElemNotHorLine(loop.IndexElem(pre))){
        bool isLarge = false;
        IsElemEndpointMaxCoord(loop.m_vElem[pre],false,isLarge);
        ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
    }
}
///只需要围绕交点是endpoint、sp、ep进行处理
void PntInPolySolve::IntersectSegmentCase(const DefLoop& loop, int i,
                                          const Point& lsp,
                                          const Point& lep,
                                          const Point& intPnt,
                                          vector<int>& indexs,
                                          IntInfo& intInfo,
                                          int& totalIntNum,
                                          bool& isInPoly)
{
    if((i == loop.m_vElem.size() - 1) && intInfo._isComLastEp){
        bool isLarge = lep.Y() > lsp.Y() ? true : false;
        ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
        return;
    }
    if(i == 0 && intPnt.IsSamePoint2D(lsp,PreErr_6)){
        intInfo._isComNexSp = true;
    }
    ///如果i!=0,当且仅当intInfo._isComNexSp==true 时,
    ///isEleSp = true, 即isEleSp取值由intInfo._isComNexSp 决定.
    bool isEndpoint = false,isEleSp = false;
    if(intInfo._isComNexSp){
        isEleSp = true;
        isEndpoint = true;
    }else{
        isEndpoint = intPnt.IsSamePoint2D(lep,PreErr_6);
    }
    if(isEndpoint){
        if(isEleSp){
            /// 这时需要比较前一元素的端点是否拥有最大纵坐标 ,这时要考虑前一元素是否
            /// 已经判断过了,如果已经判断过了,则不能再次判断.
            bool isLarge = lsp.Y() > lep.Y() ? true : false;
            ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
            if(i == 0){
                intInfo._hasComFirSp = true;
                intInfo._isComLastEp = true;
            }
            intInfo._hasComPreEp = false;
            intInfo._isComNexSp = false;
            return;
        }
        else{
            /// 线段结束点,isComNexSp不可能是true.
            if(i == loop.m_vElem.size() - 1 && !intInfo._hasComFirSp){
                bool isLarg_ = false;
                IsElemEndpointMaxCoord(loop.IndexElem(0),true,isLarg_);
                ExicuteMaxOrdinate(isLarg_,i,indexs,isInPoly,totalIntNum);
            }
            bool isLarge = lep.Y() > lsp.Y() ? true : false;
            ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
            intInfo._hasComPreEp = true;
            intInfo._isComNexSp = true;
        }
        return;
    }    ExicuteMaxOrdinate(true,i,indexs,isInPoly,totalIntNum);
    intInfo._hasComPreEp = false;
    intInfo._isComNexSp = false;
}
void PntInPolySolve::IntersectArcCase(const DefLoop& loop,
                                      const Point & lsp,
                                      const Point& lep, int i,
                                      const DefArc& arc,
                                      const GeomIntRes& intRes,
                                      vector<int>& indexs,
                                      IntInfo& intInfo,
                                      int& totalIntNum,
                                      bool& isInPoly)
{
    if(intRes._intNum == 2){
        TwoIntersectPointCase(loop,i,indexs,arc,
                              intRes._intPnt1,intRes._intPnt2,
                              intInfo,totalIntNum,isInPoly);
        return;
    }
    if((i == loop.m_vElem.size() - 1) && intInfo._isComLastEp){
        bool isLarge = false;
        IsArcEndpointMaxCoord(arc,false,isLarge);
        ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
        return;
    }
    if(i == 0 && intRes._intPnt1.IsSamePoint2D(arc._arcSp,PreErr_6)){
        intInfo._isComNexSp = true;
    }
    ///如果i!=0,当且仅当intInfo._isComNexSp==true 时,
    ///isEleSp = true, 即isEleSp取值由intInfo._isComNexSp 决定.
    const Point& intpt = intRes._intPnt1;
    bool isEndpoint = false,isEleSp = false;
    if(intInfo._isComNexSp){
        isEleSp = true;
        isEndpoint = true;
    }else{
        isEndpoint = intpt.IsSamePoint2D(arc._arcEp,PreErr_6);
    }
    if(isEndpoint){
        bool isLarge = false;
        if(isEleSp){
            IsArcEndpointMaxCoord(arc,true,isLarge);
            ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
            if(i == 0){
                intInfo._hasComFirSp = true;
                intInfo._isComLastEp = true;
            }
            intInfo._hasComPreEp = false;
            intInfo._isComNexSp = false;
            return;
        }else{
            /// 线段结束点,isComNexSp不可能是true.
            if(i == loop.m_vElem.size() - 1 && !intInfo._hasComFirSp){
                bool isLarg_ = false;
                IsElemEndpointMaxCoord(loop.IndexElem(0),true,isLarg_);
                ExicuteMaxOrdinate(isLarg_,i,indexs,isInPoly,totalIntNum);
            }
            IsArcEndpointMaxCoord(arc,false,isLarge);
            ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
            intInfo._hasComPreEp = true;
            intInfo._isComNexSp = true;        }
        return;
    }
    double dist;
    GeomCalculate().PointToLineDistance(lsp,lep,arc._arcCp,dist);
    if(std::abs(dist - arc.ArcRadius()) < PreErr5_12){
        const DefElem &ele = loop.PreviousElem(i);
        if(ele.ElemeType() == LINETYPE &&
                intpt.IsSamePoint2D(arc._arcSp,PreErr_6)){
            const Point& sp = ele._segment._segSp;
            const Point& ep = ele._segment._segEp;
            if(TupleQuality().IsHorizontalLine(sp,ep,PreErr_6)){
                bool isLarge = false;
                IsArcEndpointMaxCoord(arc,true,isLarge);
                ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
            }
        }
        return;
    }
    ExicuteMaxOrdinate(true,i,indexs,isInPoly,totalIntNum);
    intInfo._hasComPreEp = false;
    intInfo._isComNexSp = false;
}
void PntInPolySolve::TwoIntersectPointCase(const DefLoop &loop, int i,
                                           vector<int> &indexs,
                                           const DefArc& arc,
                                           const Point& p1,
                                           const Point& p2,
                                           IntInfo &intInfo,
                                           int &totalIntNum,
                                           bool &isInPoly)
{
    bool isLastEp = false;
    if((i == loop.m_vElem.size() - 1) && intInfo._isComLastEp){
        isLastEp = true;
    }
    if(i == 0 && p1.IsSamePoint2D(arc._arcSp,PreErr_6) ||
            p2.IsSamePoint2D(arc._arcSp,PreErr_6)){
        intInfo._isComNexSp = true;
    }
    if(intInfo._isComNexSp){
        ///当最后元素既要比较开始点又要比较结束点时直接跳过 .
        if(isLastEp){
            return;
        }
        bool isLarge = false;
        IsArcEndpointMaxCoord(arc,true,isLarge);
        ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
        if(i == 0){
            intInfo._hasComFirSp = true;
            intInfo._isComLastEp = true;
        }
        double dis1,dis2;
        GeomDistance godis;
        godis.PointToArcEndpointDistance(arc,p1,true,dis1);
        godis.PointToArcEndpointDistance(arc,p2,true,dis2);
        ///dis1 和dis2的数值可能是360
        dis1 = dis1 == 360 ? 0 : dis1;
        dis2 = dis2 == 360 ? 0 : dis2;
        Point noSp = dis1 > dis2 ? p1 : p2;
        bool isEp = false;        ///确定远离圆弧开始点的相交点 noArcSp是否跟圆弧结束点同一点
        isEp = noSp.IsSamePoint2D(arc._arcEp,PreErr_6);
        if(isEp){
            if((i == loop.m_vElem.size() - 1) && !intInfo._hasComFirSp){
                bool isLarg_ = false;
                IsElemEndpointMaxCoord(loop.IndexElem(0),true,isLarg_);
                ExicuteMaxOrdinate(isLarg_,i,indexs,isInPoly,totalIntNum);
                intInfo._hasComFirSp = true;
                intInfo._isComLastEp = false;
            }
            bool isLarge_ = false;
            IsArcEndpointMaxCoord(arc,false,isLarge_);
            ExicuteMaxOrdinate(isLarge_,i,indexs,isInPoly,totalIntNum);
            intInfo._isComNexSp = true;
        }
        else{
            /// 当isEp == false时,noSp不可能是开始点,即只能是非端点.
            ExicuteMaxOrdinate(true,i,indexs,isInPoly,totalIntNum);
            intInfo._isComNexSp = false;
        }
        return;
    }
    if(isLastEp){
        bool isLarge = false;
        IsArcEndpointMaxCoord(arc,false,isLarge);
        ExicuteMaxOrdinate(isLarge,i,indexs,isInPoly,totalIntNum);
        ///当intInfo._isComNexSp==false 时,noEp不可能是开始点,即只能是非端点.
        ExicuteMaxOrdinate(true,i,indexs,isInPoly,totalIntNum);
        return;
    }
    Point arcEp = arc._arcEp;
    bool isEp1 = false,isEp2 = false;
    isEp1 = p1.IsSamePoint2D(arcEp,PreErr_6);
    isEp2 = p2.IsSamePoint2D(arcEp,PreErr_6);
    if(!isEp1 && !isEp2){
        intInfo._hasComPreEp = false;
        intInfo._isComNexSp = false;
        return;
    }
    ///当两个交点之一是圆弧端点时 ,两相交点计算方式相同 ,且互不影响.
    SinglePointCase(loop,i,indexs,arc,intInfo,
                    isEp1,totalIntNum,isInPoly);
    SinglePointCase(loop,i,indexs,arc,intInfo,
                    isEp2,totalIntNum,isInPoly);
}
void PntInPolySolve::SinglePointCase(const DefLoop &loop,int i,
                                     vector<int> &vIndex,
                                     const DefArc &arc,
                                     IntInfo &intInfo,bool isEp,
                                     int &totalIntNum,
                                     bool &isInPoly)
{
    ////当两个交点之一是圆弧端点时，两相交点计算方式相同，且互不影响
    if(isEp){
        if((i == loop.m_vElem.size() - 1) && !intInfo._hasComFirSp){
            bool isLarg_ = false;
            IsElemEndpointMaxCoord(loop.IndexElem(0),true,isLarg_);
            ExicuteMaxOrdinate(isLarg_,i,vIndex,isInPoly,totalIntNum);
            intInfo._hasComFirSp = true;
            intInfo._isComLastEp = false;
        }        bool isLarge = false;
        IsArcEndpointMaxCoord(arc,false,isLarge);
        ExicuteMaxOrdinate(isLarge,i,vIndex,isInPoly,totalIntNum);
        intInfo._hasComPreEp = true;
        intInfo._isComNexSp = true;
    }
    else{
        ExicuteMaxOrdinate(true,i,vIndex,isInPoly,totalIntNum);
    }
}
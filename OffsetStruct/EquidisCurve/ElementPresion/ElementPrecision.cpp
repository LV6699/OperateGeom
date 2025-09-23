#include "ElementPrecision.h"
#if DebugModel
#pragma optimize("", off)
#endif
/// 处理一般流程:
/// 需要先初始化圆弧半径
/// 处理环中存在完全或部分重合的线段 .
/// 处理相邻元素连接点、线段水平或垂直特性、重置圆弧结束点等 .
void ElementPrecision::SpecifyInputPretreatment(bool isFixedOft,
                                                bool isAblePreProc,
                                                double oftVaue,
                                                bool isIniRadi,
                                                bool isIniSur,
                                                bool isDelInval,
                                                bool isEndtoend,
                                                bool isHandleClose,
                                                DefLoop& loop,
                                                vector<DefLoop>&vLoop)
{
#if 1
    if(isFixedOft && (oftVaue <= PreprocValLim) && !isAblePreProc)
        return;
#endif
    InputPretreatment(loop,vLoop,isIniRadi,
                      isIniSur,isDelInval,
                      isEndtoend,isHandleClose);
}
void ElementPrecision::SpecifyPretreatmentLoop(bool isFixedOft,
                                               bool isAblePreProc,
                                               double oftVaue,
                                               bool isIniRad,
                                               bool isIniSur,
                                               bool isDelInval,
                                               bool isEndtoend,
                                               bool isHandleClose,
                                               DefLoop& loop)
{
#if 1
    if(isFixedOft && (oftVaue <= PreprocValLim) && !isAblePreProc)
        return;
#endif
    PretreatmentLoop(loop,isIniRad,isIniSur,isDelInval,
                     isEndtoend,isHandleClose);
}
void ElementPrecision::InputPretreatment(DefLoop& loop,
                                         vector<DefLoop>&vLoop,
                                         bool isIniRadi,
                                         bool isIniSur,
                                         bool isDelInval,
                                         bool isEndtoend,
                                         bool isHandleClose)
{
    PretreatmentLoop(loop,isIniRadi,isIniSur,
                     isDelInval,isEndtoend,
                     isHandleClose);
    for(auto& it : vLoop)
        PretreatmentLoop(it,isIniRadi,isIniSur,isDelInval,
                         isEndtoend,isHandleClose);
}void ElementPrecision::PretreatmentLoop(DefLoop& loop,
                                        bool isIniRad,
                                        bool isIniSur,
                                        bool isDelInval,
                                        bool isEndtoend,
                                        bool isHandleClose)
{
    if(loop.IsEmpty())
        return;
    loop.ChoiceSetRadius(PreErr5_3);
    if(isIniRad){
        ///loop.InitialRadius();
    }
    if(isDelInval){
        DeleteInvalidElement(PreErr5_5,PreErr5_5,loop);
    }
    if(isIniSur){
        HandleLoopSurplusElem(loop);
    }
    if(isEndtoend){
        if(loop._loopLabel._isPreReverse){
            ElemCalculate().ReverseLoopDirection(loop);
            ///LoopElementEndToEnd(loop);
            OperateLoop().LoopCurveEndToEnd(loop);
            ElemCalculate().ReverseLoopDirection(loop);
        }else{
            OperateLoop().LoopCurveEndToEnd(loop);
            ///LoopElementEndToEnd(loop);
        }
    }
    if(isHandleClose && !loop.IsOpenLoop()){
        LoopEndToEnd().SetLoopLoopEndToEnd(loop);
    }
}
void ElementPrecision::LoopElementEndToEnd(DefLoop &loop)
{
    if(GeometryLogic().IsCircleStructureLoop(loop))
        return;
    ElementPreSolve presol;
    ///处理线段水平和垂直特性，前后元素连接性 .
    for (int i = 0; i < loop.ElemNum(); ++i) {/**
        if(i == 55){
            int tem = 1;
        }*/
        if(loop.IsOpenLoop() && i == 0)
            continue;
        Point ep;
        bool isLast = (i == loop.Size() - 1);
        presol.PreviousElemEndpoint(loop,i,ep);
        ///首先改变端点,然后处理线段水平垂直特性和圆弧两端点 .
        if(loop.IndexElem(i)._elemType == LINETYPE){
            loop.m_vElem[i]._segment._segSp = ep;
            presol.SetLineHorVerQuality(loop.m_vElem[i]._segment._segSp,
                                        loop.m_vElem[i]._segment._segEp,
                                        true,Base_LineHor_Err);
        }
        else if(loop.IndexElem(i)._elemType == ARCTYPE){
            loop.m_vElem[i]._arc._arcSp = ep;
            presol.ResetArcEndpoint(isLast,loop.m_vElem[i]._arc);
        }
    }
}void ElementPrecision::HandleLoopSurplusElem(DefLoop& loop)
{
    if(loop.Size() < 2)
        return;
    int isArcLoop = true;
    for(auto& d : loop.m_vElem){
        if(d._elemType == LINETYPE){
            isArcLoop = false;
            break;
        }
    }
    OperateLoop opel;
    if(isArcLoop){
        opel.CircleStructureArcLoop(loop);
    }
    opel.AdjacentParallSegment(loop);
    opel.AdjacentParallArc(loop);
}
void ElementPrecision::DeleteInvalidElement(double segPre,double arcPre,
                                            vector<DefElem>& vElem)
{
    vector<int>ids;
    ElemCalculate numcal;
    for (int i = 0; i < vElem.size(); ++i) {
        bool isVal = false;
        if(vElem[i].ElemeType() == ARCTYPE){
            vElem[i]._arc.ChoiceSetRadius(PreErr5_3);
        }
        numcal.ElementValidity(vElem[i],segPre,arcPre,isVal);/**
        numcal.IsValidGeomeElement(vElem[i],segPre,arcPre,
                                   PreErr5_6,PreErr5_6,isVal);*/
        if(!isVal){
            ids.emplace_back(i);
        }
    }
    numcal.DeleteIndexElement(ids,vElem);
}
void ElementPrecision::DeleteInvalidElement(double segPre,double arcPre,
                                            DefLoop& loop)
{
    DeleteInvalidElement(segPre,arcPre,loop.m_vElem);
}
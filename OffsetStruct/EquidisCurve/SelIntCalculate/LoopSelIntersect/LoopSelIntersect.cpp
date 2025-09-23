#include "LoopSelIntersect.h"
#include "../../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#endif
void LoopSelIntersect::LoopClipPreprocesse(DefLoop& loop)
{
    GeometryLogic().CircleArcLoopTransform(true,loop);
    for (int i = 0; i < loop.ElemNum(); ++i){
        loop.m_vElem[i]._elemLabel._eleHasMer = false;
        loop.m_vElem[i]._elemLabel._isValidity = true;
    }
    loop._loopLabel._loopIntersect = false;
}
void LoopSelIntersect::ArrayLoopClipPreprocesse(vector<DefLoop>& vLoop)
{
    for(auto& lo : vLoop)
        LoopClipPreprocesse(lo);
}
void LoopSelIntersect::CalculateLoopSelIntersect(const DefLoop &oriLoop,
                                                 const DefLoop &loop,
                                                 OffsetParam param,
                                                 double offsetVal,
                                                 bool isDiscernInc,
                                                 bool &isExiSelInt,
                                                 vector<DefLoop> &rloops)
{
    if(param._offsetUnit.IsPureEqud())
        return;
#if 0
    bool isCnt = loop.IsConnect(PreErr5_12);
    auto area = GeomArea(oriLoop,vector<DefLoop>{loop});
    DisplayGeom().DisplayGeomArea(area);
#endif
#if 0
    auto area = GeomArea(oriLoop,vector<DefLoop>{loop});
    string s = OperateOftString().BndIslLoopString(area);
    std::cout<<s<<endl;
    ///return;
#endif
    rloops.clear();
    isExiSelInt = false;
    bool isOpen = oriLoop.IsOpenLoop();
    if(GeometryLogic().IsCircleStructureLoop(loop)  ||
            loop.ElemNum() < 2)
        return;
    DefLoop clipLoop;
    SelIntersectLogic sellg;
    bool isIntersect = false;
    sellg.ClipSelfIntersectLoop(oriLoop,loop,offsetVal,
                                isIntersect,clipLoop);
    ///bool isCnt1 = loop.IsConnect(PreErr5_12);
    ///DisplayGeom().DisplayLoop(clipLoop,_colors[1],1);
#if 0
    isExiSelInt = true;
    string s1 = OperateOftString().LoopElementString(clipLoop,false,1);
    std::cout<<s1<<endl;
    ///return;
#endif    bool isExiDelete = false;
    sellg.DeleteInvalidClipElement(oriLoop,offsetVal,loop.IsOpenLoop(),
                                   isExiDelete,clipLoop.m_vElem);
    if(!isExiDelete)
        return;
    ///bool isCnt2 = clipLoop.IsConnect(PreErr5_12);
    ///DisplayGeom().DisplayLoop(clipLoop,_colors[1],1);
#if 0
    vector<int>vIndex;
    isExiSelInt = true;
    rloops.push_back(clipLoop);
    EqudDebug().SameEndpointIndex(clipLoop.m_vElem,
                                  PreErr5_10,vIndex);
    string s2 = OperateOftString().LoopElementString(clipLoop,false,2);
    std::cout<<s2<<endl;
    return;
#endif
    LoopClipPreprocesse(clipLoop);
    vector<DefLoop>rloops_;
    sellg.OrgnizeElementToLoop(clipLoop,isOpen,rloops_);
    for (int i = 0; i < rloops_.size(); ++i) {
        DefLoop loop_ = loop;
        loop_.SetLoopElem(rloops_[i].LoopElem());
        rloops_[i] = loop_;
    }
    SelIntersectAux selsol;
    for(auto& it : rloops_){
        selsol.SetCurLoopClose(loop.IsOpenLoop(),it);
        it.SetLoopType(oriLoop.LoopType());
    }
#if 0
    rloops = rloops_;
    isExiSelInt = true;
    string s3 = OperateOftString().ArrayLoopString(rloops);
    std::cout<<s3<<endl;
    return;
#endif
    SelfIntersectAftertreatment(oriLoop,rloops_,
                                isDiscernInc,rloops);
    ArrayLoopClipPreprocesse(rloops);
    OperateLoop().SetArrayLoopBaseParam(param._offsetUnit.
                                        ToolRadius(),offsetVal,rloops);
    isExiSelInt = true;
}
void LoopSelIntersect::SelfIntersectAftertreatment(const DefLoop &oriLoop,
                                                   const vector<DefLoop>&loops,
                                                   bool isDiscernInc,
                                                   vector<DefLoop>&rloops)
{
    GeomContain gocon;ElemCalculate numcal;
    LoopDirection lodir;CalculateValidity calval;
    for (int i = 0; i < loops.size(); ++i) {
        if(!calval.IsMeetMinBoxMeasure(loops[i]))
            continue;
        if(loops[i].IsOpenLoop()){
            rloops.push_back(loops[i]);
            continue;
        }
        DefLoop curLoop;
        vector<DefLoop> vAnoLoop;
        numcal.ExceptIndexLoop(loops,i,curLoop,vAnoLoop);
        if(!oriLoop.IsOpenLoop() &&                !numcal.IsCloseLoop(curLoop,PreErr5_5))
            continue;
        ///seIn_num->PrintLoopData(curLoop);
        numcal.ResetLoopEleParameters(curLoop,false,curLoop);
        curLoop.SetLoopType(oriLoop.LoopType());
        bool isLegalLoop = false;
        numcal.IsLegalCurveNumLoop(curLoop,isLegalLoop);
        if(!isLegalLoop)
            continue;
        RotDir rotDir = lodir.LoopRotateDirection(curLoop);
        if(rotDir != CLOCKWISE){
            bool matchResult = false;
            for(auto& it : vAnoLoop){
                gocon.IsLoopContainLoop(it,curLoop,PreErr5_6,PreErr5_6,
                                        PreErr5_10,matchResult);
                if(matchResult)
                    break;
            }
            if(matchResult){
                numcal.ReverseLoopDirection(curLoop);
                curLoop.SetLoopType(oriLoop.LoopType() == OUTSIDEPROFILE ?
                                        WITHINPROFILE : OUTSIDEPROFILE);
            }
            else
                continue;
        }
        bool isInclude = false;
        gocon.IsLoopsContainLoop(vAnoLoop,curLoop,PreErr5_6,
                                 PreErr5_10,isInclude);
        if(isInclude && rotDir == CLOCKWISE)
            continue;
        isInclude = false;
        if(rotDir == CLOCKWISE && isDiscernInc){
            gocon.IsLoopContainLoopCoord(oriLoop,curLoop,
                                         PreErr5_10,isInclude);
            if((oriLoop.LoopType() == OUTSIDEPROFILE && !isInclude) ||
                    (oriLoop.LoopType() == WITHINPROFILE && isInclude))
                continue;
        }
        rloops.push_back(curLoop);
    }
}
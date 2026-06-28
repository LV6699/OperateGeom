#include "ReconstructLoop.h"
#include "OperateLoop.h"
ReconstructLoop::ReconstructLoop(){}
bool ReconstructLoop::IsCircleStructLoop(const ofts::DefLoop& loop,
                                         numst::ReLoopErr param)
{
    if(loop.IsCircleLoop()){return true;}
    if(loop.Size() != 2){return false;}
    if(loop[0].Type() != Ofte::ARCTYPE ||
            loop[1].Type() != Ofte::ARCTYPE){
        return false;
    }
    if(!loop.IsConnect(param.RelCloseErr())){return false;}
    const auto& cp0 = loop[0].GetArc().GetCp();
    const auto& cp1 = loop[1].GetArc().GetCp();
    auto r0 = loop[0].GetArc().GetOptR(PreErr5_2);
    auto r1 = loop[1].GetArc().GetOptR(PreErr5_2);
    return cp0.IsEqual(cp1,param.RelCpErr()) &&
            std::abs(r0 - r1) <= param.RelRadErr();
}
bool ReconstructLoop::IsCoincideLoop(const DefLoop& loop1,const DefLoop& loop2)
{
    if(loop1.Size() != loop2.Size() || loop1.IsEmpty()){return false;}
    size_t s = 0;bool is = false;
    for(size_t i = 0;i < loop2.Size();++i){
        if(IsSameCurve(loop1[0],loop2[i])){
            is = true;s = i;break;
        }
    }
    if(!is){return false;}
    DefLoop l2;std::vector<DefElem>cs;
    for (size_t i = s; i < loop2.Size(); ++i){
        cs.push_back(loop2[i]);
    }
    for (size_t i = 0; i < s; ++i){
        cs.push_back(loop2[i]);
    }
    for (size_t i = 0; i < loop1.Size(); ++i) {
        if(!IsSameCurve(loop1[i],cs[i])){return false;}
    }
    return true;
}
void ReconstructLoop::OperateReconstructLoop(const NumStruct::ReLoopErr &err,
                                             OffsetStruct::DefLoop& loop)
{
    if(loop.Size() < 2){return;}
    int isArcLoop = true;
    for(auto& d : loop.m_vElem){
        if(d._type == LINETYPE){
            isArcLoop = false;break;
        }
    }
    OperateLoop opel;
    if(isArcLoop){opel.ArcLoopToCircle(loop,err);}
    opel.AdjacentParallSegment(loop,err.RelAglErr());
    opel.AdjacentParallArc(loop,err.RelCpErr(),err.RelCloseErr());
}

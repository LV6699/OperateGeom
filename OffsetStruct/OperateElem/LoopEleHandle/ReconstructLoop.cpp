#include "ReconstructLoop.h"
#include "OperateLoop.h"
ReconstructLoop::ReconstructLoop(){}
bool ReconstructLoop::IsCoincideLoop(const DefLoop& loop1,const DefLoop& loop2)
{
    if(loop1.Size() != loop2.Size())
        return false;
    for (size_t i = 0; i < loop1.Size(); ++i) {
        if(!IsSameCurve(loop1[i],loop2[i]))
            return false;
    }
    return true;
}
void ReconstructLoop::OperateReconstructLoop(const NumStruct::ReLoopErr &err,
                                             OffsetStruct::DefLoop& loop)
{
    if(loop.Size() < 2)
        return;
    int isArcLoop = true;
    for(auto& d : loop.m_vElem){
        if(d._elemType == LINETYPE){
            isArcLoop = false;break;
        }
    }
    OperateLoop opel;
    if(isArcLoop){
        opel.ArcLoopCircleStructure(loop,err);
    }
    opel.AdjacentParallSegment(loop,err.RelAglErr());
    opel.AdjacentParallArc(loop,err.RelCpErr(),err.RelCloseErr());
}
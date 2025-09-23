#ifndef OPERATELOOP_H
#define OPERATELOOP_H
#include"../../CommonFile/Header.h"
#include"../../CommonFile/NumStruct.h"
class OperateLoop
{
public:
    OperateLoop();
    void SetLoopSourceData(const DefLoop& sourceLoop,
                           OffsetParam offsetPara,
                           DefLoop& loop);
    void SetElementParam(const DefElem &oriEle,DefElem &ele);
    void SetLoopElemBaseParam(double r,double offsetVal,DefLoop& loop);
    void SetArrayLoopBaseParam(double r,double offsetVal,
                               vector<DefLoop>& vLoop);
    void LoopCurveEndToEnd(DefLoop& loop);
    void AdjacentParallSegment(DefLoop& loop,double aglErr = Pre_ParallSeg_Err);
    void AdjacentParallArc(DefLoop& loop,
                           double cpErr = Pre_SameCirPt_Err,
                           double rErr = Pre_SameCirR_Err);
    void CircleStructureArcLoop(DefLoop& loop,
                                double cpDisErr = Pre_SameCirPt_Err,
                                double closeErr = PreErr_1);
    void ArcLoopCircleStructure(OffsetStruct::DefLoop& loop,
                                const NumStruct::ReLoopErr& err);
};
#endif // OPERATELOOP_H
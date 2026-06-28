#ifndef OPERATELOOP_H
#define OPERATELOOP_H
#include"../../CommonFile/Header.h"
#include"../../CommonFile/NumStruct.h"
class OperateLoop
{
public:
    OperateLoop(){}
    void SetLoopEndToEnd(ofts::DefLoop& loop,double err = PreErr_6);
    ofts::Point LoopLenthPoint(const ofts::DefLoop& loop,size_t i,bool isOrder,double lenth);
    void LoopCurveEndToEnd(DefLoop& loop);
    void AdjacentParallSegment(DefLoop& loop,double aglErr = Pre_ParallSeg_Err);
    void AdjacentParallArc(DefLoop& loop,
                           double cpErr = Pre_SameCirPt_Err,
                           double rErr = Pre_SameCirR_Err);
    void CircleStructureArcLoop(DefLoop& loop,
                                double cpDisErr = Pre_SameCirPt_Err,
                                double closeErr = PreErr_1);
    void ArcLoopToCircle(ofts::DefLoop& loop,
                         const numst::ReLoopErr& err);
    void CircleArcLoopTrans(bool isToArc,ofts::DefLoop& loop);
    bool IsCircleStructLoop(const ofts::DefLoop& loop,bool isMatchArc,
                            double endE,double rErr);
    void OptCircleArcTrans(ofts::DefLoop& loop,
                           bool isToArc,bool isMatchArc,
                           double cpErr,double rErr);
    void LoopToCloseLoops(const ofts::DefLoop& inloop, double err,
                          std::vector<ofts::DefLoop>& loops);
    void ClipLoopSelIntCurve(const numst::IntParam& param,
                             ofts::DefLoop& loop);
    void DelLoopCoinCurve(ofts::DefLoop& loop,double err);

    void MakeLoopEndToEnd(DefLoop& loop,double err = PreErr_12);
    //使loop 第一个曲线与其他曲线首尾相连
    void SetLoopFirstConnect(ofts::DefLoop& loop,double err);
    //删除loop 中起点或终点与其他曲线不相连的曲线
    void DelNotConnectCurves(ofts::DefLoop& loop,double err);

    void EnabelCloseLoop(const ofts::DefLoop& loop,double e);
};
#endif // OPERATELOOP_H

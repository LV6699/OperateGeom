#ifndef OPERCLIPCURVE_H
#define OPERCLIPCURVE_H
#include "../../CommonFile/DataStructure.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class OperClipCurve
{
public:
    OperClipCurve(){}
    ///P 仅满足在loops 一个环中即返回
    bool IsPointInLoops(const ofts::Point& P,
                        std::vector<ofts::DefLoop>& loops,
                        bool isTemp,double bErr);
    bool IsValidPointBaseInLoops(const ofts::Point& P,
                                 std::vector<ofts::DefLoop>& loops,
                                 double bordPre,bool isIn);
    bool IsClipCurveValid(ofts::DefElem& c,
                          std::vector<ofts::DefLoop>& loops,
                          bool isTemp,double bordPre,
                          bool isIn,bool isOnce = false);
    ///参考loops 删除loop 中无效的元素.
    void LoopInvalidClipCurve(ofts::DefLoop& loop,
                              std::vector<ofts::DefLoop>& loops,
                              bool isTemp,double bordPre,
                              bool isIn,bool isOnce = false);
    ///oloops 是loops 裁减前的环,loops 中索引对应的环需要与oloops 中相同索引的环一致.
    void DeleteInvalidClipCurve(std::vector<ofts::DefLoop>& oloops,
                                std::vector<ofts::DefLoop>& loops,
                                double bordPre,bool isIn,
                                bool isOnce = false);
    void ResetCurvesOrder(ofts::DefLoop& loop, double e = PreErr5_6);
    void ArrangeCurveToLoop(const ofts::DefLoop& inloop,
                            double err, bool isClose,
                            std::vector<ofts::DefLoop> &loops);
    ///仅处理inloop 是闭环且无自交的情况
    void CloseLoopToEndtoend(const ofts::DefLoop& inloop,double err,
                             ofts::DefLoop& loop);
};
#endif // OPERCLIPCURVE_H

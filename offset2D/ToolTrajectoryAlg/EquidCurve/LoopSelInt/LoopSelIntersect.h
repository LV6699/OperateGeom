#ifndef LOOPSELINTERSECT_H
#define LOOPSELINTERSECT_H
#include "../../CommonFile/DataStructure.h"
class LoopSelIntersect
{
public:
    LoopSelIntersect(){}

    numst::IntParam GetIntParam()
    {
        numst::IntParam ipa;
        ipa._calParalLine = true;
        ipa.SetCalcTan(false);
        ipa.SetCalcParal(false);

        ipa.SetEndptRele(true,PreErr5_10);
        ipa.SetIntErr(PreErr5_10);
        ipa.SetOnCurveErr(PreErr5_10);
        ipa.SetTanErr(PreErr_10);
        ipa.SetParalErr(PreErr_10);
        return ipa;
    }
    bool IsOpen(const ofts::DefLoop& loop)
    {
        return loop.GetEndType() == Ofte::EndType::Open;
        //if(!loop.IsOpen() || loop.IsEndToEnd(PreErr5_10)){return false;}
        //if(param.GetEndType() == Ofte::EndType::Open){return true;}
        //return false;
    }
    void LoopClipPreprocesse(ofts::DefLoop& loop);
    bool OperateLoopSelInt(const ofts::DefLoop &oriLoop,
                           ofts::DefLoop &loop,
                           ofts::OffsetParam param,
                           double oftD,
                           std::vector<ofts::DefLoop> &rloops);
    void SelIntAfterProcess(const ofts::DefLoop& oloop,
                            std::vector<ofts::DefLoop>& loops,
                            std::vector<ofts::DefLoop>& rloops);
};
#endif // LOOPSELINTERSECT_H

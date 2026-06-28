#ifndef HANDLESELINT_H
#define HANDLESELINT_H
#include "ClipStruct.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class HandleSelInt
{
public:
    HandleSelInt(){}

    double RevereAgl(double a){a += 180;if(a >= 360){a -= 360;}return a;}
    bool IsCloseLoop(ofts::DefLoop& loop)
    {
        if(!loop.IsEndToEnd(PreErr_14)){return false;}
        _loops.push_back(loop);
        _bidss.emplace_back(_bids);
        loop.Clear();
        return true;
    }
    bool NextConnect(const ofts::Point& pre_ep,
                     std::vector<clst::CntCurve>& cnts,clst::CntCurve& cn)
    {
        auto& first = cnts.front();
        auto& last = cnts.back();
        if(first.IsValid() && !first.Curve()->IsDouClip()){
            if( pre_ep.IsEqual(first.Curve()->GetSp(),PreErr5_14)){
                cn = first;return true;
            }
        }
        if(last.IsValid() && !last.Curve()->IsDouClip()){
            if(pre_ep.IsEqual(last.Curve()->GetSp(),PreErr5_14)){
                cn = last;return true;
            }
        }
        if(cnts.size() < 3){return false;}
        for (size_t i = 1; i < cnts.size() - 1; i++){
            auto& cn1 = cnts[i];
            if(!cn1.IsValid()){continue;}
            if(!cn1.Curve()->IsDouClip()){
                cn = cn1;return true;
            }else{
                if(pre_ep.IsEqual(cn1.Curve()->GetSp(),PreErr5_14)){
                    cn = cn1;return true;
                }
            }
            cn = cn1;return true;
        }
        return false;
    }
    bool GetBuildCurve(std::vector<clst::ClipCurve>& ccs,
                       clst::ClipCurve& cl_c,bool isSp,
                       ofts::DefLoop& loop);

    std::vector<ofts::DefLoop>HandleSelIntLoop(const ofts::DefLoop& iloop);
    void BuildCurveToLoops(std::vector<clst::ClipCurve>& ccs);
    std::vector<ofts::DefLoop>SelIntLoopToLoops(const ofts::DefLoop& loop);

public:
    std::vector<size_t> _bids;
    std::vector<std::vector<size_t>>_bidss;
    std::vector<ofts::DefLoop> _loops;
};
#endif // HANDLESELINT_H

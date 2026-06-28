#include "LoopSelIntersect.h"
#include "LoopSelIntAux.h"
#include "OperOpenLoop.h"
#include"../../GeometryCalculate/RelativeContain/LoopContain.h"
#include "../../GeometryClip/ClipIntersect/OperClipCurve.h"
#include "../../OperCurve2D/OperCurve2D/ReconstructLoop.h"
#include "../../OperCurve2D/OperCurve2D/HandleLoop.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
void LoopSelIntersect::LoopClipPreprocesse(ofts::DefLoop& loop)
{
    OperateLoop().CircleArcLoopTrans(true,loop);
    for (auto& d : loop){
        d._label._hasMerge = false;
        d._label._isValid = true;
    }
    loop._label._isIntersect = false;
}
bool LoopSelIntersect::OperateLoopSelInt(const ofts::DefLoop& oloop,
                                         ofts::DefLoop& loop,
                                         ofts::OffsetParam param,double oftD,
                                         std::vector<ofts::DefLoop>& rloops)
{
    if(param._offsetUnit.IsPureEqud()){return false;}
#if 0
    bool isCnt = loop.IsConnect(PreErr5_12);
    DisplayGeom().DisplayLoop(oloop,_colors.back(),1,false,true);
    DisplayGeom().DisplayLoop(loop,_colors[0],1,true);
    string s = OperateOftString().ArrayLoopString(std::vector<ofts::DefLoop>{oloop,loop});
    std::cout<<s<<std::endl;//return true;
#endif
    rloops.clear();
    if(ReconstructLoop().IsCircleStructLoop(loop,numst::ReLoopErr(PreErr5_10,PreErr5_10)) ||
            loop.Size() < 2){return false;}
    auto clloop = loop;
    auto ipa = GetIntParam();
    HandleLoop().ClipLoopSelIntCurve(ipa,clloop);
    auto& cs = clloop.m_vElem;
    bool isOpen = IsOpen(loop);
    //DisplayGeom().DisplayLoop(clloop,_colors[1],1,true);return true;
    LoopSelIntAux aux;
    if(!aux.DelInvalidClipCurve(oloop,ipa,oftD,isOpen,cs) &&
            oloop.GetEndType() != Ofte::EndType::Joined){
        return false;
    }
    //DisplayGeom().DisplayLoop(clloop,_colors[2],1.5,true);return true;
    OperateLoop().DelLoopCoinCurve(clloop,PreErr5_10);
    OperateLoop().SetLoopFirstConnect(clloop,PreErr5_10);
    if(!isOpen){clloop.DelInvalid();}
    if(oloop.GetEndType() != Ofte::EndType::Polygon){
        aux.DeleDoubCntCurves(cs);
    }
    //DisplayGeom().DisplayLoop(clloop,_colors[3],2);return true;
    std::vector<ofts::DefLoop>bloops;
    aux.BuildCurveToLoops(clloop,isOpen,PreErr5_9,bloops);
    //DisplayGeom().DisplayLoops(bloops,_colors[4],2);return true;
    for(auto& d : bloops){auto l = loop;l.SetCurves(d.Curves());d = l;}
    std::vector<ofts::DefLoop>closes,opens;
    for(auto& l : bloops){
        if(!l.IsEndToEnd(PreErr5_9)){opens.emplace_back(l);}
        else{closes.emplace_back(l);}
    }
    for(auto& l : closes){
        OperateLoop().SetLoopEndToEnd(l,PreErr_14);
        l.SetLeftward(oloop.Leftward());
    }
    for(auto& l : opens){closes.emplace_back(l);}
    SelIntAfterProcess(oloop,closes,rloops);
    for(auto& d : rloops){LoopClipPreprocesse(d);}
    return true;
}
void LoopSelIntersect::SelIntAfterProcess(const ofts::DefLoop& oloop,
                                          std::vector<ofts::DefLoop>& loops,
                                          std::vector<ofts::DefLoop>& rloops)
{
    //numst::ContParam cpa(PreErr_6,PreErr_6,PreErr5_10);
    rloops.reserve(loops.size());
    auto olimc = LimitCoord2D::LoopLimitCoord(oloop);
    for(auto& d : loops){LimitCoord2D::SetLoopLimCoord(d);}
    for (size_t i = 0; i < loops.size(); ++i) {
        auto& loop = loops[i];
        loop.SetCW(oloop.IsCW());
        if(!CalculateValidity().IsMeetMinBoxMeasure(loops[i])){continue;}
        if(loops[i].GetEndType() == Ofte::EndType::Open){
            rloops.emplace_back(loops[i]);continue;
        }else{
            if(!loop.IsEndToEnd(PreErr5_5)){continue;}
        }
        loop.SetLeftward(oloop.Leftward());
        if(!loop.IsLegalNum()){continue;}
        const auto& coord = loop.GetLimCoord();
        bool iscw = LoopDirection().IsClockwiseLoop(loop);
        if(!iscw){loop.Reverse();}
        bool include = LimitCoord2D::IsLimitCoordCont(olimc,coord,PreErr5_8);
        if(oloop.GetEndType() != Ofte::EndType::Joined){
            if(!oloop.Leftward()){if(!include){continue;}}
            else{if(include && iscw){continue;}}
        }
#if 0
        std::vector<ofts::DefLoop>others;
        others.reserve(loops.size()-1);
        for (size_t j = 0; j < loops.size(); j++){
            if(i == j){continue;}
            others.emplace_back(loops[j]);
        }
        if(!iscw){
            bool isInclude = false;
            for(auto& d : others){
                isInclude = LoopContain::IsLoopContainLoop(d,loop,cpa);
                if(isInclude){break;}
            }
            if(isInclude){
                loop.Reverse();
                loop.SetLoopType(oloop.LoopType() == OUTSIDEPROFILE ?
                                     WITHINPROFILE : OUTSIDEPROFILE);
                loop.SetLeftward(oloop.LoopType() != OUTSIDEPROFILE);
            }else{
                continue;
            }
        }
        if(iscw && LoopContain::IsLoopsContLoop(others,loop,cpa)){
            continue;
        }
        bool isInclude = false;
        if(iscw){
            isInclude = LoopContain::IsLoopContLoopCoord(oloop,loop,PreErr5_10);
            if((oloop.LoopType() == OUTSIDEPROFILE && !isInclude) ||
                    (oloop.LoopType() == WITHINPROFILE && isInclude)){
                continue;
            }
        }
#endif
        rloops.emplace_back(loop);
    }
}

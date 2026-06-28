#include <iostream>
#include "LoopBoolUnio.h"
#include "../ClipIntersect/CurveIntClip.h"
#include "../ClipIntersect/OperClipCurve.h"
#include "../BoolClipAux/LoopClipAux.h"
#include "../../OperCurve2D/OperCurve2D/ReconstructLoop.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#ifdef emit
#undef emit
#include <tbb/tbb.h>
#define emit Q_EMIT
#else
#include <tbb/tbb.h>
#endif
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using std::string;
using std::vector;
using namespace OffsetStruct;
void LoopBoolUnio::InputPreprocesse(std::vector<ofts::DefLoop>& loops)
{
    GeomClipSolve().HandleCoincideLoops(loops);
    for(auto& l : loops){
        OperateLoop().OptCircleArcTrans(l,true,false,0,0);
        l.SetClipDele(false);
    }
}
///需要orils 与cloops 一一对应
void LoopBoolUnio::DeleteInvalidCurves(const std::vector<ofts::DefLoop>& orils,
                                       std::vector<ofts::DefLoop>& cloops)
{
#if 0
    string s;
    OperateOftString().ArrayLoopString(cloops,s);
    DisplayGeom().DisplayLoops(cloops,_colors.back(),1,true,true);
#endif
    LoopClipAux lca;Point mp;
    std::vector<size_t>ids;
    std::vector<ofts::DefLoop>others;
    others.reserve(cloops.size()-1);
    for (size_t i = 0; i < cloops.size(); ++i) {
        ids.clear();
        others.clear();
        auto& loop = cloops[i];
        for (size_t j = 0; j < orils.size(); ++j){
            if(j == i){continue;}
            others.emplace_back(orils[j]);
        }
        for (size_t j = 0; j < loop.Size(); ++j) {
            mp = BaseGeom2D::CurveMidpoint(loop[j]);
            if(lca.IsPointInLoops(others,mp,true,PreErr5_8)){
                ids.push_back(j);
                loop.SetClipDele(true);
            }
        }
        pnum::DeleteIndexElems(ids,loop.m_vElem);
        if(!ids.empty()){
            _hasClip = true;
        }
    }
#if 0
    string s1;
    OperateOftString().ArrayLoopString(cloops,s1);
    DisplayGeom().DisplayLoops(cloops,_colors.back(),1);
#endif
}
void LoopBoolUnio::LoopsBoolUnion(std::vector<ofts::DefLoop> inloops,
                                  std::vector<ofts::DefLoop>& rloops)
{
    if(inloops.size() < 2){rloops = inloops;return;}
    auto orils = inloops;
    InputPreprocesse(inloops);
#if 0
    string s = OperateOftString().ArrayLoopString(inloops);
    std::cout<<s<<std::endl;
    DisplayGeom().DisplayLoops(inloops,_colors.back(),1,false,true);
    ///return;
#endif
    rloops.reserve(inloops.size());
    numst::IntParam param;
    param._endptPre = PreErr_8;
    vector<DefLoop>cloops;
    CurveIntClip::LoopsMutualClip(inloops,param,cloops);
#if 0
    string s1 = OperateOftString().ArrayLoopString(cloops);
    std::cout<<s1<<std::endl;
    DisplayGeom().DisplayLoops(cloops,_colors[1],1,true,true);
    ///return;
#endif
    DeleteInvalidCurves(orils,cloops);
    if(!HasClip()){
        rloops = inloops;return;
    }
#if 0
    string s2 = OperateOftString().ArrayLoopString(cloops);
    std::cout<<s2<<std::endl;
    DisplayGeom().DisplayLoops(cloops,_colors[2],1.5,true);
    ///return;
#endif
    DefLoop aloop;
    std::vector<ofts::DefLoop>nocl_ls,cl_ls;
    for(auto& l : cloops){
        if(l.HasClipDele()){
            for(auto& c : l){aloop.AddElem(c);}continue;
        }
        nocl_ls.push_back(l);
    }
    OperClipCurve opc;
    opc.ArrangeCurveToLoop(aloop,PreErr_8,true,cl_ls);
    if(cloops[0].HasClipDele()){
        for(auto& l : cl_ls){rloops.emplace_back(l);}
        for(auto& l : nocl_ls){rloops.emplace_back(l);}
    }else{
        for(auto& l : nocl_ls){rloops.emplace_back(l);}
        for(auto& l : cl_ls){rloops.emplace_back(l);}
    }
#if 0
    string s3 = OperateOftString().ArrayLoopString(rloops);
    std::cout<<s3<<std::endl;
    DisplayGeom().DisplayLoops(rloops,_colors[0],2,true,true);
#endif
}

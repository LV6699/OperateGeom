#include "LoopBoolSubt.h"
#include "../ClipIntersect/CurveIntClip.h"
#include "../../OperCurve2D/OperCurve2D/ReconstructLoop.h"
#include "../BoolClipAux/LoopClipAux.h"
#include "../ClipIntersect/OperClipCurve.h"
#include "../../GeometryCalculate/RelativeContain/PointInLoop.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using std::vector;
using namespace OffsetStruct;
void LoopBoolSubt::InputPreprocesse(DefLoop& tloop,
                                    vector<DefLoop>& loops)
{
    OperateLoop().OptCircleArcTrans(tloop,true,false,0,0);
    tloop.SetClipDele(false);
    for(auto& l : loops){
        OperateLoop().OptCircleArcTrans(l,true,false,0,0);
        l.SetClipDele(false);
    }
}
void LoopBoolSubt::DeleteInvalidCurves(DefLoop& tloop,DefLoop& tcloop,
                                       vector<DefLoop>& loops,
                                       vector<DefLoop>& cloops)
{
    LoopClipAux lca;
    Point mp;vector<size_t>ids;
    ids.reserve(tcloop.Size());
    LimitCoord2D::SetLoopLimCoord(tloop);
#if 1
    for (size_t i = 0; i < tcloop.Size(); ++i){
        mp = BaseGeom2D::CurveMidpoint(tcloop[i]);
        if(lca.IsPointInLoops(loops,mp,true,PreErr5_8)){
            ids.emplace_back(i);
        }
    }
#else
    tbb::combinable<std::vector<size_t>> localResults;
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,tcloop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        auto& localVec = localResults.local();
        localVec.reserve(r.size());
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            BaseGeom2D::CurveMidpoint(tcloop[i],mp);
            if(lca.IsPointInLoops(loops,mp,true,PreErr5_10)){
                localVec.emplace_back(i);
            }
        }
    });
    localResults.combine_each([&](const std::vector<size_t>& vec) {
        ids.insert(ids.end(), vec.begin(), vec.end());
    });
#endif
    if(ids.empty()){return;}
#if 0
    DisplayGeom().DisplayLoop(tcloop,_colors.back(),1,true,true);
    DisplayGeom().DisplayLoops(cloops,_colors.back(),1);
#endif
    pnum::DeleteIndexElems(ids,tcloop.m_vElem);
    ///DisplayGeom().DisplayLoop(tcloop,_colors[1],2);
    for (size_t i = 0; i < cloops.size(); ++i) {
        ids.clear();
        auto& loop = cloops[i];
        for (size_t j = 0; j < loop.Size(); ++j) {
            mp = BaseGeom2D::CurveMidpoint(loop[j]);
            bool isOnBorder = false,isInPoly = false;
            ///pip.IsPointInsideLoop(tloop,mp,PreErr5_10,isOnBorder,isInPoly);
            PointInLoop().GetPointInsideLoop(tloop,mp,PreErr5_8,
                                             isOnBorder,isInPoly);
            if(!isInPoly && !isOnBorder){
                ids.emplace_back(j);
                loop.SetClipDele(true);
                _hasClip = true;
            }
        }
        pnum::DeleteIndexElems(ids,loop.m_vElem);
    }
    ///DisplayGeom().DisplayLoops(cloops,_colors[1],2);
}
void LoopBoolSubt::LoopBoolSubtract(DefLoop tloop,vector<DefLoop> inloops,
                                    vector<DefLoop>& rloops)
{
#if 0
    GeomArea r(tloop,inloops);
    string s = OperateOftString().BndIslLoopString(r);
    std::cout<<s<<std::endl;
    DisplayGeom().DisplayLoop(tloop,_colors.back(),1,true,true);
    DisplayGeom().DisplayLoops(inloops,_colors.back(),1.5,true);
    ///return;
#endif
    numst::IntParam param;
    param._endptPre = PreErr_8;
    DefLoop tcloop;vector<DefLoop>cloops;
    InputPreprocesse(tloop,inloops);
    if(inloops.empty()){return;}
    CurveIntClip::LoopsClipLoop(tloop,inloops,param,tcloop);
    for (size_t i = 0; i < inloops.size(); ++i) {
        DefLoop cl;
        CurveIntClip::LoopClipLoop(inloops[i],tloop,param,cl);
        cloops.emplace_back(cl);
    }
#if 0
    string s1 = OperateOftString().LoopElementString(tcloop);
    string s1_ = OperateOftString().ArrayLoopString(cloops);
    std::cout<<s1<<s1_<<endl;
    DisplayGeom().DisplayLoop(tcloop,_colors[1],1,true,true);
    DisplayGeom().DisplayLoops(cloops,_colors[1],2,true);
    ///return;
#endif
    DeleteInvalidCurves(tloop,tcloop,inloops,cloops);
    if(!HasClip()){
        rloops.reserve(rloops.size()+1);
        rloops.emplace_back(tloop);
        for(auto& l : inloops){rloops.emplace_back(l);}
        return;
    }
#if 0
    string s2 = OperateOftString().LoopElementString(tcloop);
    string s2_ = OperateOftString().ArrayLoopString(cloops);
    std::cout<<s2<<s2_<<endl;
    DisplayGeom().DisplayLoop(tcloop,_colors[2],1);
    DisplayGeom().DisplayLoops(cloops,_colors[2],1.5);
    ///return;
#endif
    OperClipCurve opc;
    DefLoop aloop = tcloop;
    std::vector<ofts::DefLoop>nocl_ls,cl_ls;
    for(auto& l : cloops){
        if(l.HasClipDele()){
            for(auto& c : l){aloop.AddElem(c);}continue;
        }
        nocl_ls.emplace_back(l);
    }
    opc.ArrangeCurveToLoop(aloop,PreErr_8,true,cl_ls);
    for(auto& l : cl_ls){
        rloops.emplace_back(l);
    }
    for(auto& l : nocl_ls){
        rloops.emplace_back(l);
    }
#if 0
    string s3 = OperateOftString().ArrayLoopString(rloops);
    std::cout<<s3<<endl;
    DisplayGeom().DisplayLoops(rloops,_colors[0],2);
#endif
}

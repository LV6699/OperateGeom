#include <iostream>
#include "ClipPlaneRegion.h"
#include "../GeomBoolInt/LoopBoolInt.h"
#include "../../GeometryCalculate/GeomIntersect/IsCurveInt2D.h"
#include "../GeomBoolSubtract/LoopBoolSubt.h"
#include "../GeomBoolUnion/LoopBoolUnio.h"
#include "../../GeometryCalculate/RelativeContain/LoopContain.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
using std::vector;
using namespace OffsetStruct;
void ClipPlaneRegion::IntRegionLoopClip(const vector<ofts::DefLoop>&intBs,
                                        const vector<ofts::DefLoop>&intIs,
                                        const ofts::DefLoop& loop,
                                        vector<ofts::DefLoop>& rloops)
{
    LoopBoolSubt lbs;LoopBoolInt lbi;
    for (size_t i = 0; i < intBs.size(); ++i) {
    }
}
void IntBoundaryLoopClip(const ofts::DefLoop& loop,
                         const ofts::DefLoop& bloop,
                         const vector<ofts::DefLoop>& intIs,
                         vector<DefLoop>& rloops)
{
    vector<DefLoop>rloops_;
    NumStruct::IntParam param;
    param._endptPre = PreErr_6;
    LoopBoolSubt lbs;LoopBoolInt lbi;
    lbi.LoopBoolIntersection(vector<DefLoop>{bloop,loop},param,rloops_);
#if 0
    DisplayGeom().DisplayLoops(rloops_,_colors[2],2);
    string s = OperateOftString().ArrayLoopString(rloops_);
    std::cout<<s<<endl;
#endif
    if(rloops_.empty()){
        rloops_.push_back(loop);
    }
    if(intIs.empty()){
#if 0
        DisplayGeom().DisplayLoops(rloops_,_colors[2],2);
        string s1 = OperateOftString().ArrayLoopString(rloops_);
        std::cout<<s1<<endl;
#endif
        for(auto& l : rloops_){
            rloops.push_back(l);
        }
        return;
    }
    for (size_t i = 0; i < rloops_.size(); ++i) {
        vector<DefLoop>rls;
        lbs.LoopBoolSubtract(rloops_[i],intIs,rls);
#if 0
        DisplayGeom().DisplayLoops(rls,_colors[2],2);
        string s1 = OperateOftString().ArrayLoopString(rls);
        std::cout<<s1<<endl;
#endif
        for(auto& l : rls){
            rloops.push_back(l);
        }
    }
}
void ClipPlaneRegion::RegionLoopsBoolInt(const ofts::GeomArea& region,
                                         const vector<ofts::DefLoop>& inloops,
                                         vector<ofts::DefLoop>& rloops)
{
    LoopBoolSubt lbs;
    numst::IntParam param;
    param.SetEndptRele(false,PreErr_4);
    auto bloop = region._bndLoop;
    auto iloops = region._vIslLoop;
    LimitCoord2D::SetLoopLimCoord(bloop);
#if 0
    DisplayGeom().DisplayGeomArea(region);
    string s = OperateOftString().BndIslLoopString(region);
    std::cout<<s<<std::endl;
#endif
    for(size_t i = 0;i < inloops.size();i++){/**
        if(i == 4){
            std::cout<<i<<std::endl;
        }*/
        bool isIntBl = false;
        vector<ofts::DefLoop>intIs;
        auto l = inloops[i];
        l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));
        if(IsCurveInt2D().IsLoopIntersect(l,bloop,param)){
            isIntBl = true;
        }
        for (size_t j = 0;j < iloops.size();j++){
            if(IsCurveInt2D().IsLoopIntersect(l,iloops[j],param)){
                intIs.push_back(iloops[j]);
            }else{
                ///删除rloops 中在岛屿内部的环,理论上这里不会存在这样的环
            }
        }
#if 0
        DisplayGeom().DisplayLoop(inloops[i],_colors[1],1/*,false,true*/);
        ///DisplayGeom().DisplayLoops(intIs,_colors[4],1);
        string s1 = OperateOftString().ArrayLoopString(intIs);
        std::cout<<s1<<std::endl;
#endif
        if(isIntBl){
            vector<DefLoop>rls;
            IntBoundaryLoopClip(l,bloop,intIs,rls);
#if 0
            DisplayGeom().DisplayLoops(rls,_colors[2],3);
            string s2_ = OperateOftString().ArrayLoopString(rls);
            std::cout<<s2_<<std::endl;
#endif
            for(auto& l_ : rls){
                rloops.push_back(l_);
            }
            continue;
        }else{
            ///删除inloops 中在边界外部的环
            numst::ContParam cparam(PreErr5_10,PreErr5_6,PreErr5_8,true);
            if(!LoopContain::IsLoopContainLoop(bloop,l,cparam)){
                continue;
            }
        }
        if(intIs.empty()){rloops.push_back(l);continue;}
        vector<DefLoop>rls_;
        lbs.LoopBoolSubtract(l,intIs,rls_);
#if 0
        DisplayGeom().DisplayLoops(rls_,_colors[2],3);
        string s2 = OperateOftString().ArrayLoopString(rls_);
        std::cout<<s2<<std::endl;
#endif
        for(auto& l_ : rls_){
            rloops.push_back(l_);
        }
    }
#if 0
    DisplayGeom().DisplayLoops(rloops,_colors[0],2);
    string s3 = OperateOftString().ArrayLoopString(rloops);
    std::cout<<s3<<std::endl;
#endif
}

#ifndef CREATEFITTING_H
#define CREATEFITTING_H
#include "CreateFitting.h"
#include "../../CommonFile/Header.h"
#include "./GeomFitting/ExicuteFitt.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"

#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
using namespace OffsetStruct::Fitt2D;

class CreateFitting
{
public:
    CreateFitting(){}
    void FittingLoopCurves(const numst::FitParam& param,ofts::DefLoop& loop)
    {
        //std::string s = OperateOftString().LoopElementString(loop);
        //std::cout<<s<<std::endl;
        //DisplayGeom().DisplayLoop(loop,_colors.back(),1,true,true);
        std::vector<gfit::FittArc>fitts;
        ExicuteFitt().ExicuteFittSegToArc(param,loop,fitts);
        if(fitts.empty()){return;}
        auto rloop = ExicuteFitt().FittingDataToLoop(param,loop,fitts);
        //DisplayGeom().DisplayLoop(rloop,_colors.back(),1,false,false);
        //bool isc = rloop.IsConnect(PreErr5_13);
        loop = rloop;
    }
    void FittingGeomRegion(const numst::FitParam& param,ofts::GeomArea& region)
    {
        FittingLoopCurves(param,region._bndLoop);
        for (size_t i = 0; i < region.ILoops().size(); ++i) {
            FittingLoopCurves(param,region[i]);
        }
    }
    void FittingLoopArcSeg(const numst::FitParam& param,
                           ofts::DefLoop& loop)
    {
        //bool isc = loop.IsConnect(PreErr_13);
        ExicuteFitt().FittingLoopSegment(param,loop);
        //isc = loop.IsConnect(PreErr_13);
        ExicuteFitt().FittingLoopArc(param,loop);
        //isc = loop.IsConnect(PreErr_13);
    }
    void FittingRegionArcSeg(const numst::FitParam& param,
                             ofts::GeomArea& region)
    {
        //bool isc = region._bndLoop.IsConnect(PreErr_13);
        FittingLoopArcSeg(param,region._bndLoop);
        //isc = region._bndLoop.IsConnect(PreErr_13);
        for(auto& l : region){
            FittingLoopArcSeg(param,l);
            //isc = l.IsConnect(PreErr_13);
        }
    }
};
#endif // CREATEFITTING_H

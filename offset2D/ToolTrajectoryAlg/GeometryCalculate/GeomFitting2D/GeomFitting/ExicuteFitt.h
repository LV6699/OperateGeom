/**
  @author: lvdong
  @date: 2025-03
**/
#ifndef EXICUTEFITT_H
#define EXICUTEFITT_H
#include"GeomFittAux.h"
#include"../../../CommonFile/Header.h"
#include"../../../../OperateView/OperateView/DisplayGeom.h"
//using namespace gfit;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class ExicuteFitt
{
public:
    ExicuteFitt(){}
    void ResetFittingData(gfit::FittArc &fit,std::vector<gfit::FittArc>& fitts)
    {
        if(fit.Ids().size() > 1){fitts.emplace_back(fit);}
        fit = gfit::FittArc();
    }
    void InvalidFitting(size_t i,std::vector<gfit::FittArc>& fitts,
                        gfit::FittArc& fit){
        fit.IdsPopBack();
        fit.SetFitArc(fit.PreArc());
        ResetFittingData(fit,fitts);
        fit.AddId(i);
    }
    void ExicuteFittSegToArc(const numst::FitParam& param,
                             const ofts::DefLoop& loop,
                             std::vector<gfit::FittArc>& fitts)
    {
        gfit::FittArc fit;
        GeomFittAux gfs;
        fitts.reserve(loop.Size() / 5);
        //bool isc = loop.IsConnect(PreErr_13);
        //DisplayGeom().DisplayLoop(loop,_colors.back(),1);
        for (size_t i = 0; i < loop.Size(); i++){/**
            if(i == 188 || i == 278 || fitts.size() == 1){
                int tem = 1;
            }*/
            if(!gfs.IsCurveMeet(param,loop[i])){
                ResetFittingData(fit,fitts);continue;
            }
            if(fit.Ids().empty()){
                fit.AddId(i);continue;
            }
            if(!gfs.IsMeetAdjoinAngle(param,loop,i)){
                //DisplayGeom().DisplayArc(fit.PreArc(),_colors[1],1);
                ResetFittingData(fit,fitts);
                //fit.AddId(i);
            }
            fit.AddId(i);
            if(!gfs.IsMeetParam(loop,param,fit)){
                //DisplayGeom().DisplayArc(fit.PreArc(),_colors[2],1);
                InvalidFitting(i,fitts,fit);continue;
            }
            if((i == loop.Size() - 1) && fit.Ids().size() > 2){
                fitts.push_back(fit);
            }
            //int tem = 1;
        }
    }
    void FittingLoopSegment(const numst::FitParam& param,ofts::DefLoop& loop)
    {
        if(loop.Size() < 2){return;}
        double err = pnum::AglToSinSquare(param.SegLinear());
        //double err = param.SegLinear();
        struct FitSeg{
            FitSeg(){}
            ofts::DefSeg seg;
            std::vector<size_t>ids;
            bool isInitVec = false;
        };
        FitSeg fit;
        auto AddData = [&](){
            auto& ids = fit.ids;
            if(ids.size() < 2){fit = FitSeg();return;}
            loop[ids[0]]._segment.SetEp(loop[ids.back()]._segment.GetEp());
            ids.erase(ids.begin());
            for(auto& d : ids){
                loop[d].SetValid(false);
            }
            fit = FitSeg();
        };
        auto IsFitting = [&](size_t i,const ofts::DefSeg& s){
            if(fit.ids.empty()){
                fit.seg = s;
                fit.ids.emplace_back(i);
                if(s.LenSquare() >= PreErr_10){fit.isInitVec = true;}
                return true;
            }
            if(!fit.isInitVec){
                fit.seg.SetEp(s.GetEp());
                if(fit.seg.LenSquare() >= PreErr_10){fit.isInitVec = true;}
                fit.ids.emplace_back(i);
                return true;
            }
            if(s.LenSquare() > PreErr_6 &&
                    /*!CurveQual2D::IsLineSlopeEqual(s,fit.seg,err)*/
                    !CurveQual2D::IsLineAglEqual(s,fit.seg,err)){
                return false;
            }
            fit.ids.emplace_back(i);return true;
        };
        for (size_t i = 0; i < loop.Size(); ++i) {
            /*if(i == 1702 || i == 1716){
                int tem = 1;
            }*/
            auto& curve = loop[i];
            if(curve.Type() != Ofte::LINETYPE){
                AddData();continue;
            }
            if(IsFitting(i,curve.GetSeg())){
                continue;
            }
            AddData();
            i --;
        }
        AddData();  //添加最后一次数据
        loop.DelInvalid();
        //bool isc = loop.IsConnect(PreErr_13);
    }
    void FittingLoopArc(const numst::FitParam& param,ofts::DefLoop& loop)
    {
        //DisplayGeom().DisplayLoop(loop,_colors.back(),1,true);
        if(loop.Size() < 2){return;}
        struct FitArc{
            FitArc(){}
            ofts::DefArc arc;
            std::vector<size_t>ids;
            bool isInitial = false;
        };
        FitArc fit;
        auto AddData = [&](){
            auto& ids = fit.ids;
            if(ids.size() < 2){fit = FitArc();return;}
            double r = 0,x = 0,y = 0,z = 0;
            for(auto& d : ids){
                r += loop[d].GetArc().R();
                const auto& cp = loop[d].GetArc().GetCp();
                x += cp.X();y += cp.Y();z += cp.Z();
            }
            double n = static_cast<double>(ids.size());
            r /= n;x /= n;y /= n;z /= n;
            loop[ids[0]]._arc.SetR(r);
            loop[ids[0]]._arc.SetCp(ofts::Point(x,y,z));
            loop[ids[0]]._arc.SetEp(loop[ids.back()]._arc.GetEp());
            ids.erase(ids.begin());
            for(auto& d : ids){
                loop[d].SetValid(false);
            }
            fit = FitArc();
        };
        auto IsFitting = [&](size_t i,const ofts::DefArc& arc){
            if(!fit.isInitial){
                fit.arc = arc;
                fit.isInitial = true;
                fit.ids.emplace_back(i);return true;
            }
            if(std::abs(arc.R() - fit.arc.R()) > param.RadiusErr()){
                return false;
            }
            if(std::abs(arc.IsCW() != fit.arc.IsCW())){
                return false;
            }
            if(!arc.GetCp().IsEqual(fit.arc.GetCp(),param.CenterErr())){
                return false;
            }
            fit.ids.emplace_back(i);return true;
        };
        for (size_t i = 0; i < loop.Size(); ++i) {
            auto& curve = loop[i];
            if(curve.Type() != Ofte::ARCTYPE){
                AddData();continue;
            }
            if(IsFitting(i,curve.GetArc())){
                continue;
            }
            AddData();
            i --;
        }
        AddData();
        loop.DelInvalid();
        //DisplayGeom().DisplayLoop(loop,_colors[1],2,true,true);
        //bool isc = loop.IsConnect(PreErr_13);
    }
    ofts::DefLoop FittingDataToLoop(const numst::FitParam& param,
                                    ofts::DefLoop loop,
                                    const std::vector<gfit::FittArc>& ofitts)
    {
        auto fitts = ofitts;
        auto rloop = loop;rloop.Clear();
        if(fitts.size() == 1){
            const auto& arc = fitts[0].FitArc();
            if(arc.GetSp().IsEqual(arc.GetEp(),PreErr5_10)){
                const auto& cp = arc.GetCp();
                rloop.AddElem(ofts::DefElem(ofts::DefCircle(cp,arc.R())));
                return rloop;
            }
        }
        for(auto& d : loop){d.SetValid(true);}
        for(auto& d : fitts){
            for(auto& i : d._ids){
                loop[i].SetValid(false);
            }
        }
        for (size_t i = 0; i < loop.Size(); ++i) {
            if(loop[i].IsValid()){
                rloop.AddElem(loop[i]);continue;
            }
            auto& fit = fitts.front();
            if(fit.Centers().empty() || fit.Rs().empty() || fit.Ids().empty()){
                rloop.AddElem(loop[i]);continue;
            }
            rloop.AddElem(ofts::DefElem(fit.FitArc()));
            i = fit.Ids().back();
            //if(!fitts.empty()){
            fitts.erase(fitts.begin());
            //}
        }
        if(fitts.size() == 2){
            numst::ReLoopErr reErr(param.Bh(),param.Bh(),param.Bh());
            OperateLoop().ArcLoopToCircle(rloop,reErr);
        }
#if 0
        std::vector<int>ids;
        for (int i = 0; i < rloop.Size(); ++i) {
            if(rloop[i].Type() == Ofte::ARCTYPE){ids.push_back(i);}
        }
        if(ids.size() != ofitts.size()){
            std::cout<<"";
        }
        bool isc = rloop.IsConnect(PreErr_13);
        int tem = 1;
#endif
        return rloop;
    }

};
#endif // EXICUTEFITT_H

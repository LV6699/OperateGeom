#include "LoopDirection.h"
#include "../RelativeContain/LimitCoord.h"
#include "../GeomFitting2D/CreateFitting.h"
#include "../../OperCurve2D/OperCurve2D/OperateCurve.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#include<iostream>
#pragma optimize("", off)
#endif
bool LoopDirection::IsClockwiseLoop(const ofts::DefLoop& inloop)
{
    //DisplayGeom().DisplayLoop(inloop,_colors.back(),1,true,true);
    if(inloop.IsCircleLoop()){return inloop[0].GetCircle().IsCW();}
    if(inloop.Size() < 2){return false;}

    auto loop = inloop;
    numst::FitParam param(PreErr5_8,PreErr5_8,PreErr5_8);
    CreateFitting().FittingLoopArcSeg(param,loop);
    //DisplayGeom().DisplayLoop(loop,_colors.back(),1,true);
    std::vector<std::pair<size_t,ofts::Point>>datas;
    datas.reserve(loop.Size());
    for (size_t i = 0; i < loop.Size(); ++i) {
        auto p = LimitCoord2D::CurveLimitPoint(loop[i],true,true);
        datas.emplace_back(std::make_pair(i,p));
    }
    std::sort(datas.begin(),datas.end(),[](const std::pair<size_t,ofts::Point>& a,
              const std::pair<size_t,ofts::Point>& b){return a.second.X() < b.second.X();});
    std::vector<std::pair<size_t,ofts::Point>>datas1;
    datas1.reserve(static_cast<size_t>(datas.size() / 2));
    /*if(datas.empty()){
        DisplayGeom().DisplayLoop(inloop,_colors.back(),1,true,true);
        DisplayGeom().DisplayLoop(loop,_colors.back(),1,true);
        auto s = OperateOftString().LoopElementString(inloop);
        std::cout<<s<<std::endl;
    }*/
    double coord = datas.back().second.X();
    for(auto& d : datas){if(d.second.X() == coord){datas1.emplace_back(d);}}

    double step = PreErr_3;
    auto ExplorePt = [&](size_t id,const ofts::Point& p,
            ofts::Point& p1,ofts::Point& p2){
        size_t it = 0,pid = id,nid = id;
        const auto& c = loop[id];
        auto prec = c.GetSplit(p,true),nexc = c.GetSplit(p,false);
        if(p.IsEqual(c.GetSp(),PreErr5_14)){
            pid = pnum::PreId(loop.Size(),id);
            prec = loop[pid];
        }else if(p.IsEqual(c.GetEp(),PreErr5_14)){
            nid = pnum::NexId(loop.Size(),id);
            nexc = loop[nid];
        }
        double l1 = CurveLog2D::CurveLenth(prec);
        double l2 = CurveLog2D::CurveLenth(nexc);
        bool isPreFin = false,isNexFin = false;
        if(l1 >= step){isPreFin = true;}
        else{pid = loop.PreId(id);}
        if(l2 >= step){isNexFin = true;}
        else{nid = loop.NexId(id);}
        if(isPreFin && isNexFin){
            p1 = CurveLog2D::NearCurveEndpt(prec,false,false,step,true);
            p2 = CurveLog2D::NearCurveEndpt(nexc,true,false,step,true);
            return p1.X() < p.X() || p2.X() < p.X();
        }
        double step1 = step,step2 = step;
        while (true) {
            if(!isPreFin){
                double l = CurveLog2D::CurveLenth(loop[pid]);
                if(l1 + l >= step){
                    step1 = step - l1;
                    isPreFin = true;
                }else{
                    l1 += l;
                    pid = loop.PreId(pid);
                }
            }
            if(!isNexFin){
                double l = CurveLog2D::CurveLenth(loop[nid]);
                if(l2 + l >= step){
                    step2 = step - l2;
                    isNexFin = true;
                }else{
                    l2 += l;
                    nid = loop.NexId(nid);
                }
            }
            if(isPreFin && isNexFin){break;}
            it ++;
            if(it > static_cast<size_t>(loop.Size() / 2)){return false;}
        }
        p1 = CurveLog2D::NearCurveEndpt(loop[pid],false,false,step1,true);
        p2 = CurveLog2D::NearCurveEndpt(loop[nid],true,false,step2,true);
        return p1.X() < p.X() || p2.X() < p.X();
    };
    ofts::Point op0,op1,op2;
    for(size_t i = 0;i < datas1.size();++i){
        const auto& d = datas1[i];
        op1 = d.second;
        if(!ExplorePt(d.first,op1,op0,op2)){
            continue;
        }
        //DisplayGeom().DisplayPoint(op0,_colors[0]);
        //DisplayGeom().DisplayPoint(op1,_colors[1]);
        //DisplayGeom().DisplayPoint(op2,_colors[2]);
        bool iscw = BaseGeom2D::IsTriaRotDireCW(op0,op1,op2);
        return iscw;
    }
    return false;
}

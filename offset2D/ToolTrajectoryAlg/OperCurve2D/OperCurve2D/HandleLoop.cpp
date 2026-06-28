#include "HandleLoop.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void HandleLoop::ClipLoopSelIntCurve(const numst::IntParam& param,
                                     ofts::DefLoop& loop)
{
    struct InloopInt{
        InloopInt(){}
        size_t id;
        std::vector<ofts::Point>ps;
    };
    ofts::GeomIntRes iRes;
    std::vector<InloopInt>datas(loop.Size(),InloopInt());
    for (size_t i = 0; i < loop.Size(); i++){loop[i].SetId(i);}

    auto AddData = [&](const ofts::DefElem& c,InloopInt& data){
        if(!c.IsEndpoint(iRes.P1(),param.EndptPre())){
            data.ps.emplace_back(iRes.P1());
        }
        if(iRes.Num() < 2){return;}
        if(!c.IsEndpoint(iRes.P2(),param.EndptPre())){
            data.ps.emplace_back(iRes.P2());
        }
    };
    for (size_t i = 0; i < loop.Size(); i++){
        auto& data = datas[i];
        const auto& c = loop[i];
        for (size_t j = i+1; j < loop.Size(); j++){
            auto& data1 = datas[j];
            const auto& c1 = loop[j];
            /*if(i == 16 && j == 31){
                DisplayGeom().DisplayElement(c,_colors[0],1,true,true);
                DisplayGeom().DisplayElement(c1,_colors[1],1,true);
                int tem = 1;
            }*/
            CurveInt2D::GetCurveIntersect(c,c1,param,iRes);
            if(!iRes.IsInt()){continue;}
            AddData(c,data);
            AddData(c1,data1);
            iRes.Reset();
        }
    }
    for(auto& d : datas){
        BaseGeom2D::DeduplicatePts(d.ps,param.EndptPre());
    }
    std::vector<ofts::DefElem>rcs;
    rcs.reserve(loop.Size()*2);
    for (size_t i = 0; i < loop.Size(); i++){
        auto& c = loop[i];
        auto& ps = datas[i].ps;
        if(ps.empty()){rcs.emplace_back(c);continue;}

        std::vector<ofts::DefElem>cs;
        CurveLog2D::PointsClipCurve(c,ps,cs);
        cs[0].SetEpClip(true);
        cs.back().SetSpClip(true);
        for (size_t j = 1; j < cs.size()-1; j++){
            cs[j].SetSpClip(true);cs[j].SetEpClip(true);
        }
        for(auto& d : cs){rcs.emplace_back(d);}
    }
    loop.SetCurves(rcs);
}

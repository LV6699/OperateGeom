#include "HandleSelInt.h"
#include "../../../OperCurve2D/OperCurve2D/HandleLoop.h"
#include "../../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
bool HandleSelInt::GetBuildCurve(std::vector<clst::ClipCurve>& ccs,
                                 clst::ClipCurve& cl_c,bool isSp,
                                 ofts::DefLoop& loop)
{
    //获取与当前曲线终点相连的曲线,isSp 指是否与cl_c 起点相连
    //auto& cnt = isSp ? cl_c._spCnts[0] : cl_c._epCnts[0];
    auto& bids = _bids;auto& bidss = _bidss;auto& loops = _loops;
    if(!cl_c.IsValid()){
        loop.Clear();return false;
    }
    if(isSp){
        loop.AddElem(cl_c.Curve());  //loop[cnt.Id()]
    }else{
        auto c = cl_c.Curve();
        c.Reverse();loop.AddElem(c);
    }
    bids.emplace_back(cl_c.Id());
    cl_c.SetVisited(true);
    if(IsCloseLoop(loop)){
        return true;
    }
    auto& cnts = isSp ? cl_c._epCnts : cl_c._spCnts;
    clst::CntCurve cnt;
    if(!NextConnect(cl_c.Curve().GetEp(),cl_c._epCnts,cnt)){
        return false;
    }
    auto& id = cnt.Id();
    isSp = cnt.IsSp();
    return GetBuildCurve(ccs,ccs[id],isSp,loop);
}
void HandleSelInt::BuildCurveToLoops(std::vector<clst::ClipCurve>& ccs)
{
    ofts::DefLoop loop;loop.Reserve(ccs.size());
    for (size_t i = 0; i < ccs.size(); i++){ccs[i].SetId(i);}
    auto& bids = _bids;auto& bidss = _bidss;
    auto& loops = _loops;bids.reserve(ccs.size());
    //遍历ccs 组建封闭环,首先遍历最多只有一端被裁剪的曲线
    for (size_t i = 0; i < ccs.size(); i++){
        auto& cl_c = ccs[i];
        if(!cl_c.IsValid()){continue;}
        auto& curve = cl_c._curve;

        cl_c.SetVisited(true);
        loop.AddElem(curve);
        if(IsCloseLoop(loop)){
            bidss.back().emplace_back(cl_c.Id());bids.clear();
            continue;
        }
        if(cl_c.EpCnts().size() < 2){
            bids.emplace_back(cl_c.Id());continue;
        }
        clst::CntCurve cnt;
        if(!NextConnect(curve.GetEp(),cl_c._epCnts,cnt)){
            continue;
        }
        size_t id = cnt.Id();
        bool isSp = cnt.IsSp();
        if(GetBuildCurve(ccs,cl_c,isSp,loop)){
            for(auto& d : bids){
                for(auto& c : ccs){
                    if(!c.IsValid() || d != c.Id()){continue;}
                    auto& cl_c1 = ccs[d];
                    cl_c1.IncVisNum();
                    cl_c1.JudgeValid();
                }
            }
        }
        bids.clear();
    }
    //再次遍历两端均被裁剪的曲线,这类曲线是两个边界的公共边
    //BuildLoop(false);
#if 0
    for (size_t i = 0; i < loops.size(); i++){
        DisplayGeom().DisplayLoop(loops[i],_colors[i],1,true);
        bool isc = loops[i].IsConnect(PreErr_14);
        std::cout<<"";
    }
#endif
}
std::vector<ofts::DefLoop>HandleSelInt::HandleSelIntLoop(const ofts::DefLoop& iloop)
{
    auto loop = iloop;
    numst::IntParam param;
    param.SetIntErr(PreErr_10);param.SetOnCurveErr(PreErr_10);
    param.SetTanErr(PreErr_10);param.SetParalErr(PreErr_8);
    param.SetParalAxisErr(PreErr_10);param.SetCalcTan(false);
    param.SetCalcParal(false);param.SetEndptRele(false,PreErr_8);
    size_t onum = loop.Size();
    HandleLoop().ClipLoopSelIntCurve(param,loop);
    //DisplayGeom().DisplayLoop(loop,_colors.back(),1,true);  //return _loops;
    if(onum == loop.Size()){return vector<ofts::DefLoop>{loop};}
    for (size_t i = 0; i < loop.Size(); i++){loop[i].SetId(i);}
    double endPre = param.EndptPre();
    std::vector<clst::ClipCurve>ccs;ccs.reserve(loop.Size());
    for (size_t i = 0; i < loop.Size(); i++){
        ccs.emplace_back(clst::ClipCurve(i,loop[i]));
    }
    auto AddCntCurve = [&](const size_t& i,const size_t& j,const ofts::Point& ip,
            const ofts::Point& jp,bool isiSp,bool isjSp,std::vector<clst::CntCurve>& ips,
            std::vector<clst::CntCurve>& jps){
        if(!ip.IsEqual(jp,endPre)){return;}
        ips.emplace_back(clst::CntCurve(j,&(ccs[j]._curve),isjSp));
        jps.emplace_back(clst::CntCurve(i,&(ccs[i]._curve),isiSp));
    };
    for (size_t i = 0; i < loop.Size(); i++){
        auto& curve = loop[i];
        auto& cc = ccs[i];
        //连接曲线仅针对裁减曲线
        cc.SetCurve(curve);
        const auto& sp = curve.GetSp();
        const auto& ep = curve.GetEp();
        if(curve.IsSpClip() && curve.IsEpClip()){cc.SetDouble(true);}
        for (size_t j = i + 1; j < loop.Size(); j++){
            const auto& curve1 = loop[j];
            //if(!curve1.IsClip()){continue;}
            auto& cc1 = ccs[j];
            const auto& sp1 = curve1.GetSp();
            const auto& ep1 = curve1.GetEp();
            AddCntCurve(i,j,sp,sp1,true,true,cc._spCnts,cc1._spCnts);
            AddCntCurve(i,j,sp,ep1,true,false,cc._spCnts,cc1._epCnts);
            AddCntCurve(i,j,ep,sp1,false,true,cc._epCnts,cc1._spCnts);
            AddCntCurve(i,j,ep,ep1,false,false,cc._epCnts,cc1._epCnts);
        }
    }
    bool isCW = loop.IsCW();
    //计算连接曲线角度(角度制)和当前曲线的夹角
    auto GetIncAgl = [&](clst::ClipCurve& cl_c,bool isSpClip){
        double agl = 0;
        auto& cnts = isSpClip ? cl_c._spCnts : cl_c._epCnts;
        for (size_t i = 0; i < cnts.size(); i++){
            auto& cnt = cnts[i];
            auto& id = cnt.Id();
            bool isSp = cnt.IsSp();
            const auto& cn_c = cnt.Curve();
            if(cn_c->Type() == Ofte::LINETYPE){
                auto& s = cn_c->GetSeg();
                if(isSp){agl = BaseGeom2D::SegmentAngle(s.GetSp(),s.GetEp());}
                else{agl = BaseGeom2D::SegmentAngle(s.GetEp(),s.GetSp());}
            }else if(cn_c->Type() == Ofte::ARCTYPE){
                auto& a = cn_c->GetArc();
                if(isSp){agl = BaseGeom2D::ArcPointTangAgl(a,a.GetSp());}
                else{
                    agl = BaseGeom2D::ArcPointTangAgl(a,a.GetEp());
                    agl = RevereAgl(agl);
                }
            }
            cnt.SetAgl(agl);
        }
        //计算当前裁剪曲线角度
        const auto& curve = cl_c.Curve();
        if(curve.Type() == Ofte::LINETYPE){
            const auto& s = curve.GetSeg();  //这里计算对偶边角度
            agl = BaseGeom2D::SegmentAngle(s.GetSp(),s.GetEp());
        }else{
            const auto& a = curve.GetArc();
            const auto& op = isSpClip ? a.GetSp() : a.GetEp();
            agl = BaseGeom2D::ArcPointTangAgl(a,op);
        }
        agl = isSpClip ? agl : RevereAgl(agl);
        cl_c.SetAgl(agl) ;
        //计算和curve 端点相连的曲线与curve 反向边的夹角
        for (size_t i = 0; i < cnts.size(); i++){
            double a = pnum::TwoAglIncludedAgl(agl,cnts[i].Agl(),isCW);
            cnts[i].SetIncAgl(a);
        }
        std::sort(cnts.begin(),cnts.end(),[](const clst::CntCurve& d1,
                  const clst::CntCurve& d2){return d1.IncAgl() < d2.IncAgl();});
    };
    //按照曲线对偶边最左(或右)转原则排序
    for(size_t i = 0;i < ccs.size();++i){
        auto& cl_c = ccs[i];
        double agl = 0;
        if(cl_c.SpCnts().size() > 1){
            GetIncAgl(cl_c,true);
        }
        if(cl_c.EpCnts().size() > 1){
            GetIncAgl(cl_c,false);
        }
        //std::cout<<"";
    }
#if 0
    for (size_t i = 0; i < ccs.size(); i++){
        const auto& cc = ccs[i];const auto& sps = cc.SpCnts(),eps = cc.EpCnts();
        DisplayGeom().DisplayElement(cc.Curve(),_colors[0],1,true,true);
        DisplayGeom().DisplayPoint(cc.Curve().GetSp(),_colors[4]);
        for (size_t j = 0; j < sps.size(); j++){
            DisplayGeom().DisplayElement(loop[sps[j].Id()],_colors[1],1,true,false);
        }
        for (size_t j = 0; j < eps.size(); j++){
            DisplayGeom().DisplayElement(loop[eps[j].Id()],_colors[1],1,true,false);
        }
    }
#endif
    BuildCurveToLoops(ccs);
    return _loops;
}

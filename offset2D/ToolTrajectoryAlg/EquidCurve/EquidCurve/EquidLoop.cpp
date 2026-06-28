#include "EquidLoop.h"
#include "EquidLoopAux.h"
#include "EquidCurve/EquidCurve2D.h"
#include "../../GeometryCalculate/GeomIntersect/CurveInt2D.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../GeometryCalculate/GeomQuality/CurveQual2D.h"
#include "../../GeometryCalculate/GeomCalculate/CurveDist2D.h"
#include "../../OperCurve2D/OperateOftString/OperateOftString.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
bool EquidLoop::EquidCurvePreproc(ofts::DefLoop& oloop,
                                  double dist,bool& isValid,
                                  ofts::DefLoop& rloop)
{
    bool isIterminal = false;
    if(oloop.IsEmpty()){return true;}
    //rloop.SetLoopType(oloop.LoopType());
    rloop.SetLeftward(oloop.Leftward());
    auto CircleOffset = [](const ofts::DefLoop& oloop,double dist,
            bool &isIterminal,bool &isValid,ofts:: DefLoop& rloop){
        if(!oloop.IsCircleLoop()){return;}
        isIterminal = true;
        double R = oloop[0]._circle.R();
        if(!oloop.Leftward() && ((std::abs(R - dist) <= Oft_CircleVal_Err) || (R <= dist))){return;}
        double radius;
        if(!oloop.Leftward()){radius = R - dist;}
        else{radius = R + dist;}
        rloop = oloop;
        rloop[0]._circle.SetR(radius);
        isValid = true;
    };
    CircleOffset(oloop,dist,isIterminal,isValid,rloop);
    if(isIterminal){return true;}
    OperateLoop().OptCircleArcTrans(oloop,false,true,PreErr_6,PreErr_6);
    CircleOffset(oloop,dist,isIterminal,isValid,rloop);
    return isIterminal;
}
void EquidLoop::OperateInvalidArc(std::vector<ofts::DefElem>& cs)
{
    ///DisplayGeom().DisplayLoop(DefLoop(cs),_colors[1],1,true,false);
    std::vector<size_t>ids;
    for(size_t i = 0;i < cs.size();++i){
        if(cs[i].Type() != OffsetEnum::ARCTYPE){
            continue;
        }
        auto& arc = cs[i]._arc;
        if(!arc._arcSp.IsEqual(arc._arcEp,PreErr_6)){
            continue;
        }
        double agl = BaseGeom2D::ArcCenterAngle(arc);
        if(agl > PreErr5_4 && agl < 360 - PreErr5_4){
            continue;
        }
        if(agl >= 360 - PreErr5_4 && !cs[i].IsCntCurve()){
            continue;
        }
        const auto& sp = arc._arcSp;
        const auto& ep = arc._arcEp;
        if(sp.IsEqual(ep,PreErr_12)){
            ids.emplace_back(static_cast<int>(i));
            continue;
        }
        ///auto d = sp.Distance2D(ep);
        cs[i].SetType(OffsetEnum::LINETYPE);
        cs[i].ChangeEndpt(sp,false);
        cs[i].ChangeEndpt(ep,true);
    }
    if(!ids.empty()){
        pnum::DeleteIndexElems(ids,cs);
    }
}
void EquidLoop::CalculateEquidCurve(const std::vector<ofts::DefElem>& cs,
                                    bool leftward,double dist,
                                    std::vector<ofts::DefElem>& rcs)
{
    EquidLoopAux aux;
    //bool leftward = proType != Ofte::OUTSIDEPROFILE ? true : false;
    rcs.reserve(cs.size() + static_cast<size_t>(cs.size() / 4));
    //DisplayGeom().DisplayLoop(ofts::DefLoop(cs),_colors.back(),1,true,false);
    for (int i = 0; i < cs.size(); ++i) {/**
        if(vElem.size() == 9){
            int tem = 1;
        }*/
        const auto& c = cs[i];
        //DisplayGeom().DisplayElement(c,_colors[i],1.5);
        ofts::DefElem ec;
        if(EquidCurve2D::EquidistantCurve(c,dist,leftward,ec)){
            //DisplayGeom().DisplayElement(ec,_colors[i],1.5);
            ec.SetSourceId(i);
            rcs.emplace_back(ec);
            continue;
        }
        if(c.Type() != Ofte::ARCTYPE){continue;}
        ofts::DefArc arc1,arc2;
        aux.NotExistEquidCurve(c.GetArc(),dist,arc1,arc2);
        ofts::DefElem c1(arc1),c2(arc2);
        c1.SetCntCurve(true);
        c2.SetCntCurve(true);
        //DisplayGeom().DisplayArc(arc1,_colors[1],2,true,false);
        //DisplayGeom().DisplayArc(arc2,_colors[2],2,true,false);
        c1.SetSourceId(i);
        c2.SetSourceId(i);
        rcs.emplace_back(c1);
        rcs.emplace_back(c2);
    }
    aux.ResetCoinAgjCurve(rcs);
    //DisplayGeom().DisplayLoop(DefLoop(rcs),_colors[1],1,true,false);
}
void EquidLoop::ClosedEquidCurve(const ofts::DefLoop& oloop,double dist,
                                 std::vector<ofts::DefElem>& eq_cs,
                                 std::vector<ofts::DefElem>& rcs)
{
#define view 0
    numst::IntParam ipa;
    ipa.SetEndptRele(true,PreErr5_10);
    ipa.SetIntErr(PreErr5_10);ipa.SetOnCurveErr(PreErr5_10);
    ipa.SetTanErr(PreErr5_14);ipa.SetCalcParal(true);
    EquidLoopAux aux;
    auto eq_cs1 = eq_cs;
    for(size_t i = 0;i < eq_cs.size();++i) {/**
        if(i == 5){
            int tem = 1;
        }*/
        if(oloop.IsOpen() && i >= eq_cs.size() - 1){
            if(CurveQual2D::IsValidGeomCurve(eq_cs[i],PreErr5_5,
                                             PreErr5_5,true,PreErr5_5)){
                rcs.emplace_back(eq_cs[i]);
            }
            break;
        }
        ofts::GeomIntRes iRes;
        size_t nex = pnum::NexId(eq_cs.size(),i);
        auto& c1 = eq_cs[i];
        auto& c2 = eq_cs[nex];
        const auto& sp1 = c1.GetSp();
        const auto& ep1 = c1.GetEp();
        const auto& sp2 = c2.GetSp();
        if(ep1.IsEqual(sp2,PreErr_6)){
            if(ep1.IsEqual(sp2,PreErr5_10)){
                auto mp = ep1.Midpoint(sp2);
                aux.SetCurveEndptCase(mp,sp1,eq_cs[i],eq_cs[nex],rcs);
            }else{
                aux.ChangeEndpointCase(c1,c2,ep1,sp2,rcs);
            }
            continue;
        }
        CurveInt2D::GetCurveIntersect(c1,c2,ipa,iRes);
        if(iRes.IsInt()){
            if(iRes.Num() == 1){
                aux.SetCurveEndptCase(iRes.P1(),sp1,eq_cs[i],eq_cs[nex],rcs);
                continue;
            }
            bool is1 = c1.IsEndpoint(iRes.P1(),PreErr5_10) ||
                    c2.IsEndpoint(iRes.P1(),PreErr5_10);
            bool is2 = c1.IsEndpoint(iRes.P2(),PreErr5_10) ||
                    c2.IsEndpoint(iRes.P2(),PreErr5_10);
            if(is1 && is2){
                auto op = ep1.DistSquare2D(iRes.P1()) <
                        ep1.DistSquare2D(iRes.P2()) ? iRes.P1() : iRes.P2();
                aux.SetCurveEndptCase(op,sp1,eq_cs[i],eq_cs[nex],rcs);
                continue;
            }
        }
        if(c1.Type() == Ofte::LINETYPE && c2.Type() == Ofte::LINETYPE){
            if(aux.AdjoinEndConnect(oloop,dist,ipa,c1,c2,rcs)){
                continue;
            }
        }
        if(!sp1.IsEqual(ep1,PreErr5_10)){rcs.emplace_back(c1);}
        ofts::DefArc arc;
        int id = c1.RootId();
        auto cp = oloop[id].GetEp();
        //此时c1 可能端点重合,计算arc 方向时可能结果错误,因此引入
        //eq_cs1[i]计算arc 方向.
        aux.GetConnectArc(eq_cs1[i],c1,c2,cp,dist,oloop.Leftward(),arc);
        ofts::DefElem ac(arc);
        //DisplayGeom().DisplayArc(arc,_colors[4],2);
        ac.SetCntCurve(true);
        rcs.emplace_back(ac);
    }
    if(rcs.empty() || oloop.IsOpen()){return;}
    const auto& ep = rcs.back().GetEp();
    rcs[0].ChangeEndpt(ep,false);
}
ofts::DefLoop EquidLoop::ExicuteEquidCurve(const ofts::OffsetParam& param,
                                           ofts::DefLoop& oloop,
                                           double dist,bool& isValid)
{
#define view 0
#if view
    _mainwind->myOccView->getContext()->RemoveAll(true);
    //DisplayGeom().DisplayLoop(oloop,_colors.back(),1,true,true);
    for (size_t i = 0; i < oloop.Size(); i++){
        DisplayGeom().DisplayElement(oloop[i],_colors[i],1.5);
        std::cout<<"";
    }
    auto s = OperateOftString().LoopElementString(oloop);
    std::cout<<s<<endl;
#endif
    isValid = false;
    ofts::DefLoop rloop;
    if(EquidCurvePreproc(oloop,dist,isValid,rloop)){
        return rloop;
    }
    std::vector<ofts::DefElem> eq_cs,curves;
    CalculateEquidCurve(oloop.m_vElem,oloop.Leftward(),dist,eq_cs);
#if view
    //DisplayGeom().DisplayLoop(ofts::DefLoop(eq_cs),_colors[1],1,true,false);
    /*for(auto& c : eq_cs){
        DisplayGeom().DisplayPoint(c.GetSp(),_colors[4]);
    }*/
    auto s1 = OperateOftString().LoopElementString(ofts::DefLoop(eq_cs));
    std::cout<<s1<<endl;
#endif
    ClosedEquidCurve(oloop,dist,eq_cs,curves);
#if view
    DisplayGeom().DisplayLoop(ofts::DefLoop(curves),_colors[2],1,true,false);
    for (size_t i = 0; i < curves.size(); i++){
        DisplayGeom().DisplayElement(curves[i],_colors[i],1.5,true);
        std::cout<<"";
    }
    auto s2 = OperateOftString().LoopElementString(ofts::DefLoop(curves));
    std::cout<<s2<<endl;
#endif
    OperateInvalidArc(curves);
    rloop.SetCurves(curves);
    EquidLoopAux().ResetInvalidCurves(curves,oloop.GetEndType());
    rloop.SetEndType(oloop.GetEndType());
#if view
    DisplayGeom().DisplayLoop(ofts::DefLoop(curves),_colors[3],2,true,false);
    auto s3 = OperateOftString().LoopElementString(ofts::DefLoop(curves));
    std::cout<<s3<<endl;
#endif
    isValid = true;
    rloop.SetCW(oloop.IsCW());
    return rloop;
}
ofts::DefLoop EquidLoop::GetEquidistanceCurve(const ofts::OffsetParam& param,
                                              ofts::DefLoop oloop,
                                              double dist,bool& isValid)
{
    if(oloop.IsEmpty()){return ofts::DefLoop();}
    if(oloop.GetEndType() == Ofte::EndType::Polygon){
        return ExicuteEquidCurve(param,oloop,dist,isValid);
    }
    if(oloop.GetEndType() == Ofte::EndType::Joined){
        //DisplayGeom().DisplayLoop(oloop,_colors.back(),1,true,false);
        auto loop1 = ExicuteEquidCurve(param,oloop,dist,isValid);
        if(!isValid){return loop1;}
        oloop.ReverseType();
        auto loop2 = ExicuteEquidCurve(param,oloop,dist,isValid);
        for(auto& d : loop2){d.SetMark(1);loop1.AddElem(d);}
        //DisplayGeom().DisplayLoop(loop1,_colors[1],1.5,true,false);
        return loop1;
    }
    auto loop1 = ExicuteEquidCurve(param,oloop,dist,isValid);
    if(!isValid){return ofts::DefLoop();}
    oloop.ReverseType();
    auto loop2 = ExicuteEquidCurve(param,oloop,dist,isValid);
    if(!isValid){return ofts::DefLoop();}
    EquidLoopAux().OpenLoopEndConnect(oloop,loop1,loop2,dist);
    return loop1;
}

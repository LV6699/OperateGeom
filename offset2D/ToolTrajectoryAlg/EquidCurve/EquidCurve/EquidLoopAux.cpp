#include "EquidLoopAux.h"
#include "EquidCurve/EquidCurve2D.h"
#include "../PreprocLoop/PreprocLoop.h"
#include "../../GeometryCalculate/GeomCalculate/PtOnCurve2D.h"
#include "../../GeometryCalculate/GeomIntersect/CurveInt2D.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../OperCurve2D/OperCurve2D/OperateCurve.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
bool EquidLoopAux::AdjoinEndConnect(const ofts::DefLoop& oloop,
                                    double R,const numst::IntParam& ipa,
                                    ofts::DefElem& c1,ofts::DefElem& c2,
                                    std::vector<ofts::DefElem>& rcs)
{
    auto type = oloop.GetJoinType();
    if(type == Ofte::JoinType::Round){return false;}
    const auto& oc1 = oloop[c1.RootId()];
    const auto& oc2 = oloop[c2.RootId()];
    if(BaseGeom2D::PtLocatedSegLeft(oc1.GetSp(),oc1.GetEp(),oc2.GetEp()) == oloop.Leftward()){
        return false;
    }
    ofts::GeomIntRes iRes;
    CurveInt2D::GetLineIntersect(c1.GetSeg(),c2.GetSeg(),ipa,iRes);
    if(!iRes.IsInt()){return false;}
    auto ip = iRes.P1();
    const auto& op = oc1.GetEp();
    const auto& sp = c1.GetEp();
    const auto& ep = c2.GetSp();  //这里对线段(sp,ep)方向有要求
    ofts::Point rsp,rep,ip1,ip2;
    //bool leftward = oloop.LoopType() != Ofte::OUTSIDEPROFILE;
    bool leftward = oloop.Leftward();

    auto ChCurveEp = [&](ofts::Point& p){
        c1.ChangeEndpt(p,true);
        c2.ChangeEndpt(p,false);
        if(c1.IsValidEnd(PreErr_10)){rcs.emplace_back(c1);}
    };
    auto OftInt = [&](double d){
        iRes.Reset();
        EquidCurve2D::EquidistantSeg(sp,ep,d,leftward,rsp,rep);
        CurveInt2D::GetLineIntersect(c1.GetSp(),c1.GetEp(),rsp,rep,ipa,iRes);
        if(!iRes.IsInt()){return false;}
        ip1 = iRes.P1();
        iRes.Reset();
        CurveInt2D::GetLineIntersect(c2.GetSp(),c2.GetEp(),rsp,rep,ipa,iRes);
        if(!iRes.IsInt()){return false;}
        ip2 = iRes.P1();
        if(ip1.DistSquare(ip2) > PreErr_12){
            c1.ChangeEndpt(ip1,true);
            c2.ChangeEndpt(ip2,false);
            if(c1.IsValidEnd(PreErr_10)){rcs.emplace_back(c1);}
            rcs.emplace_back(ofts::DefElem(ofts::DefSeg(ip1,ip2)));
        }else{
            auto mp = ip1.Midpoint(ip2);
            ChCurveEp(mp);
        }
        return true;
    };
    //原始曲线连接点至两偏置线段连接线段距离
    double dist = BaseGeom2D::PointToLineDist(sp,ep,op);
    if(type == Ofte::JoinType::Square){
        double oftD = R - dist;
        if(oftD < PreErr_6 || ip.IsEqual(op,R + PreErr_6)){
            ChCurveEp(ip);
        }else{
            if(!OftInt(oftD)){return false;}
        }
        return true;
    }
    if(type == Ofte::JoinType::Miter){
        double oftD = oloop.MiterLimit() - dist;
        if(oftD < PreErr_6 || ip.IsEqual(op,oloop.MiterLimit() + PreErr_6)){
            ChCurveEp(ip);
        }else{
            if(!OftInt(oftD)){return false;}
        }
        return true;
    }
    return false;
}
void EquidLoopAux::NotExistEquidCurve(const ofts::DefArc& oArc,
                                      double R,ofts::DefArc& arc1,
                                      ofts::DefArc& arc2)
{
    const auto& arcSp = oArc.GetSp();
    const auto& arcEp = oArc.GetEp();
    numst::IntParam param;
    ofts::GeomIntRes iRes,iRes1,iRes2;
    CurveInt2D::GetCircleIntersect(arcSp,R,arcEp,R,param,iRes);
    if(!iRes._isExistInt){
        double d = arcSp.Distance2D(arcEp);
        CurveInt2D::GetCircleIntersect(arcSp,d,arcEp,d,param,iRes);
    }
    ofts::Point ip;
    if(iRes.Num() == 1){
        ip = iRes.P1();
    }else{
        auto mp = BaseGeom2D::ArcMidpoint(oArc);
        ip = BaseGeom2D::GetPtBaseDist(mp,iRes.P1(),iRes.P2(),false);
    }
    //iRes1 和iRes2 一定有两个相交点.
    CurveInt2D::GetLineCircleInt(arcSp,arcEp,arcSp,R,param,iRes1);
    CurveInt2D::GetLineCircleInt(arcSp,arcEp,arcEp,R,param,iRes2);
    if(!iRes1.IsInt() || !iRes2.IsInt()){
        return;
    }
    auto prep = BaseGeom2D::GetPtBaseDist(arcEp,iRes1.P1(),iRes1.P2(),false);
    auto nexp = BaseGeom2D::GetPtBaseDist(arcSp,iRes2.P1(),iRes2.P2(),false);
    auto iscw = !oArc.IsCW();
    SetArcParameters(prep,ip,arcSp,iscw,R,arc1);
    SetArcParameters(ip,nexp,arcEp,iscw,R,arc2);
}
void EquidLoopAux::GetConnectArc(const ofts::DefElem& prec,
                                 const ofts::DefElem& c1,
                                 const ofts::DefElem& c2,
                                 const ofts::Point& cp,
                                 double R,bool leftward,
                                 ofts::DefArc& arc)
{
    bool isCW = false;
    if(prec.Type() == Ofte::LINETYPE){
        const auto& sp = prec.GetSeg().GetSp();
        const auto& ep = prec.GetSeg().GetEp();
        ofts::Point esp,eep;
        //EquidCurve2D::EquidistantSeg(sp,ep,2,proType != Ofte::OUTSIDEPROFILE,esp,eep);
        EquidCurve2D::EquidistantSeg(sp,ep,2,leftward,esp,eep);
        auto cwp = BaseGeom2D::PointRotate2D(esp,sp,90,true);
        auto ccwp = BaseGeom2D::PointRotate2D(esp,sp,90,false);
        double a = BaseGeom2D::SegmentAngle(sp,ep);
        double cw_a = BaseGeom2D::SegmentAngle(sp,cwp);
        double ccw_a = BaseGeom2D::SegmentAngle(sp,ccwp);
        isCW = std::abs(cw_a - a) < std::abs(ccw_a - a);
    }else{
        const auto& arc1 = prec.GetArc();
        auto R = arc1.GetOptR();
        double val = R / 2;
        //int sym = proType == Ofte::OUTSIDEPROFILE ? 1 : -1;
        int sym = leftward == false ? 1 : -1;
        double R1 = arc1.IsCW() ? R - val * sym : R + val * sym;
        if(R1 > R){isCW = arc1.IsCW();}
        else{isCW = !arc1.IsCW();}
    }
    SetArcParameters(c1.GetEp(),c2.GetSp(),cp,isCW,R,arc);
}
void EquidLoopAux::ResetInvalidCurves(std::vector<ofts::DefElem>& cs,Ofte::EndType etype)
{
    bool isR = false;
    double endErr = PreErr5_10,minR = PreErr5_10;
    std::vector<size_t>ids;ids.reserve(10);
    for (size_t i = 0; i < cs.size(); i++){
        //仅处理端点无效情况
        if(!CurveQual2D::IsValidGeomCurve(cs[i],endErr,endErr,isR,minR)){
            ids.emplace_back(i);
            cs[i].SetValid(false);
        }
    }
    if(ids.empty()){return;}
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& c){
        return !c.IsValid();}),cs.end());
    double err = endErr * endErr;
    double err1 = PreErr_14 * PreErr_14;
    for (size_t i = 0; i < cs.size() ; i++){
        size_t ni = pnum::NexId(cs.size(),i);
        const auto& ep = cs[i].GetEp();
        const auto& sp = cs[ni].GetSp();
        double d = ep.DistSquare2D(sp);
        if(d > err || d < err1){
            continue;
        }
        auto mp = ep.Midpoint(sp);
        cs[i].SetEp(mp);
        cs[ni].SetSp(mp);
    }
    if(ids.empty()){return;}
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& c){
        return !c.IsValid();}),cs.end());
}
void EquidLoopAux::ChangeEndpointCase(ofts::DefElem& c1,ofts::DefElem& c2,
                                      const ofts::Point& ep1,const ofts::Point& sp2,
                                      std::vector<ofts::DefElem>& cs)
{
    if(c1.Type() == OffsetEnum::LINETYPE){
        if(c2.Type() == OffsetEnum::LINETYPE){
            auto mp = ep1.Midpoint(sp2);
            SetCurveEndptCase(mp,c1.GetSp(),c1,c2,cs);
        }else{
            c1.ChangeEndpt(sp2,true);
            if(!c1.GetSp().IsEqual(c1.GetEp(),PreErr5_10)){
                cs.emplace_back(c1);
            }
        }
        return;
    }
    if(c2.Type() == OffsetEnum::LINETYPE){
        if(!c1.GetSp().IsEqual(c1.GetEp(),PreErr5_10)){
            cs.emplace_back(c1);
        }
        c2.ChangeEndpt(ep1,false);
        return;
    }
    if(!c1.GetSp().IsEqual(c1.GetEp(),PreErr5_10)){
        cs.emplace_back(c1);
    }
    c2.ChangeEndpt(ep1,false);
    PreprocLoop().ResetArcCenter(c2._arc);
}
void EquidLoopAux::ResetCoinAgjCurve(std::vector<ofts::DefElem>& curves)
{
    //处理inArc 与上一曲线存在重合的情况
    if(curves.size() < 2){return;}
    ofts::Point op;
    double lineErr = pnum::AglToSinSquare(PreErr_10); //1.7453e-12
    std::vector<size_t>ids;ids.reserve(10);
    bool isEqD = false,sp1_on = false,ep1_on = false,
            sp2_on = false,ep2_on = false;

    auto ResetCoin1 = [&](ofts::DefElem& pc,ofts::DefElem& cc,const size_t& pi){
        if(isEqD){cc.SetSp(pc.GetEp());return;}
        cc.SetSp(pc.GetSp());
        ids.emplace_back(pi);
    };
    auto ResetCoin2 = [&](ofts::DefElem& pc,ofts::DefElem& cc,const size_t& ci){
        pc.SetEp(cc.GetEp());
        ids.emplace_back(ci);
    };
    auto ResetEndpt = [&](ofts::DefElem& pc,ofts::DefElem& cc,
            const size_t& pi,const size_t& ci){
        if(sp1_on){
            if(sp2_on){  //prec 和curc 只能反向
                cc.SetSp(pc.GetSp());
                ids.emplace_back(pi);
            }else if(ep2_on){
                pc.SetEp(cc.GetEp());  //prec 和curc 只能同向
                ids.emplace_back(ci);
            }
        }else if(ep1_on){
            if(sp2_on){
                cc.SetSp(pc.GetEp());  //prec 和curc 只能同向
            }else if(ep2_on){  //prec 和curc 只能反向
                pc.SetEp(cc.GetEp());
                ids.emplace_back(ci);
            }
        }
    };
    for (size_t i = 0; i < curves.size(); ++i) {
        /*DisplayGeom().DisplayElement(curves[i],_colors[0],2,true);
        DisplayGeom().DisplayElement(curves[i+1],_colors[1],1,false);
        _mainwind->myOccView->fitAll();
        if(i == 5){
            int tem = 1;
        }*/
        auto pi = pnum::PreId(curves.size(),i);
        auto& prec = curves[pi];
        auto& curc = curves[i];
        if(prec.Type() != curc.Type()){continue;}
        if(curc.Type() == Ofte::LINETYPE){
            const auto& s1 = prec.GetSeg();
            const auto& s2 = curc.GetSeg();
            if(s1.GetEp().IsEqual(s2.GetSp(),PreErr5_10)){continue;}
            if(!CurveQual2D::IsLineAglEqual(s1,s2,lineErr)){continue;}
            sp1_on = PtOnCurve2D::IsPointOnSegment(s1.GetSp(),s2,PreErr_12,PreErr_12);
            ep1_on = PtOnCurve2D::IsPointOnSegment(s1.GetEp(),s2,PreErr_12,PreErr_12);
            if(!sp1_on || !ep1_on){continue;}

            auto v1 = s1.GetEp() - s1.GetSp();
            auto v2 = s2.GetEp() - s2.GetSp();
            isEqD = BaseGeom2D::IsEqualDire(v1,v2,PreErr_8);
            if(sp1_on && ep1_on){
                ResetCoin1(prec,curc,pi);continue;
            }
            sp2_on = PtOnCurve2D::IsPointOnSegment(s2.GetSp(),s1,PreErr_12,PreErr_12);
            ep2_on = PtOnCurve2D::IsPointOnSegment(s2.GetEp(),s1,PreErr_12,PreErr_12);
            if(!sp2_on || !ep2_on){continue;}
            if(sp2_on && ep2_on){
                ResetCoin2(prec,curc,i);continue;
            }
            ResetEndpt(prec,curc,pi,i);
            continue;
        }
        const auto& arc1 = prec.GetArc();
        const auto& arc2 = curc.GetArc();
        if(arc1.GetEp().IsEqual(arc2.GetSp(),PreErr5_10)){continue;}
        if(!arc1.GetCp().IsEqual(arc2.GetCp(),PreErr_12) ||
                !pnum::IsEqualNum(arc1.GetOptR(),arc2.GetOptR(),PreErr_12)){
            continue;
        }
        isEqD = arc1.IsCW() == arc2.IsCW();
        sp1_on = PtOnCurve2D::IsPointOnArc(arc2,arc1.GetSp(),PreErr_12,PreErr_12,false);
        ep1_on = PtOnCurve2D::IsPointOnArc(arc2,arc1.GetEp(),PreErr_12,PreErr_12,false);
        if(!sp1_on || !ep1_on){continue;}
        if(sp1_on && ep1_on){
            ResetCoin1(prec,curc,pi);continue;
        }
        sp1_on = PtOnCurve2D::IsPointOnArc(arc1,arc2.GetSp(),PreErr_12,PreErr_12,false);
        sp1_on = PtOnCurve2D::IsPointOnArc(arc1,arc2.GetEp(),PreErr_12,PreErr_12,false);
        if(!sp2_on || !ep2_on){continue;}
        if(sp2_on && ep2_on){
            ResetCoin1(prec,curc,i);continue;
        }
        ResetEndpt(prec,curc,pi,i);
    }
    pnum::DeleteIndexElems(ids,curves);
}
//loop1 与loop2 首位曲线来源需要一致
void EquidLoopAux::OpenLoopEndConnect(ofts::DefLoop& oloop,ofts::DefLoop& loop1,
                                      ofts::DefLoop& loop2,double R)
{ 
    loop2.Reverse();
    auto sc1 = loop1.First();
    auto ec1 = loop1.Last();
    auto sc2 = loop2.First();
    auto ec2 = loop2.Last();
    if(sc1.IsCntCurve()){loop1.Erase(0);}
    if(ec1.IsCntCurve()){if(!loop1.IsEmpty()){loop1.PopBack();}}
    if(sc2.IsCntCurve()){loop2.Erase(0);}
    if(ec2.IsCntCurve()){if(!loop2.IsEmpty()){loop2.PopBack();}}
#if 0
    DisplayGeom().DisplayLoop(oloop,_colors.back(),1,true);  //true false
    DisplayGeom().DisplayLoop(loop1,_colors[0],1,true);
    DisplayGeom().DisplayLoop(loop2,_colors[1],1,true);
#endif
    auto ChEnd = [](ofts::DefElem& c,ofts::Point& p,bool issp){
        if(issp){c.SetSp(p);return;}c.SetEp(p);
    };
    auto type = oloop.GetEndType();
    if(type == Ofte::EndType::Square){
        ofts::Point v1,v2,ep1,sp2,ep2,sp1;
        if(!ec1.IsCntCurve()){v1 = CurveLog2D::CurveEndVector(ec1,false,true);}
        else{v1 = CurveLog2D::CurveEndVector(sc2,true,false);}
        if(!ec2.IsCntCurve()){v2 = CurveLog2D::CurveEndVector(ec2,false,true);}
        else{v2 = CurveLog2D::CurveEndVector(sc1,true,false);}

        if(sc1.Type() == Ofte::LINETYPE){
            ep1 = BaseGeom2D::MovePoint(ec1.GetEp(),v1,R);
            sp2 = BaseGeom2D::MovePoint(sc2.GetSp(),v1,R);
            //ec1.SetEp(ep1);sc2.SetSp(sp2);
            loop1.Last().SetEp(ep1);loop2.First().SetSp(sp2);  //线段一定存在等距曲线
            loop1.AddElem(ofts::DefElem(ofts::DefSeg(ep1,sp2)));
        }else if(ec1.Type() == Ofte::ARCTYPE){
            if(!ec1.IsCntCurve()){  //存在等距曲线时直接插入
                ep1 = BaseGeom2D::MovePoint(ec1.GetEp(),v1,R);
                loop1.AddElem(ofts::DefElem(ofts::DefSeg(ec1.GetEp(),ep1)));
                if(!sc2.IsCntCurve()){
                    sp2 = BaseGeom2D::MovePoint(sc2.GetSp(),v1,R);
                    loop1.AddElem(ofts::DefElem(ofts::DefSeg(ep1,sp2)));
                    loop1.AddElem(ofts::DefElem(ofts::DefSeg(sp2,sc2.GetSp())));
                }else{
                    sp2 = BaseGeom2D::MovePoint(sc2.GetEp(),v1,R);
                    sp2 = BaseGeom2D::PointOnLineProj(ep1,sc2.GetEp(),sp2);
                    loop1.AddElem(ofts::DefElem(ofts::DefSeg(ep1,sp2)));
                    loop1.AddElem(ofts::DefElem(ofts::DefSeg(sp2,sc2.GetEp())));
                }
            }else{  //sc1 无效,插入线段起点是sp2 在线段(sp2,sc1.GetEp())投影点
                sp2 = BaseGeom2D::MovePoint(sc2.GetSp(),v1,R);
                ep1 = BaseGeom2D::MovePoint(ec1.GetSp(),v1,R);
                ep1 = BaseGeom2D::PointOnLineProj(sp2,ec1.GetSp(),ep1);
                loop1.AddElem(ofts::DefElem(ofts::DefSeg(ec1.GetSp(),ep1)));
                loop1.AddElem(ofts::DefElem(ofts::DefSeg(ep1,sp2)));
                loop1.AddElem(ofts::DefElem(ofts::DefSeg(sp2,sc2.GetSp())));
            }
        }
        if(ec2.Type() == Ofte::LINETYPE){
            ep2 = BaseGeom2D::MovePoint(ec2.GetEp(),v2,R);
            sp1 = BaseGeom2D::MovePoint(sc1.GetSp(),v2,R);
            //ec2.SetEp(ep2);sc1.SetSp(sp1);
            loop2.Last().SetEp(ep2);loop1.First().SetSp(sp1);
            loop2.AddElem(ofts::DefElem(ofts::DefSeg(ep2,sp1)));
        }else if(ec2.Type() == Ofte::ARCTYPE){
            if(!ec2.IsCntCurve()){  //存在等距曲线时直接插入
                ep2 = BaseGeom2D::MovePoint(ec2.GetEp(),v2,R);
                loop2.AddElem(ofts::DefElem(ofts::DefSeg(ec2.GetEp(),ep2)));
                if(!sc1.IsCntCurve()){
                    sp1 = BaseGeom2D::MovePoint(sc1.GetSp(),v2,R);
                    loop2.AddElem(ofts::DefElem(ofts::DefSeg(ep2,sp1)));
                    loop2.AddElem(ofts::DefElem(ofts::DefSeg(sp1,sc1.GetSp())));
                }else{
                    sp1 = BaseGeom2D::MovePoint(sc1.GetEp(),v2,R);
                    sp1 = BaseGeom2D::PointOnLineProj(ep2,sc1.GetEp(),sp1);
                    loop2.AddElem(ofts::DefElem(ofts::DefSeg(ep2,sp1)));
                    loop2.AddElem(ofts::DefElem(ofts::DefSeg(sp1,sc1.GetEp())));
                }
            }else{  //sc1 无效,插入线段起点是sp2 在线段(sp2,sc1.GetEp())投影点
                sp1 = BaseGeom2D::MovePoint(sc1.GetSp(),v2,R);
                ep2 = BaseGeom2D::MovePoint(ec2.GetSp(),v2,R);
                ep2 = BaseGeom2D::PointOnLineProj(sp1,ec2.GetSp(),ep2);
                loop2.AddElem(ofts::DefElem(ofts::DefSeg(ec2.GetSp(),ep2)));
                loop2.AddElem(ofts::DefElem(ofts::DefSeg(ep2,sp1)));
                loop2.AddElem(ofts::DefElem(ofts::DefSeg(sp1,sc1.GetSp())));
            }
        }
#if 0
        DisplayGeom().DisplayPoint(ep1,_colors[0]);
        DisplayGeom().DisplayPoint(sp2,_colors[1]);
        DisplayGeom().DisplayPoint(sp1,_colors[2]);
        DisplayGeom().DisplayPoint(ep2,_colors[3]);
        std::cout<<"";
#endif
    }
    else if(type == Ofte::EndType::Round){
        bool iscw = false;
        const auto& cp1 = oloop.Endpoint(true);
        const auto& cp2 = oloop.Endpoint(false);
        if(ec1.Type() == Ofte::LINETYPE){
            iscw = BaseGeom2D::IsTriaRotDireCW(cp1,ec1.GetSp(),ec1.GetEp());
        }else if(ec1.Type() == Ofte::ARCTYPE){
            if(ec1.IsCntCurve()){
                iscw = ec1.GetArc().IsCW();
            }else{
                if(sc2.IsCntCurve()){
                    iscw = sc2.GetArc().IsCW();
                }else{
                    iscw = ec1.GetArc().R() > sc2.GetArc().R() ?
                                ec1.GetArc().IsCW() : sc2.GetArc().IsCW();
                }
            }
        }
        const auto& sp1 = ec1.IsCntCurve() ? ec1.GetSp() : ec1.GetEp();
        const auto& ep1 = sc2.IsCntCurve() ? sc2.GetEp() : sc2.GetSp();
        ofts::DefArc arc1(sp1,ep1,cp1,R,iscw);
        //DisplayGeom().DisplayArc(arc1,_colors[0],2);
        loop1.AddElem(ofts::DefElem(arc1));

        const auto& sp2 = ec2.IsCntCurve() ? ec2.GetSp() : ec2.GetEp();
        const auto& ep2 = sc1.IsCntCurve() ? sc1.GetEp() : sc1.GetSp();
        if(ec2.Type() == Ofte::LINETYPE){
            iscw = BaseGeom2D::IsTriaRotDireCW(cp2,ec2.GetSp(),ec2.GetEp());
        }else if(ec2.Type() == Ofte::ARCTYPE){
            if(ec2.IsCntCurve()){
                iscw = ec2.GetArc().IsCW();
            }else{
                if(sc1.IsCntCurve()){
                    iscw = sc1.GetArc().IsCW();
                }else{
                    iscw = ec2.GetArc().R() > sc1.GetArc().R() ?
                                ec2.GetArc().IsCW() : sc1.GetArc().IsCW();
                }
            }
        }
        ofts::DefArc arc2(sp2,ep2,cp2,R,iscw);
        //DisplayGeom().DisplayArc(arc2,_colors[1],2);
        loop2.AddElem(ofts::DefElem(arc2));
    }
    else if(type == Ofte::EndType::Butt){
        const auto& sp1 = ec1.IsCntCurve() ? ec1.GetSp() : ec1.GetEp();
        const auto& ep1 = sc2.IsCntCurve() ? sc2.GetEp() : sc2.GetSp();
        const auto& sp2 = ec2.IsCntCurve() ? ec2.GetSp() : ec2.GetEp();
        const auto& ep2 = sc1.IsCntCurve() ? sc1.GetEp() : sc1.GetSp();
        ofts::DefElem c1(ofts::DefSeg(sp1,ep1));
        ofts::DefElem c2(ofts::DefSeg(sp2,ep2));
        c1.SetAbsValid(true);c2.SetAbsValid(true);
        loop1.AddElem(c1);loop2.AddElem(c2);
    }
#if 0
    DisplayGeom().DisplayLoop(loop1,_colors[2],1.5,false); //true false
    DisplayGeom().DisplayLoop(loop2,_colors[3],1.5,false);
#endif
    loop1.Reserve(loop1.Size() + loop2.Size());
    for(auto& d : loop2){loop1.AddElem(d);}
#if 0
    DisplayGeom().DisplayLoop(loop1,_colors[4],2.5,true);
    bool isc = loop1.IsConnect(PreErr_14);
    std::cout<<"";
#endif
}

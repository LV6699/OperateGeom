#include "OperateLoop.h"
#include "OperateCurve.h"
#include "../../GeometryCalculate/GeomIntersect/CurveInt2D.h"
#include "../../GeometryCalculate/GeomCalculate/CurveLog2D.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
void OperateLoop::MakeLoopEndToEnd(DefLoop& loop,double err)
{
    if(loop.Size() < 2 || loop.IsConnect(err)){return;}
    size_t nex = 0;
    DefLoop loop_ = loop;
    auto NextCurve = [&](const Point& p,DefElem& c)->bool{
        for (size_t i = 0; i < loop_.Size(); ++i) {
            if(p.IsSamePoint2D(loop_[i].GetSp(),err)){
                nex = i;
                c = loop_[i];
                return true;
            }else if(p.IsSamePoint2D(loop_[i].GetEp(),err)){
                nex = i;
                c = loop_[i];
                c.Reverse();
                return true;
            }
        }
        return false;
    };
    loop.Clear();
    loop.AddElem(loop_.m_vElem.front());
    loop_.m_vElem.erase(loop_.m_vElem.begin());
    DefElem curve;
    while (!loop_.IsEmpty()) {
        if(!NextCurve(loop.Endpoint(true),curve)){
            return;
        }
        loop.AddElem(curve);
        loop_.m_vElem.erase(loop_.m_vElem.begin()+nex);
    }
    bool iscw = LoopDirection().IsClockwiseLoop(loop);
    if(!iscw){
        loop.Reverse();
    }
#if 0
    bool is = loop.IsConnect(err);
    bool is1 = loop.IsEndToEnd(err);
    bool is2 = is && is1;
#endif
}
ofts::Point OperateLoop::LoopLenthPoint(const ofts::DefLoop& loop,size_t i,
                                        bool isOrder,double lenth)
{
    if(loop.IsEmpty() || i >= loop.Size() || i < 0){return ofts::Point();}
    double l = CurveLog2D::CurveLenth(loop[i]);
    if(l >= lenth){
        if(isOrder){return CurveLog2D::NearCurveEndpt(loop[i],lenth,false,true);}
        return CurveLog2D::NearCurveEndpt(loop[i],lenth,false,false);
    }
    size_t id = i;
    while (l < lenth) {
        if(isOrder){id = loop.NexId(id);}
        else{id = loop.PreId(id);}
        double len = CurveLog2D::CurveLenth(loop[id]);
        if(l + len >= lenth){
            double l_ = lenth - l;
            return CurveLog2D::NearCurveEndpt(loop[id],l_,false,isOrder);
        }
        l += len;
    }
    return ofts::Point();
}
void OperateLoop::LoopCurveEndToEnd(DefLoop &loop)
{
    if(loop.IsEmpty() || IsCircleStructLoop(loop,true,PreErr5_10,PreErr5_10)){
        return;
    }
    //DisplayGeom().DisplayLoop(DefLoop(),_colors[1],1,true,true);
    size_t len = loop.Size() - 1;
    PreprocLoop pl;
    for(size_t i = 0;i < loop.Size();++i){
        /*if(i == loop.Size() - 1){
            int tem = 1;
        }*/
        auto& c = loop[i];
        if(loop.GetEndType() != Ofte::EndType::Polygon && i == 0){
            if(c.Type() == Ofte::ARCTYPE){pl.ResetArcCenter(c._arc);}
            continue;
        }
        const auto& pep = loop.PreElem(i).GetEp();
        switch (c.Type()) {
        case Ofte::LINETYPE:{
            c._segment.SetSp(pep);
            pl.SetLineHorVerQual(c._segment._segSp,c._segment._segEp,
                                 true,Base_LineHor_Err);
            //DisplayGeom().DisplayElement(c,_colors[i],1.5);
            break;
        }
        case Ofte::ARCTYPE:{
            c._arc.SetSp(pep);
            pl.ResetArcEndpoint(i == len,c._arc);
            //DisplayGeom().DisplayElement(c,_colors[i],1.5);
            break;
        }
        default: break;
        }
    }
    //DisplayGeom().DisplayLoop(loop,_colors[1],1,true,true);
    if(!loop.IsOpen()){SetLoopEndToEnd(loop,PreErr_12);}
    //bool isc = loop.IsConnect(PreErr_13);
}
void OperateLoop::AdjacentParallSegment(DefLoop& loop, double aglErr)
{
    if(loop.Size() < 2)
        return;
    GeomCalculate gocal;
    int i = 0,n = 0;
    while (i < int(loop.Size())) {
        n = loop.NexId(i);
        if(loop[i]._type != LINETYPE || loop[n]._type != LINETYPE){
            i ++;continue;
        }
        const auto& sp1 = loop[i]._segment._segSp;
        const auto& ep1 = loop[i]._segment._segEp;
        const auto& sp2 = loop[n]._segment._segSp;
        const auto& ep2 = loop[n]._segment._segEp;
#if 0
        double slop1 = gocal.SegmentSlope(sp1,ep1);
        double slop2 = gocal.SegmentSlope(sp2,ep2);
        if(std::abs(slop1 - slop2) >= aglErr){
            i ++;continue;
        }
#else
        if(!CurveQual2D::IsLineSlopeEqual(sp1,ep1,sp2,ep2,aglErr)){
            i ++;continue;
        }
#endif
        loop[i]._segment._segEp = loop[n]._segment._segEp;
        loop.m_vElem.erase(loop.m_vElem.begin() + n);
    }
}
void OperateLoop::AdjacentParallArc(DefLoop& loop, double cpErr, double rErr)
{
    if(GeometryLogic().IsCircleStructureLoop(
                loop,PreErr5_10,PreErr5_10) || loop.Size() < 2)
        return;
    int i = 0,n = 0;
    PreprocLoop pl;
    while (i < int(loop.Size())) {
        if(loop.IsOpen() && i == loop.Size() - 1){break;}
        n = loop.NexId(i);
        if(loop[i]._type != OffsetEnum::ARCTYPE ||
                loop[n]._type != OffsetEnum::ARCTYPE){
            i ++;
            continue;
        }
        auto& parc = loop[i]._arc;
        auto& narc = loop[n]._arc;
        parc.OptInitR(PreErr5_3);
        narc.OptInitR(PreErr5_3);
        if(std::abs(parc._arcR - narc._arcR) >= rErr){
            i ++;
            continue;
        }
        bool isSame = false;
        const auto& cp1 = parc._arcCp;
        const auto& cp2 = narc._arcCp;
        isSame = cp1.IsSamePoint2D(cp2,cpErr);
        if(!isSame){
            i ++;
            continue;
        }
        const auto& sp = parc._arcSp;
        const auto& ep = narc._arcEp;
        double R = (parc._arcR + narc._arcR)/2;
        DefArc arc (sp,ep,cp1.Midpoint(cp2),R,parc._isCW);
        pl.ResetArcCenter(arc);
        loop.m_vElem[i]._arc = arc;
        loop.m_vElem.erase(loop.m_vElem.begin() + n);
    }
}
void OperateLoop::CircleStructureArcLoop(DefLoop& loop,double cpDisErr,double closeErr)
{
    if(loop.Size() < 2)
        return;
    int n = 0;
    for (size_t i = 0; i < loop.Size(); ++i) {
        n = loop.NexId(i);
        if(loop[i].Type() != ARCTYPE || loop[n].Type() != ARCTYPE){return;}

        auto& parc = loop[i]._arc;
        auto& narc = loop[n]._arc;
        if(parc.IsCW() != narc.IsCW()){return;}

        bool isSame_ = false;
        const auto& cp1 = parc._arcCp;
        const auto& cp2 = narc._arcCp;
        isSame_ = cp1.IsSamePoint2D(cp2,cpDisErr);
        if(!isSame_){return;}
    }
    size_t last = loop.Size() - 1;
    const auto& sp_ = loop[0]._arc._arcSp;
    const auto& ep_ = loop[last]._arc._arcEp;
    bool isSame = false;
    isSame = sp_.IsSamePoint2D(ep_,closeErr);
    if(!isSame)
        return;
#if 0
    const auto& sp = loop[0]._arc._arcSp;
    const auto& ep = loop[0]._arc._arcEp;
    const auto& cp = loop[0]._arc._arcCp;
    double R1 = std::sqrt(std::pow(sp._x - cp._x, 2) + pow(sp._y - cp._y, 2));
    double R2 = std::sqrt(std::pow(ep._x - cp._x, 2) + pow(ep._y - cp._y, 2));
    double R = (R1 + R2) / 2;
#else
    double cx = 0,cy = 0,rs = 0;
    for(auto& d : loop.m_vElem){
        d._arc.OptInitR(PreErr5_3);
        cx += d._arc._arcCp.X();
        cy += d._arc._arcCp.Y();
        rs += d._arc._arcR;
    }
    double R = rs / loop.Size();
    double x = cx / loop.Size(),y = cy / loop.Size();
    Point cp(x,y,sp_.Z());
#endif
    DefCircle circle(cp,R);
    loop.Clear();
    loop.AddElem(DefElem(CIRCLETYPE, circle));
}
void OperateLoop::ArcLoopToCircle(ofts::DefLoop& loop,
                                  const numst::ReLoopErr& err)
{
    for (size_t i = 0; i < loop.Size(); ++i) {
        auto& curve1 = loop[i];
        auto& curve2 = loop[loop.NexId(i)];
        if(curve1.Type() != Ofte::ARCTYPE || curve2.Type() != Ofte::ARCTYPE){
            return;
        }
        auto& arc1 = curve1._arc;
        auto& arc2 = curve2._arc;
        if(arc1.IsCW() != arc2.IsCW()){
            return;
        }
        if(!arc1._arcCp.IsEqual(arc2._arcCp,err.RelCpErr())){
            return;
        };
        arc1.OptInitR();
        arc2.OptInitR();
        if(std::abs(arc1.R() - arc2.R()) > err.RelRadErr()){
            return;
        }
    }
    const auto& sp = loop[0]._arc._arcSp;
    const auto& ep = loop.Last()._arc._arcEp;
    if(!sp.IsEqual(ep,err.RelCloseErr())){
        return;
    }
    double cx = 0,cy = 0,rs = 0;
    for(auto& d : loop){
        cx += d._arc._arcCp.X();
        cy += d._arc._arcCp.Y();
        rs += d._arc._arcR;
    }
    double x = cx / static_cast<double>(loop.Size());
    double y = cy / static_cast<double>(loop.Size());
    double R = rs / static_cast<double>(loop.Size());
    ofts::DefCircle circle(ofts::Point(x,y,sp.Z()),R);
    loop.Clear();
    loop.AddElem(ofts::DefElem(Ofte::CIRCLETYPE,circle));
}
void OperateLoop::LoopToCloseLoops(const ofts::DefLoop& inloop,
                                   double err,std::vector<ofts::DefLoop>& loops)
{
#if 0
    DisplayGeom().DisplayLoop(inloop,_colors.back(),1,true,true);
#endif
    if(inloop.Size() < 2){
        if(inloop.IsConnect(err)){loops.push_back(inloop);}
        return;
    }
    size_t nex = 0;
    DefLoop iloop = inloop;
    for(size_t i = 0;i < iloop.Size();++i){iloop[i]._id = i;}
    auto NextCurve = [&](const ofts::Point& p,size_t id,ofts::DefElem& c)->bool{
        for (size_t i = 0; i < iloop.Size(); ++i) {
            if(id == iloop[i]._id){continue;}
            if(p.IsSamePoint2D(iloop[i].GetSp(),err)){
                nex = i;
                c = iloop[i];
                return true;
            }else if(p.IsSamePoint2D(iloop[i].GetEp(),err)){
                nex = i;
                c = iloop[i];
                c.Reverse();
                return true;
            }
        }
        return false;
    };
    auto AddLoop = [](ofts::DefLoop& iloop,
            ofts::DefLoop& loop,std::vector<ofts::DefLoop>& loops){
        for(auto& c : loop){
            for (size_t i = 0; i < iloop.Size(); ++i){
                if(iloop[i]._id == c._id){
                    iloop.Erase(i);break;
                }
            }
        }
        loops.push_back(loop);
        loop.Clear();
        if(!iloop.IsEmpty()){
            loop.AddElem(iloop[0]);
            iloop.Erase(0);
        }
    };
    ofts::DefLoop loop;
    loop.AddElem(iloop[0]);
    ofts::DefElem curve;
    while (!iloop.IsEmpty()) {
        const auto& c = loop.Last();
        if(!NextCurve(c.GetEp(),c._id,curve)){
            size_t id = 0;
            bool is = false;
            for(size_t i = 0;i < loop.Size();++i){
                if(c.GetEp().IsSamePoint2D(loop[i].GetSp(),err)){
                    id = i;is = true;break;
                }
            }
            if(is){
                std::vector<size_t>ids;
                for(size_t i = 0;i < id;++i){
                    for(size_t j = 0;j < iloop.Size();++j){
                        if(loop[i]._id == iloop[j]._id){
                            iloop.Erase(j);break;
                        }
                    }
                    ids.emplace_back(i);
                }
                pnum::DeleteIndexElems(ids,loop.m_vElem);
                AddLoop(iloop,loop,loops);
            }else{
                const auto& las = loop.Last();
                for (size_t i = 0; i < iloop.Size(); ++i){
                    if(iloop[i]._id == las._id){
                        iloop.Erase(i);break;
                    }
                }
                loop.PopBack();
            }
            continue;
        }
        loop.AddElem(curve);
        if(!loop.IsEndToEnd(err)){continue;}
        ///double d = loop.Endpoint(false).Distance2D(loop.Endpoint(true));
        AddLoop(iloop,loop,loops);
#if 0
        DisplayGeom().DisplayLoops(loops,_colors[1],1.5);
#endif
    }
}
///需要保证loop 在几何上是一个整圆
void OperateLoop::CircleArcLoopTrans(bool isToArc,ofts::DefLoop& loop)
{
    if(isToArc){
        if(!loop.IsCircleLoop()){return;}
        const auto& cp = loop[0]._circle.GetCp();
        const double& R = loop[0]._circle.R();
        ofts::Point sp(cp.X(),cp.Y() + R,cp.Z());
        ofts::Point ep(cp.X(),cp.Y() - R,cp.Z());
        ofts::DefArc arc1(sp,ep,cp,R,loop.IsCW());
        ofts::DefArc arc2(ep,sp,cp,R,loop.IsCW());
        loop.Clear();
        loop.AddElem(ofts::DefElem(arc1));
        loop.AddElem(ofts::DefElem(arc2));
        loop[0]._label._isCirDisc = true;
        loop[1]._label._isCirDisc = true;
        return;
    }
    if(loop.Size() != 2 || loop[0]._type != Ofte::ARCTYPE ||
            loop[1]._type != Ofte::ARCTYPE){
        return;
    }
    auto cp = loop[0].GetArc().GetCp().Midpoint(loop[1].GetArc().GetCp());
    double R = (loop[0]._arc.R() + loop[1]._arc.R())/2;
    loop.Clear();
    loop.AddElem(ofts::DefElem(ofts::DefCircle(cp,R)));
}
void OperateLoop::OptCircleArcTrans(ofts::DefLoop& loop,
                                    bool isToArc,bool isMatchArc,
                                    double cpErr,double rErr)
{
    if(!IsCircleStructLoop(loop,isMatchArc,cpErr,rErr)){return;}
    CircleArcLoopTrans(isToArc,loop);
}
bool OperateLoop::IsCircleStructLoop(const ofts::DefLoop& loop,bool isMatchArc,
                                     double endE,double rErr){
    if(loop.IsCircleLoop()){return true;}
    if(!isMatchArc){return false;}
    if(loop.Size() != 2 || loop[0]._type != Ofte::ARCTYPE ||
            loop[1]._type != Ofte::ARCTYPE){
        return false;
    }
    const auto& a0 = loop[0].GetArc();
    const auto& a1 = loop[1].GetArc();
    if(!a0.GetCp().IsEqual(a1.GetCp(),endE)){
        return false;
    }
    if(std::abs(a0.GetOptR() - a1.GetOptR()) > rErr){
        return false;
    }
    if(!a0.GetEp().IsEqual(a1.GetSp(),endE) || !a1.GetEp().IsEqual(a0.GetSp(),endE)){
        return false;  //验证封闭性
    }
    return true;
}
void OperateLoop::DelLoopCoinCurve(ofts::DefLoop& loop,double err)
{
    std::vector<size_t>ids;ids.reserve(loop.Size());
    for (size_t i = 0; i < loop.Size(); ++i) {
        const auto& c = loop[i];
        for (size_t j = i+1; j < loop.Size(); ++j) {
            if(!CurveQual2D::IsCoincideCurve(c,loop[j],err)){
                continue;
            }
            ids.push_back(i);
        }
    }
    pnum::DeleteIndexElems(ids,loop.m_vElem);
}
void OperateLoop::SetLoopEndToEnd(DefLoop& loop,double err)
{
    if(loop.Size() < 2 || loop.IsOpen()){
        return;
    }
    if(loop.IsEndToEnd(err)){
        return;
    }
    auto& fc = loop[0];
    auto& ec = loop[loop.Size()-1];
    numst::IntParam param(PreErr5_12,PreErr5_12,PreErr5_12,PreErr5_12,
                          PreErr5_12,PreErr5_12,PreErr5_12,PreErr5_12,
                          PreErr5_10,PreErr5_10,PreErr5_10,PreErr5_10,PreErr5_10);
    ofts::GeomIntRes iRes;
    CurveInt2D::GetCurveGeomInt(fc,ec,param,iRes);
    const auto& sp = fc.GetSp();
    const auto& ep = ec.GetEp();
    PreprocLoop pl;
    if(!iRes.IsInt()){
        auto mp = sp.Midpoint(ep);
        if(fc._type == Ofte::LINETYPE){
            if(ec._type == Ofte::LINETYPE){
                fc.SetSp(mp);
                ec.SetEp(mp);
            }else if(ec._type == Ofte::ARCTYPE){
                ec.SetEp(sp);
                pl.ResetArcCenter(ec._arc);
            }
        }else if(fc._type == Ofte::ARCTYPE){
            if(ec._type == Ofte::LINETYPE){
                fc.SetSp(ep);
                pl.ResetArcCenter(fc._arc);
            }else if(ec._type == ARCTYPE){
                fc.SetSp(mp);
                ec.SetEp(mp);
                pl.ResetArcCenter(fc._arc);
                pl.ResetArcCenter(ec._arc);
            }
        }
        return;
    }
    if(iRes.Num() == 1){
        fc.SetSp(iRes.P1());
        ec.SetEp(iRes.P1());
        return;
    }
    double dis1 = 0,dis2 = 0,dis3 = 0,dis4 = 0;
    dis1 = iRes.P1().DistSquare2D(sp);
    dis2 = iRes.P1().DistSquare2D(ep);
    dis3 = iRes.P2().DistSquare2D(sp);
    dis4 = iRes.P2().DistSquare2D(ep);
    auto op = (dis1 + dis2) < (dis3 + dis4) ? iRes.P1() : iRes.P2();
    fc.SetSp(op);
    ec.SetEp(op);
}
void OperateLoop::SetLoopFirstConnect(ofts::DefLoop& loop,double err)
{
    if(loop.Size() < 3){return;}
    if(loop[0].GetEp().IsEqual(loop[1].GetSp(),err) &&
            loop[0].GetSp().IsEqual(loop.Last().GetEp(),err)){
        return;
    }
    size_t id = 0;
    for (size_t i = 0; i < loop.Size(); ++i) {
        bool isSp = false,isEp = false;
        const auto& sp = loop[i].GetSp();
        const auto& ep = loop[i].GetEp();
        //向前查找
        for (size_t j = 0; j < loop.Size(); ++j) {
            if(i == j){continue;}
            auto& c1 = loop[j];
            if(!c1.IsValid()){continue;}
            if(ep.IsEqual(c1.GetSp(),err) || ep.IsEqual(c1.GetEp(),err)){
                isEp = true;break;
            }
        }
        //向后查找
        for (size_t j = loop.Size(); j > 0; --j) {
            if(i == j-1){continue;}
            auto& c1 = loop[j-1];
            if(!c1.IsValid()){continue;}
            if(sp.IsEqual(c1.GetEp(),err) || sp.IsEqual(c1.GetSp(),err)){
                isSp = true;break;
            }
        }
        if(isSp && isEp){id = i;break;}
        else{loop[i].SetValid(false);}
    }
    if(id == 0){
        return;
    }
    auto rloop = loop;
    rloop.Clear();
    for (size_t i = id; i < loop.Size(); ++i) {
        rloop.AddElem(loop[i]);
    }
    for (size_t i = 0; i < id; ++i) {
        rloop.AddElem(loop[i]);
    }
    loop.SetCurves(rloop.Curves());
}
void OperateLoop::DelNotConnectCurves(ofts::DefLoop& loop,double err)
{
    if(loop.IsCircleLoop()){return;}
    std::vector<std::pair<bool,bool>> symbs(loop.Size(),std::make_pair(false,false));
    for (size_t i = 0; i < loop.Size(); i++){
        const auto& sp = loop[i].GetSp();
        const auto& ep = loop[i].GetEp();
        for (size_t j = i + i; j < loop.Size(); j++){
            if(!symbs[i].first){
                if(sp.IsEqual(loop[j].GetSp(),err)){
                    symbs[i].first = true;
                    symbs[j].first = true;
                }else if(sp.IsEqual(loop[j].GetEp(),err)){
                    symbs[i].first = true;
                    symbs[j].second = true;
                }
            }
            if(!symbs[i].second){
                if(ep.IsEqual(loop[j].GetSp(),err)){
                    symbs[i].second = true;
                    symbs[j].first = true;
                }else if(ep.IsEqual(loop[j].GetEp(),err)){
                    symbs[i].second = true;
                    symbs[j].second = true;
                }
            }
        }
    }
    for (size_t i = 0; i < symbs.size(); i++){
        if(!symbs[i].first || !symbs[i].second){
            loop[i].SetValid(false);
        }
    }
    auto& cs = loop.m_vElem;
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& c){
        return !c.IsValid();}),cs.end());
}
void OperateLoop::EnabelCloseLoop(const ofts::DefLoop& loop,double e)
{


}

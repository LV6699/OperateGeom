#ifndef CURVELOG2D_H
#define CURVELOG2D_H
#include "CurveDist2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class CurveLog2D
{
public:
    CurveLog2D(){}

    static double CurveLenth(const ofts::DefElem& c)
    {
        switch (c.Type()){
        case Ofte::LINETYPE : return c.GetSeg().Lenth2D();
        case Ofte::ARCTYPE : return BaseGeom2D::ArcLenth(c.GetArc());
        case Ofte::CIRCLETYPE : return c.GetCircle().Length();
        default : break;
        }
        return 0;
    }
    static double CurveLenSquare(const ofts::DefElem& c)
    {
        switch (c.Type()) {
        case Ofte::LINETYPE: return c.GetSeg().LenSquare2D();
        case Ofte::ARCTYPE:{
            double l = BaseGeom2D::ArcLenth(c.GetArc());
            return l * l;
        }
        case Ofte::CIRCLETYPE : {
            double l = c.GetCircle().Length();
            return l * l;
        }
        default:break;
        }
        return 0;
    }
    //在c 上的点至c 端点距离,线段返回长度(或长度平方),圆弧返回圆心角(角度制)
    static double ToCurvesEndDist(const ofts::DefElem& c,const ofts::Point& p,
                                  bool toSp = true,bool isSegSq = true)
    {
        double a = 0;
        switch (c.Type()) {
        case Ofte::LINETYPE:{
            if(toSp){
                return isSegSq ? p.DistSquare(c.GetSeg().GetSp()) :
                                 p.Distance(c.GetSeg().GetSp());
            }
            return isSegSq ? p.DistSquare(c.GetSeg().GetEp()) :
                             p.Distance(c.GetSeg().GetEp());
        }case Ofte::ARCTYPE:{
            double pa = BaseGeom2D::PointAngle(p,c.GetArc().GetCp());
            if(toSp){
                double sa = BaseGeom2D::PointAngle(c.GetArc().GetSp(),c.GetArc().GetCp());
                a = pnum::TwoAglIncludedAgl(sa,pa,c.GetArc().IsCW());
            }
            else{
                double ea = BaseGeom2D::PointAngle(c.GetArc().GetEp(),c.GetArc().GetCp());
                a = pnum::TwoAglIncludedAgl(pa,ea,c.GetArc().IsCW());
            }
        }
        default: break;
        }
        return a;
    }
    static void PointsSliptSeg(const std::vector<ofts::Point>& ps,
                               const ofts::Point& sp,
                               const ofts::Point& ep,
                               std::vector<ofts::DefSeg>& segs){
        if(ps.empty()){return;}
        if(ps.size() == 1){
            if(ps[0].IsSamePoint2D(sp,PreErr5_10) ||
                    ps[0].IsSamePoint2D(ep,PreErr5_10)){
                segs.push_back(ofts::DefSeg(sp,ep));
            }else {
                segs.push_back(ofts::DefSeg(sp,ps[0]));
                segs.push_back(ofts::DefSeg(ps[0],ep));
            }
            return;
        }
        std::vector<std::pair<ofts::Point,double>>datas;
        datas.reserve(ps.size());
        for(const auto& p : ps){
            datas.emplace_back(std::make_pair(p,p.DistSquare2D(sp)));
        }
        std::sort(datas.begin(),datas.end(),[](const std::pair<ofts::Point,double>& a,
                  const std::pair<ofts::Point,double>& b) {
            return a.second < b.second;});
        segs.reserve(datas.size()+1);
        if(datas[0].first.IsSamePoint2D(sp,PreErr5_10)){
            datas[0].first = sp;
        }else{
            segs.emplace_back(ofts::DefSeg(sp,datas[0].first));
        }
        for (size_t i = 0; i < datas.size()-1; ++i) {
            segs.emplace_back(ofts::DefSeg(datas[i].first,datas[i+1].first));
        }
        if(datas.back().first.IsSamePoint2D(ep,PreErr5_10)){
            segs.back()._segEp = ep;
        }else{
            segs.emplace_back(ofts::DefSeg(datas.back().first,ep));
        }
    }
    static void PointsSlipArc(const std::vector<ofts::Point>& ps,
                              const ofts::DefArc& arc,
                              std::vector<ofts::DefArc>& as){
        if(ps.empty()){return;}
        if(ps.size() == 1){
            if(ps[0].IsSamePoint2D(arc.GetSp(),PreErr5_10) ||
                    ps[0].IsSamePoint2D(arc.GetEp(),PreErr5_10)){
                as.push_back(arc);
            }else {
                as.emplace_back(ofts::DefArc(arc.GetSp(),ps[0],
                                arc.GetCp(),arc.R(),arc.IsCW()));
                as.emplace_back(ofts::DefArc(ps[0],arc.GetEp(),
                                arc.GetCp(),arc.R(),arc.IsCW()));
            }
            return;
        }
        std::vector<std::pair<ofts::Point,double>>datas;
        datas.reserve(ps.size());
        double sAgl = std::atan2(arc.GetSp().Y() - arc.GetCp().Y(),
                                 arc.GetSp().X() - arc.GetCp().X()) * _AlgRad;
        if(sAgl < 0){sAgl += 360;}
        for(const auto& p : ps){
            double a = std::atan2(p.Y() - arc.GetCp().Y(),
                                  p.X() - arc.GetCp().X()) * _AlgRad;
            if(a < 0){a += 360;}
            double agl = pnum::TwoAglIncludedAgl(sAgl,a,arc.IsCW());
            ///double agl = CurveDist2D::PointToArcEndptDist(arc,p,true);
            datas.emplace_back(std::make_pair(p,agl));
        }
        std::sort(datas.begin(),datas.end(),[](const std::pair<ofts::Point,double>& a,
                  const std::pair<ofts::Point,double>& b) {
            return a.second < b.second;});
        as.reserve(ps.size()+1);
        if(datas[0].first.IsSamePoint2D(arc.GetSp(),PreErr5_10)){
            datas[0].first = arc.GetSp();
        }else{
            as.emplace_back(ofts::DefArc(arc.GetSp(),datas[0].first,
                            arc.GetCp(),arc.R(),arc.IsCW()));
        }
        for (size_t i = 0; i < datas.size()-1; ++i) {
            as.emplace_back(ofts::DefArc(datas[i].first,datas[i+1].first,
                            arc.GetCp(),arc.R(),arc.IsCW()));
        }
        if(datas.back().first.IsSamePoint2D(arc.GetEp(),PreErr5_10)){
            as.back()._arcEp = arc.GetEp();
        }else{
            as.emplace_back(ofts::DefArc(datas.back().first,arc.GetEp(),
                                         arc.GetCp(),arc.R(),arc.IsCW()));
        }
    }
    static void PointsClipCurve(const ofts::DefElem& curve,
                                const std::vector<ofts::Point>& ps,
                                std::vector<ofts::DefElem>& cs)
    {
        switch (curve.Type()) {
        case Ofte::LINETYPE:{
            std::vector<ofts::DefSeg>segs;
            PointsSliptSeg(ps,curve._segment.GetSp(),curve._segment.GetEp(),segs);
            for(auto& s : segs){
                cs.emplace_back(ofts::DefElem(s));
            }
            break;
        }
        case Ofte::ARCTYPE:{
            std::vector<ofts::DefArc>as;
            PointsSlipArc(ps,curve.GetArc(),as);
            for(auto& a : as){
                cs.emplace_back(ofts::DefElem(a));
            }
            break;
        }
        default: break;
        }
        for(auto& d : cs){d.SetLable(curve.Lable());}
        cs[0].SetEpClip(true);
        cs.back().SetSpClip(true);
        for (size_t i = 1; i < cs.size()-1; i++){
            cs[i].SetSpClip(true);cs[i].SetEpClip(true);
        }
    }
    static ofts::Point NearSegEndpt(const ofts::DefSeg& s,bool issp,
                                    double dist,bool isLim = true)
    {
        if(isLim && s.LenSquare2D() <= dist){
            return issp ? s.GetEp() : s.GetSp();
        }
        auto dir = issp ? (s.GetEp() - s.GetSp()) : (s.GetSp() - s.GetEp());
        dir.Normalize();
        const auto& origin = issp ? s.GetSp() : s.GetEp();
        auto op = origin + dir * dist;
        return op;
    }
    static ofts::Point NearArcEndpt(const ofts::DefArc& arc,bool issp,
                                    double agl, bool isLim = true)
    {
        if(isLim){
            double a1 = BaseGeom2D::ArcCenterAngle(arc);
            if(a1 <= agl){return issp ? arc.GetEp() : arc.GetSp();}
        }
        if(issp){return BaseGeom2D::PointRotate2D(arc.GetSp(),arc.GetCp(),agl,arc.IsCW());}
        return BaseGeom2D::PointRotate2D(arc.GetEp(),arc.GetCp(),agl,!arc.IsCW());
    }
    static ofts::Point NearCurveEndpt(const ofts::DefElem& c, bool issp,
                                      bool isAgl,double step,bool isLim = true)
    {
        switch (c.Type()) {
        case Ofte::LINETYPE:
            return NearSegEndpt(c.GetSeg(),issp,step,isLim);
        case Ofte::ARCTYPE:{
            if(!isAgl){
                step = pnum::CircleLenToAgl(step,c.GetArc().GetOptR(PreErr5_3));
            }
            return NearArcEndpt(c.GetArc(),issp,step,isLim);
        }
        default:{
            return ofts::Point();
        }
        }
        return ofts::Point();
    }
    static ofts::Point CurveEndVector(const ofts::DefElem& c,bool issp,bool s_to_e)
    {
        ofts::Point v;
        switch (c.Type()) {
        case Ofte::LINETYPE:{
            const auto& s = c.GetSeg();
            v = s_to_e ? (s.GetEp() - s.GetSp()) : (s.GetSp() - s.GetEp());
            break;
        }case Ofte::ARCTYPE:{
            const auto& arc = c.GetArc();
            const auto& p = issp ? arc.GetSp() : arc.GetEp();
            bool iscw = s_to_e ? arc.IsCW() : !arc.IsCW();
            auto rp = BaseGeom2D::PointRotate2D(p,arc.GetCp(),90,iscw);
            v = rp - arc.GetCp();
            break;
        }
        default:{break;}
        }
        v.Normalize();
        return v;
    }
    //在曲线端点(issp:是否起点)位置朝固定方向(s_to_e:是否起点至终点切线方向)延伸一定距离
    static ofts::Point CurveEndExtend(const ofts::DefElem& c,double d,bool issp,bool s_to_e)
    {
        switch (c.Type()) {
        case Ofte::LINETYPE:{
            auto dir = CurveEndVector(c,issp,s_to_e);
            const auto& origin = issp ? c.GetSp() : c.GetEp();
            auto op = origin + dir * d;
            return op;
        }
        case Ofte::ARCTYPE:{
            const auto& arc = c.GetArc();
            const auto& p = issp ? arc.GetSp() : arc.GetEp();
            auto dir = CurveEndVector(c,issp,s_to_e);
            auto op = BaseGeom2D::MovePoint(p,dir,d);
            return op;
        }
        default:{break;}
        }
        return ofts::Point();
    }
};
#endif // CURVELOG2D_H

#ifndef CURVEINT2D_H
#define CURVEINT2D_H
#include"../GeomQuality/CurveQual2D.h"
#include"../../GeometryCalculate/GeomIntersect/CurveBoxInt.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
class CurveInt2D
{
public:
    CurveInt2D(){}
    static bool LineIntCondition(const ofts::Point &sp1,const ofts::Point &ep1,
                                 const ofts::Point &sp2,const ofts::Point &ep2,
                                 const numst::IntParam& param){
        if(!param.CalParalLine() && CurveQual2D::IsLineSlopeEqual(
                    sp1,ep1,sp2,ep2,param.LParalPre())){
            return false;
        }
        return true;
    }
    static bool LineIntCondition(const ofts::DefSeg& s1,const ofts::DefSeg& s2,
                                 const numst::IntParam& param)
    {
        return LineIntCondition(s1.GetSp(),s1.GetEp(),s2.GetSp(),s2.GetEp(),param);
    }
    static bool LineCircleIntCond(const ofts::Point &sp,const ofts::Point &ep,
                                  const ofts::Point& cp,double R,
                                  const numst::IntParam& param){
        if(!param.Cal_CL_Tan() && CurveQual2D::IsLineCircleTan(
                    sp,ep,cp,R,param.LCTanPre())){
            return false;
        }
        return true;
    }
    static bool LineCircleIntCond(const ofts::DefSeg& seg,
                                  const ofts::Point& cp,double R,
                                  const numst::IntParam& param){
        return LineCircleIntCond(seg.GetSp(),seg.GetEp(),cp,R,param);
    }
    static bool CircleIntCond(const ofts::Point& cp1,double R1,
                              const ofts::Point& cp2,double R2,
                              const numst::IntParam& param){
        if(!param.Cal_CC_Tan()){
            if(CurveQual2D::IsCircleTangency(cp1,R1,cp2,R2,param.CCTanPre())){return false;}
        }
        if(!param.CalParalCir() && cp1.IsEqual(cp2,param.CParalPre())){
            return false;
        }
        return true;
    }
    static bool IsLineGeomInt(const ofts::Point &sp1,const ofts::Point &ep1,
                              const ofts::Point &sp2,const ofts::Point &ep2,
                              const numst::IntParam& param)
    {
        double lparam1[3],lparam2[3];
        BaseGeom2D::GetLineParam(sp1,ep1,param.LinePXPre(),
                                 param.LinePYPre(),lparam1);
        BaseGeom2D::GetLineParam(sp2,ep2,param.LinePXPre(),
                                 param.LinePYPre(),lparam2);
        double A1 = lparam1[0],B1 = lparam1[1];
        double A2 = lparam2[0],B2 = lparam2[1];
        double disc = A1*B2 - A2*B1;
        if(std::abs(disc) < PreErr5_16){
            return false;
        }
        return true;
    }
    static bool IsLineGeomInt(const ofts::DefSeg& s1,const ofts::DefSeg& s2,
                              const numst::IntParam& param)
    {
        return IsLineGeomInt(s1.GetSp(),s1.GetEp(),s2.GetSp(),s2.GetEp(),param);
    }
    static bool IsLineIntersect(const ofts::Point &sp1,const ofts::Point &ep1,
                                const ofts::Point &sp2,const ofts::Point &ep2,
                                const numst::IntParam& param,
                                bool isJudge = true)
    {
        if(isJudge && !LineIntCondition(sp1,ep1,sp2,ep2,param)){
            return false;
        }
        return IsLineGeomInt(sp1,ep1,sp2,ep2,param);
    }
    static bool IsLineIntersect(const ofts::DefSeg& s1,const ofts::DefSeg& s2,
                                const numst::IntParam& param, bool isJudge = true)
    {
        return IsLineIntersect(s1.GetSp(),s1.GetEp(),s2.GetSp(),s2.GetEp(),param,isJudge);
    }
    static bool IsLineCircleGeomInt(const ofts::Point& sp,const ofts::Point& ep,
                                    const ofts::Point& cp,double R,
                                    const numst::IntParam& param)
    {
        double limitVal = R + param.LCIntPre(),d = BaseGeom2D::PointToLineDist(sp,ep,cp);
        if(limitVal < d){return false;}
        return true;
    }
    static bool IsLineCircleGeomInt(const ofts::DefSeg& s,
                                    const ofts::DefCircle& c,
                                    const numst::IntParam& param)
    {
        return IsLineCircleGeomInt(s.GetSp(),s.GetEp(),c.GetCp(),c.R(),param);
    }
    static bool IsLineCircleInt(const ofts::Point& sp,const ofts::Point& ep,
                                const ofts::Point& cp,double R,
                                const numst::IntParam& param,
                                bool isJudge = true)
    {
        if(isJudge && !LineCircleIntCond(sp,ep,cp,R,param)){return false;}
        return IsLineCircleGeomInt(sp,ep,cp,R,param);
    }
    static bool IsLineCircleInt(const ofts::DefSeg& s,
                                const ofts::DefCircle& c,
                                const numst::IntParam& param,
                                bool isJudge = true)
    {
        return IsLineCircleInt(s.GetSp(),s.GetEp(),c.GetCp(),c.R(),param,isJudge);
    }
    static bool IsCircleGeomInt(const ofts::Point &cp1,double R1,
                                const ofts::Point &cp2,double R2,
                                const numst::IntParam& param)
    {
        double maxR = 0,minR = 0;
        if(R1 > R2){maxR = R1;minR = R2;}
        else{maxR = R2;minR = R1;}
        double dist = cp1.Distance(cp2);
        if(dist > (R1 + R2 + param.CCIntPre())){return false;}
        if(maxR > (minR + dist + param.CCIntPre())){return false;}
        if(!param.Cal_CC_Tan() && std::abs(dist - R1 - R2) <= param.CCTanPre()){
            return false;
        }
        if(cp1.IsEqual(cp2,param.CCIntPre()) && std::abs(R1 - R2) < param.CCIntPre()){
            return false;
        }
        return true;
    }
    static bool IsCircleIntersect(const ofts::Point &cp1,double R1,
                                  const ofts::Point &cp2,double R2,
                                  const numst::IntParam& param,
                                  bool isJudge = true)
    {
        if(isJudge && !CircleIntCond(cp1,R1,cp2,R2,param)){
            return false;
        }
        return IsCircleGeomInt(cp1,R1,cp2,R2,param);
    }
    static void GetLineParamInt(double A1,double B1,double C1,
                                double A2,double B2,double C2,
                                const numst::IntParam& param,
                                ofts::GeomIntRes& iRes,
                                bool isJudge = true)
    {
        double disc = A1*B2 - A2*B1;
        if(std::abs(disc) < PreErr5_16 || (isJudge && !param.CalParalLine() &&
                                           std::abs(disc) < param.LLIntPre())){
            return;
        }
        double x = (B1*C2 - B2*C1) / (disc);
        double y = (C1*A2 - C2*A1) / (disc);
        iRes.SetIntXY(x,y);
        iRes.SetNum(1);
        iRes.SetInt();
        if(std::abs(disc) >= param.LLIntPre()){
            iRes.SetGeoInt();
        }
    }
    static void GetLineIntersect(const ofts::Point &sp1,const ofts::Point &ep1,
                                 const ofts::Point &sp2,const ofts::Point &ep2,
                                 const numst::IntParam& param,
                                 ofts::GeomIntRes& iRes,bool isJudge = true)
    {
#if 0
        double lparam1[3],lparam2[3];
        BaseGeom2D::GetLineParam(sp1,ep1,param.LinePXPre(),
                                 param.LinePYPre(),lparam1);
        BaseGeom2D::GetLineParam(sp2,ep2,param.LinePXPre(),
                                 param.LinePYPre(),lparam2);
        double A1 = lparam1[0],B1 = lparam1[1],C1 = lparam1[2];
        double A2 = lparam2[0],B2 = lparam2[1],C2 = lparam2[2];
        GetLineParamInt(A1,B1,C1,A2,B2,C2,param,iRes,isJudge);
#else
        auto v1 = ep1 - sp1;
        auto v2 = ep2 - sp2;
        double rxs = v1.Cross2D(v2);
        if(std::abs(rxs) < PreErr5_16 || (isJudge && !param.CalParalLine() &&
                                          std::abs(rxs) < param.LLIntPre())){
            return;
        }
        ofts::Point p(sp2.X() - sp1.X(),sp2.Y() - sp1.Y());
        double t = p.Cross2D(v2)/rxs;
        double ix = sp1.X() + t * v1.X();
        double iy = sp1.Y() + t * v1.Y();
        iRes.SetIntXY(ix,iy);
        iRes.SetNum(1);
        iRes.SetInt();
        if(std::abs(rxs) >= param.LLIntPre()){iRes.SetGeoInt();}
#endif
    }
    static void GetLineIntersect(const ofts::DefSeg &s1,const ofts::DefSeg &s2,
                                 const numst::IntParam& param,
                                 ofts::GeomIntRes& iRes,bool isJudge = true)
    {
        GetLineIntersect(s1.GetSp(),s1.GetEp(),s2.GetSp(),s2.GetEp(),param,iRes,isJudge);
    }
    static void GetLineCircleInt(const ofts::Point& sp,const ofts::Point& ep,
                                 const ofts::Point& cp,double R,
                                 const numst::IntParam& param,
                                 ofts::GeomIntRes& iRes,bool isJudge = true)
    {
        double lparam[3];
        BaseGeom2D::GetLineParam(sp,ep,param.LinePXPre(),
                                 param.LinePYPre(),lparam);
        double A = lparam[0],B = lparam[1];
        if(std::abs(A) < PreErr5_16 && std::abs(B) < PreErr5_16){return;}
        if(!IsLineCircleInt(sp,ep,cp,R,param,isJudge)){
            return;
        }
        iRes.SetNum(2);
        double ix1,iy1,ix2,iy2;
#if 0
        double h = cp._x;
        double k = cp._y;
        double a = A * A + B * B;
        double b = 2 * A * C + 2 * A * B * k -
                2 * B * B * h;
        double c = C * C + 2 * B * C * k +
                B * B * h * h + B * B * k * k -
                B * B * R * R;
        double delta = b * b - 4 * a * c;
        if(delta < 0){delta = 0;}
        if(delta > 0){
            ix1 = (-b + std::sqrt(delta)) / (2 * a);
            ix2 = (-b - std::sqrt(delta)) / (2 * a);
            iy1 = (-A * ix1 - C) / B;
            iy2 = (-A * ix2 - C) / B;
        }else{
            ix1 = ix2 = -b / (2 * a);
            iy1 = iy2 = (-A * ix2 - C) / B;
        }
#elif 1
        ofts::Point vec = ep - sp;
        vec.SetZ(0);
        ofts::Point prop = sp + vec * ((cp - sp) * vec)/ (vec.LengthSquare2D());
        double tem = R * R - (prop - cp).LengthSquare2D();
        if(tem < 0){tem = 0;iRes.SetNum(1);}
        double base = std::sqrt(tem);
        ofts::Point univ = vec / vec.Length2D();
        auto p1 = prop + univ * base;
        auto p2 = prop - univ * base;
        ix1 = p1.X();iy1 = p1.Y();
        ix2 = p2.X();iy2 = p2.Y();
#else
        ofts::Point Q;
        gocal.PointOnLineProjection(cp,sp,ep,Q);
        double toLDis;
        gocal.PointToLineDistance(sp,ep,cp,toLDis);
        double slop = std::atan2(ep.Y() - sp.Y(),ep.X() - sp.X());
        double base = (R * R) - (toLDis * toLDis);
        if(base < 0){base = 0;}
        base = std::sqrt(base);
        ix1 = Q.X() - base * std::cos(slop);
        iy1 = Q.Y() - base * std::sin(slop);
        ix2 = Q.X() + base * std::cos(slop);
        iy2 = Q.Y() + base * std::sin(slop);
#endif
        iRes.SetInt1XY(ix1,iy1);
        iRes.SetInt2XY(ix2,iy2);
        double dist = BaseGeom2D::PointToLineDist(sp,ep,cp);
        if(iRes.Num() == 1 || std::abs(dist - R) <= param.LCTanPre()){
            iRes.SetNum(1);
            double x = (ix1 + ix2)/2;
            double y = (iy1 + iy2)/2;
            iRes.SetIntXY(x,y);
        }
        iRes.SetExistAll();
#if 0
        double dis1,dis2;
        gocal.TwoPointDistance(cp,iRes._intPnt1,dis1);
        gocal.TwoPointDistance(cp,iRes._intPnt2,dis2);
        double pre1 = std::abs(dis1 - R);
        double pre2 = std::abs(dis2 - R);
        double sigDis1,sigDis2;
        gocal.PointToLineDistance(sp,ep,iRes._intPnt1,sigDis1);
        gocal.PointToLineDistance(sp,ep,iRes._intPnt2,sigDis2);
        double err = pre1 + pre2 + sigDis1 + sigDis2;
#endif
    }

    static void GetLineCircleInt(const ofts::DefSeg& s,
                                 const ofts::DefCircle& c,
                                 const numst::IntParam& param,
                                 ofts::GeomIntRes& iRes,bool isJudge = true)
    {
        GetLineCircleInt(s.GetSp(),s.GetEp(),c.GetCp(),c.R(),param,iRes,isJudge);
    }
    static void CircleExistInt(const ofts::Point& c1,double r1,
                               const ofts::Point& c2,double r2,
                               ofts::Point& p1,ofts::Point& p2)
    {
        double x1 = c1.X(),x2 = c2.X(),y1 = c1.Y(),y2 = c2.Y();
        if(r1 > r2){std::swap(x1,x2);std::swap(y1,y2);std::swap(r1,r2);}
        else if(x1 > x2){std::swap(x1,x2);std::swap(y1,y2);std::swap(r1,r2);}
        else if(y1 > y2){std::swap(x1,x2);std::swap(y1,y2);std::swap(r1,r2);}
#if 1
        double d = std::sqrt(std::pow(x1 - x2,2) + std::pow(y1 - y2,2));
        double l = ((r1*r1) - (r2*r2) + (d*d)) / (2*d);
        double temp = (r1 * r1) - (l * l);
        if(temp < 0){temp = 0;}
        double h = std::sqrt(temp);
        double t1 = l/d,t2 = h/d;
        double ix1 = t1*(x2 - x1) + t2 * (y2-y1) + x1;
        double iy1 = t1*(y2 - y1) - t2 * (x2-x1) + y1;
        double ix2 = t1*(x2 - x1) - t2 * (y2-y1) + x1;
        double iy2 = t1*(y2 - y1) + t2 * (x2-x1) + y1;
        p1 = {ix1,iy1};
        p2 = {ix2,iy2};
#elif 0
        double R = c1.Distance2D(c2);
        double RR = R*R,rr1 = r1*r1,rr2 = r2*r2;
        double t1 = (rr1-rr2)/(2*RR);
        double t2 = 2*(rr1+rr2)/RR;
        double t3 = (rr1-rr2)*(rr1-rr2)/(RR*RR);
        double t4 = t2-t3-1;
        if(t4 < 0){
            t4 = 0;
        }
        double ix1 = (x1+x2)/2 + t1*(x2-x1) + (std::sqrt(t4)/2)*(y2-y1);
        double iy1 = (y1+y2)/2 + t1*(y2-y1) + (std::sqrt(t4)/2)*(x1-x2);
        double ix2 = (x1+x2)/2 + t1*(x2-x1) - (std::sqrt(t4)/2)*(y2-y1);
        double iy2 = (y1+y2)/2 + t1*(y2-y1) - (std::sqrt(t4)/2)*(x1-x2);
        p1 = {ix1,iy1};
        p2 = {ix2,iy2};
#else
        double dx = x2 - x1,dy = y2 - y1;
        double d_sq = dx*dx + dy*dy;
        double d = std::sqrt(d_sq);
        double a = (r1*r1 - r2*r2 + d_sq) / (2.0 * d);
        double h_sq = r1*r1 - a*a; /// 计算两交点连线半长的平方
        if (h_sq < 0) {h_sq = 0;}
        double h = std::sqrt(h_sq);
        auto u = ofts::Point(dx,dy)/d; /// 单位向量u:从c1 指向c2
        auto v = ofts::Point(-u.Y(),u.X()); /// 垂直单位向量v(逆时针旋转90 度)
        auto M = c1 + u * a; /// 计算中点 M
        p1 = M + v * h;
        p2 = M - v * h;
#endif
    }
    static void CircleExistInt(const ofts::DefCircle& c1,
                               const ofts::DefCircle& c2,
                               ofts::Point& p1,ofts::Point& p2)
    {
        CircleExistInt(c1.GetCp(),c1.R(),
                       c2.GetCp(),c2.R(),p1,p2);
    }
    static void GetCircleIntersect(const ofts::Point &cp1,double R1,
                                   const ofts::Point &cp2,double R2,
                                   const numst::IntParam& param,
                                   ofts::GeomIntRes &iRes,bool isJudge = true)
    {
        if(!IsCircleIntersect(cp1,R1,cp2,R2,param,isJudge)){return;}
        double cpDis = cp1.Distance2D(cp2);
        iRes.SetNum(2);
        double maxR = 0,minR = 0;
        if(R1 > R2){maxR = R1;minR = R2;}
        else{maxR = R2;minR = R1;}
        double dis1 = std::abs(cpDis - R1 - R2);
        double dis2 = std::abs(maxR - minR - cpDis);
        if(dis1 <= param.CCTanPre() || dis2 <= param.CCTanPre()){
            iRes.SetNum(1);
        }
        CircleExistInt(cp1,R1,cp2,R2,iRes._intPnt1,iRes._intPnt2);
        if(iRes.Num() == 1){
            double x = (iRes.P1().X() + iRes.P2().X()) / 2;
            double y = (iRes.P1().Y() + iRes.P2().Y()) / 2;
            iRes.SetIntXY(x,y);
        }
        iRes.SetExistAll();
    }
    static void GetCircleIntersect(const ofts::DefCircle &c1,
                                   const ofts::DefCircle &c2,
                                   const numst::IntParam& param,
                                   ofts::GeomIntRes &iRes,bool isJudge = true)
    {
        GetCircleIntersect(c1.GetCp(),c1.R(),c2.GetCp(),c2.R(),param,iRes,isJudge);
    }
    static void EndptOnTupleCase(bool isSpOn,bool isEpOn,
                                 const ofts::Point &sp,const ofts::Point &ep,
                                 bool& isEndpntOn,ofts::Point& onEndpnt,
                                 ofts::GeomIntRes &iRes)
    {
        if(isSpOn && isEpOn){
            iRes._intPnt1 = sp;
            iRes._intPnt2 = ep;
            iRes._intNum = 2;
            iRes.SetExistAll();
            return;
        }
        if(isSpOn){
            isEndpntOn = true;
            onEndpnt = sp;
        }
        if(isEpOn){
            isEndpntOn = true;
            onEndpnt = ep;
        }
    }
    static void HandleEndptOnTuple(bool isEndptOn,
                                   const ofts::Point &endpt,
                                   const ofts::GeomIntRes& iRes2,
                                   ofts::GeomIntRes& iRes)
    {
        if(!isEndptOn){return;}
        if(iRes._intNum < 2){
            iRes.SetIntPt(endpt);
            iRes._intNum = 1;
        }else{
            iRes._intPnt1 = endpt;
            if(iRes2.P1().DistSquare2D(endpt) <
                    iRes2.P2().DistSquare2D(endpt)){
                iRes.SetIntPt(iRes2.P2());
            }else{
                iRes.SetIntPt(iRes2.P1());
            }
        }
    }
    static void GetLineArcInt(const ofts::Point& sp,
                              const ofts::Point& ep,
                              const ofts::DefArc& arc,
                              const numst::IntParam& param,
                              ofts::GeomIntRes& iRes)
    {
        if(!LineCircleIntCond(sp,ep,arc.GetCp(),arc.R(),param)){
            return;
        }
        ofts::Point onEndpt;
        bool isSpOn = false,isEpOn = false,isEndptOn = false;
        isSpOn =  PtOnCurve2D::IsPointOnLine(sp,ep,arc.GetSp(),param.EndptPre());
        isEpOn =  PtOnCurve2D::IsPointOnLine(sp,ep,arc.GetEp(),param.EndptPre());
        EndptOnTupleCase(isSpOn,isEpOn,arc.GetSp(),arc.GetEp(),isEndptOn,onEndpt,iRes);
        if(isSpOn && isEpOn){return;}
        ofts::GeomIntRes iRes1;
        GetLineCircleInt(sp,ep,arc.GetCp(),arc.R(),param,iRes1,false);
        if(!iRes1.IsInt()){return;}
        bool isP1OnArc = false,isP2OnArc = false;
        isP1OnArc =  PtOnCurve2D::IsPointOnArc(arc,iRes1.P1(),param.EndptPre(),
                                               param.LCIntPre(),false);
        if(iRes1.Num() == 2){
            isP2OnArc = PtOnCurve2D::IsPointOnArc(arc,iRes1.P2(),param.EndptPre(),
                                                  param.LCIntPre(),false);
        }
        if(!isP1OnArc && !isP2OnArc){return;}
        else if(isP1OnArc && !isP2OnArc){
            iRes.SetIntPt(iRes1.P1());
            iRes._intNum = 1;
        }else if(!isP1OnArc && isP2OnArc){
            iRes.SetIntPt(iRes1.P2());
            iRes._intNum = 1;
        }else{
            iRes._intPnt1 = iRes1._intPnt1;
            iRes._intPnt2 = iRes1._intPnt2;
            iRes._intNum = 2;
        }
        iRes.SetExistAll();
    }
    static void GetLineArcInt(const ofts::DefSeg& seg,
                              const ofts::DefArc& arc,
                              const numst::IntParam& param,
                              ofts::GeomIntRes& iRes)
    {
        GetLineArcInt(seg.GetSp(),seg.GetEp(),arc,param,iRes);
    }
    static void GetSegmentIntersect(const ofts::Point& sp1,const ofts::Point& ep1,
                                    const ofts::Point& sp2,const ofts::Point& ep2,
                                    const numst::IntParam& param,
                                    ofts::GeomIntRes& iRes)
    {
        if(!LineIntCondition(sp1,ep1,sp2,ep2,param)){return;}
        if(!CurveBoxInt::IsRectExistInt(sp1,ep1,sp2,ep2,param.LLIntPre() *ErrTimes_10)){return;}
        std::vector<ofts::Point>ps;ps.reserve(4);
        if(PtOnCurve2D::IsPointOnSegment(sp2,ep2,sp1,param.EndptPre(),param.LLIntPre(),true)){
            ps.emplace_back(sp1);
        }
        if(PtOnCurve2D::IsPointOnSegment(sp2,ep2,ep1,param.EndptPre(),param.LLIntPre(),true)){
            ps.emplace_back(ep1);
        }
        if(PtOnCurve2D::IsPointOnSegment(sp1,ep1,sp2,param.EndptPre(),param.LLIntPre(),true)){
            ps.emplace_back(sp2);
        }
        if(PtOnCurve2D::IsPointOnSegment(sp1,ep1,ep2,param.EndptPre(),param.LLIntPre(),true)){
            ps.emplace_back(ep2);
        }
        if(ps.size() > 1){BaseGeom2D::DeduplicatePts(ps,param.EndptPre());}
        if(ps.empty()){
            GetLineIntersect(sp1,ep1,sp2,ep2,param,iRes,false);
            if(iRes.IsInt()){
                bool isOnL1 = false,isOnL2 = false;
                isOnL1 = PtOnCurve2D::IsPointOnSegment(
                            sp1,ep1,iRes.P1(),param.EndptPre(),
                            param.OnSegPre(),false);
                isOnL2 = PtOnCurve2D::IsPointOnSegment(
                            sp2,ep2,iRes.P1(),param.EndptPre(),
                            param.OnSegPre(),false);
                if(!isOnL1 || !isOnL2){
                    iRes.Reset();
                }
            }
            return;
        }
        int intNum = ps.size();
        if(intNum == 2){
            iRes.SetIntPts(ps[0],ps[1]);
        }else{
            iRes.SetIntPt(ps[0]);
        }
        iRes._isExistInt = true;
        iRes._intNum = intNum > 2 ? 2 : intNum;
        iRes._isExiGeoInt = !CurveQual2D::IsLineSlopeEqual(sp1,ep1,sp2,ep2,param.LParalPre());
    }
    static void GetSegmentIntersect(const ofts::DefSeg& seg1,
                                    const ofts::DefSeg& seg2,
                                    const numst::IntParam& param,
                                    ofts::GeomIntRes& iRes)
    {
        GetSegmentIntersect(seg1.GetSp(),seg1.GetEp(),seg2.GetSp(),seg2.GetEp(),param,iRes);
    }
    static void GetSegmentCircleInt(const ofts::Point& sp,const ofts::Point& ep,
                                    const ofts::Point& cp,double R,
                                    const numst::IntParam& param,
                                    ofts::GeomIntRes& iRes)
    {
        if(!LineCircleIntCond(sp,ep,cp,R,param)){return;}
        bool isSpOn = false,isEpOn = false;
        isSpOn = PtOnCurve2D::IsPtOnCircle(sp,cp,R,param.EndptPre());
        isEpOn = PtOnCurve2D::IsPtOnCircle(ep,cp,R,param.EndptPre());
        ofts::Point onEndpt;
        bool isEndptOn = false;
        EndptOnTupleCase(isSpOn,isEpOn,sp,ep,isEndptOn,onEndpt,iRes);
        if(isSpOn && isEpOn){return;}
        ofts::GeomIntRes iRes2;
        GetLineCircleInt(sp,ep,cp,R,param,iRes2,false);
        if(!iRes2.IsInt() && !isEndptOn){return;}
        if(!CurveBoxInt().IsIntResInsideBox(
                    sp,ep,iRes2,param.LCIntPre()*ErrTimes_10)){
            return;
        }
        bool isP1On = false,isP2On = false;
        isP1On = PtOnCurve2D::IsPointOnSegment(sp,ep,iRes2.P1(),
                                               param.EndptPre(),
                                               param.OnSegPre(),false);
        if(iRes2.Num() == 2){
            isP2On = PtOnCurve2D::IsPointOnSegment(sp,ep,iRes2.P1(),
                                                   param.EndptPre(),
                                                   param.OnSegPre(),false);
        }
        if(!isP1On && !isP2On && !isEndptOn){return;}
        if(isP1On && !isP2On){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P1());
        }else if(!isP1On && isP2On){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P2());
        }else{
            iRes._intNum = 2;
            iRes.SetIntPts(iRes2.P1(),iRes2.P2());
        }
        HandleEndptOnTuple(isEndptOn,onEndpt,iRes2,iRes);
        iRes.SetExistAll();
    }
    static void GetSegmentCircleInt(const ofts::DefSeg& seg,
                                    const ofts::DefCircle& cir,
                                    const numst::IntParam& param,
                                    ofts::GeomIntRes& iRes)
    {
        GetSegmentCircleInt(seg.GetSp(),seg.GetEp(),cir.GetCp(),cir.R(),param,iRes);
    }
    static void GetArcCircleInt(const ofts::DefArc& arc,
                                const ofts::Point& cp,double R,
                                const numst::IntParam& param,
                                ofts::GeomIntRes& iRes)
    {
        if(!CircleIntCond(arc.GetCp(),arc.R(),cp,R,param)){return;}
        bool isSpOn = false,isEpOn = false;
        isSpOn = PtOnCurve2D::IsPtOnCircle(arc.GetSp(),cp,R,param.EndptPre());
        isEpOn = PtOnCurve2D::IsPtOnCircle(arc.GetEp(),cp,R,param.EndptPre());
        ofts::Point onEndpt;
        bool isEndptOn = false;
        EndptOnTupleCase(isSpOn,isEpOn,arc.GetSp(),arc.GetEp(),isEndptOn,onEndpt,iRes);
        if(isSpOn && isEpOn){return;}
        ofts::GeomIntRes iRes2;
        GetCircleIntersect(arc.GetCp(),arc.R(),cp,R,param,iRes2,false);
        if(!iRes2.IsInt() && !isEndptOn){return;}
        bool isP1OnArc = false,isP2OnArc = false;
        isP1OnArc = PtOnCurve2D::IsPointOnArc(arc,iRes2.P1(),param.EndptPre(),
                                              param.OnArcPre(),false);
        if(iRes2.Num() == 2){
            isP2OnArc = PtOnCurve2D::IsPointOnArc(arc,iRes2._intPnt2,param.EndptPre(),
                                                  param.OnArcPre(),false);
        }
        if(!isP1OnArc && !isP2OnArc && !isEndptOn){return;}
        if(isP1OnArc && isP2OnArc){
            iRes._intNum = 2;
            iRes._intPnt1 = iRes2.P1();
            iRes._intPnt2 = iRes2.P2();
        }else if(isP1OnArc && !isP2OnArc){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P1());
        }else if(!isP1OnArc && isP2OnArc){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P2());
        }
        HandleEndptOnTuple(isEndptOn,onEndpt,iRes2,iRes);
        iRes.SetExistAll();
    }
    static void GetArcCircleInt(const ofts::DefArc& arc,
                                const ofts::DefCircle& cir,
                                const numst::IntParam& param,
                                ofts::GeomIntRes& iRes)
    {
        GetArcCircleInt(arc,cir.GetCp(),cir.R(),param,iRes);
    }
    static void GetSegmentArcInt(const ofts::Point& sp,
                                 const ofts::Point& ep,
                                 const ofts::DefArc& arc,
                                 const numst::IntParam& param,
                                 ofts::GeomIntRes& iRes)
    {
        if(!LineCircleIntCond(sp,ep,arc.GetCp(),arc.R(),param)){return;}
        if(!CurveBoxInt().IsLineCirceBoxInt(sp,ep,arc,param._endptPre * ErrTimes_10)){
            return;
        }
        ofts::GeomIntRes iRes1;
        SegArcIntEndpt(sp,ep,arc,param,iRes1);
        if(iRes1.IsInt() && iRes1.Num() == 2){
            iRes._intNum = 2;
            iRes.SetExistAll();
            iRes.SetIntPts(iRes1.P1(),iRes1.P2());
            return;
        }
        ofts::GeomIntRes iRes2;
        GetLineCircleInt(sp,ep,arc.GetCp(),arc.R(),param,iRes2,false);
        if(!iRes2.IsInt() && !iRes1.IsInt()){return;}
        if(!iRes1.IsInt() && iRes2.IsInt() &&
                !CurveBoxInt().IsIntResInsideBox(sp,ep,iRes2,param.LCIntPre() * ErrTimes_10)){
            return;
        }
        bool isP1Val = false,isP2Val = false;
        if(PtOnCurve2D::IsPointOnSegment(sp,ep,iRes2.P1(),param.EndptPre(),
                                         param.OnSegPre(),false)){
            if(PtOnCurve2D::IsPointOnArc(arc,iRes2.P1(),param.EndptPre(),
                                         param.OnArcPre(),false)){
                isP1Val = true;
            }
        }
        if(iRes2.Num() == 2){
            if(PtOnCurve2D::IsPointOnSegment(sp,ep,iRes2.P2(),param.EndptPre(),
                                             param.OnSegPre(),false)){
                if(PtOnCurve2D::IsPointOnArc(arc,iRes2.P2(),param.EndptPre(),
                                             param.OnArcPre(),false)){
                    isP2Val = true;
                }
            }
        }
        if(!isP1Val && !isP2Val && !iRes1.IsInt()){return;}
        if(isP1Val && !isP2Val){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P1());
        }else if(!isP1Val && isP2Val){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P2());
        }else if(isP1Val && isP2Val){
            iRes._intNum = 2;
            iRes.SetIntPts(iRes2.P1(),iRes2.P2());
        }
        HandleEndptOnTuple(iRes1.IsInt(),iRes1.P1(),iRes2,iRes);
        iRes.SetExistAll();
    }
    static void GetSegmentArcInt(const ofts::DefSeg& seg,
                                 const ofts::DefArc& arc,
                                 const numst::IntParam& param,
                                 ofts::GeomIntRes& iRes)
    {
        GetSegmentArcInt(seg.GetSp(),seg.GetEp(),arc,param,iRes);
    }
    static void GetArcIntersect(const ofts::DefArc& arc1,
                                const ofts::DefArc& arc2,
                                const numst::IntParam& param,
                                ofts::GeomIntRes& iRes)
    {
        if(!CircleIntCond(arc1.GetCp(),arc1.R(),arc2.GetCp(),arc2.R(),param)){
            return;
        }
        if(!CurveBoxInt().IsCirceCirceBoxInt(arc1.GetCp(),arc1.R(),arc2.GetCp(),arc2.R(),
                                             param.EndptPre() * ErrTimes_10)){
            return;
        }
        ofts::GeomIntRes iRes1;
        ///IsTwoArcSameEndpt(arc1,arc2,param.EndptPre(),iRes1);
        ArcIntersectEndpt(arc1,arc2,param,iRes1);
        if(iRes1.IsInt() && iRes1.Num() == 2){
            iRes.SetNum(2);
            iRes.SetIntPts(iRes1.P1(),iRes1.P2());
            iRes.SetExistAll();
            return;
        }
        ofts::GeomIntRes iRes2;
        GetCircleIntersect(arc1.GetCp(),arc1.R(),arc2.GetCp(),arc2.R(),param,iRes2,false);
        if(!iRes2.IsInt() && !iRes1.IsInt()){return;}
        bool isP1Val = false,isP2Val = false;
        if(PtOnCurve2D::IsPointOnArc(arc1,iRes2.P1(),param.EndptPre(),
                                     param.OnArcPre(),false)){
            if(PtOnCurve2D::IsPointOnArc(arc2,iRes2.P1(),param.EndptPre(),
                                         param.OnArcPre(),false)){
                isP1Val = true;
            }
        }
        if(iRes2.Num() == 2){
            if(PtOnCurve2D::IsPointOnArc(arc1,iRes2.P2(),param.EndptPre(),
                                         param.OnArcPre(),false)){
                if(PtOnCurve2D::IsPointOnArc(arc2,iRes2.P2(),param.EndptPre(),
                                             param.OnArcPre(),false)){
                    isP2Val = true;
                }
            }
        }
        if(!isP1Val && !isP2Val && !iRes1.IsInt()){return;}
        if(isP1Val && !isP2Val){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P1());
        }else if(!isP1Val && isP2Val){
            iRes._intNum = 1;
            iRes.SetIntPt(iRes2.P2());
        }else if(isP1Val && isP2Val){
            iRes._intNum = 2;
            iRes.SetIntPts(iRes2.P1(),iRes2.P2());
        }
        HandleEndptOnTuple(iRes1.IsInt(),iRes1.P1(),iRes2,iRes);
        iRes.SetExistAll();
    }
    static void SegEndptOnArc(const ofts::Point& endpt,
                              const ofts::DefArc& arc,
                              bool isArcSpOn,bool isArcEpOn,
                              const numst::IntParam& param,
                              ofts::GeomIntRes& iRes)
    {
        if(isArcSpOn && !isArcEpOn){
            if(endpt.IsSamePoint2D(arc.GetSp(),param.EndptPre())){
                iRes._intNum = 1;
                iRes.SetIntPt(endpt.Midpoint(arc.GetSp()));
            }else{
                iRes._intNum = 2;
                iRes.SetIntPts(endpt,arc.GetSp());
            }
        }else if(!isArcSpOn && isArcEpOn){
            if(endpt.IsSamePoint2D(arc.GetEp(),param.EndptPre())){
                iRes._intNum = 1;
                iRes.SetIntPt(endpt.Midpoint(arc.GetEp()));
            }else{
                iRes._intNum = 2;
                iRes.SetIntPts(endpt,arc.GetEp());
            }
        }else if(isArcSpOn && isArcEpOn){
            iRes._intNum = 2;
            iRes.SetIntPts(arc.GetSp(),arc.GetEp());
        }else{
            iRes._intNum = 1;
            iRes.SetIntPt(endpt);
        }
    }
    static void ArcEndptOnSeg(const ofts::Point& endpt,const ofts::Point& sp,
                              const ofts::Point& ep,bool isSegSpOn,
                              bool isSegEpOn,const numst::IntParam& param,
                              ofts::GeomIntRes& iRes)
    {
        if(isSegSpOn && !isSegEpOn){
            if(endpt.IsSamePoint2D(sp,param.EndptPre())){
                iRes._intNum = 1;
                iRes.SetIntPt(endpt.Midpoint(sp));
            }else{
                iRes._intNum = 2;
                iRes.SetIntPts(endpt,sp);
            }
        }
        else if(!isSegSpOn && isSegEpOn){
            if(endpt.IsSamePoint2D(ep,param.EndptPre())){
                iRes._intNum = 1;
                iRes.SetIntPt(endpt.Midpoint(ep));
            }else{
                iRes._intNum = 2;
                iRes.SetIntPts(endpt,ep);
            }
        }else if(isSegSpOn && isSegEpOn){
            iRes._intNum = 2;
            iRes.SetIntPts(sp,ep);
        }else{
            iRes._intNum = 1;
            iRes.SetIntPt(endpt);
        }
    }
    static void SegArcIntEndpt(const ofts::Point& sp,
                               const ofts::Point& ep,ofts::DefArc arc,
                               const numst::IntParam& param,
                               ofts::GeomIntRes& iRes)
    {
        bool isSegSpOn = false,isSegEpOn = false,
                isArcSpOn = false,isArcEpOn = false;
        const auto& endE = param.EndptPre();
        isSegSpOn = PtOnCurve2D::IsPointOnArc(arc,sp,endE,param.OnArcPre(),true);
        isSegEpOn = PtOnCurve2D::IsPointOnArc(arc,ep,endE,param.OnArcPre(),true);
        isArcSpOn = PtOnCurve2D::IsPointOnSegment(sp,ep,arc.GetSp(),endE,param.OnSegPre(),true);
        isArcEpOn = PtOnCurve2D::IsPointOnSegment(sp,ep,arc.GetEp(),endE,param.OnSegPre(),true);
        if(!isSegSpOn && !isSegEpOn && !isArcSpOn && !isArcEpOn){
            return;
        }
        if(isSegSpOn && !isSegEpOn){
            SegEndptOnArc(sp,arc,isArcSpOn,isArcEpOn,param,iRes);
        }else if(!isSegSpOn && isSegEpOn){
            SegEndptOnArc(ep,arc,isArcSpOn,isArcEpOn,param,iRes);
        }else if(isSegSpOn && isSegEpOn){
            iRes.SetNum(2);
            iRes.SetIntPts(sp,ep);
        }else if(isArcSpOn && !isArcEpOn){
            ArcEndptOnSeg(arc.GetSp(),sp,ep,isSegSpOn,isSegEpOn,param,iRes);
        }else if(!isArcSpOn && isArcEpOn){
            ArcEndptOnSeg(arc.GetEp(),sp,ep,isSegSpOn,isSegEpOn,param,iRes);
        }else if(isArcSpOn && isArcEpOn){
            iRes.SetNum(2);
            iRes.SetIntPts(arc.GetSp(),arc.GetEp());
        }
        iRes.SetExistAll();
    }
    static void ArcIntersectEndpt(const ofts::DefArc& arc1,
                                  const ofts::DefArc& arc2,
                                  const numst::IntParam& param,
                                  ofts::GeomIntRes& iRes)
    {
        const auto& sp1 = arc1.GetSp();
        const auto& ep1 = arc1.GetEp();
        const auto& sp2 = arc2.GetSp();
        const auto& ep2 = arc2.GetEp();
        bool isSp1On = false,isEp1On = false,isSp2On = false,isEp2On = false;
        isSp1On = PtOnCurve2D::IsPointOnArc(arc2,sp1,param.EndptPre(),param.OnArcPre(),true);
        isEp1On = PtOnCurve2D::IsPointOnArc(arc2,ep1,param.EndptPre(),param.OnArcPre(),true);
        isSp2On = PtOnCurve2D::IsPointOnArc(arc1,sp2,param.EndptPre(),param.OnArcPre(),true);
        isEp2On = PtOnCurve2D::IsPointOnArc(arc1,ep2,param.EndptPre(),param.OnArcPre(),true);
        if(!isSp1On && !isEp1On && !isSp2On && !isEp2On){return;}
        else if(isSp1On && !isEp1On){
            ArcEnptOnArc(sp1,arc2,isSp2On,isEp2On,param,iRes);
        }else if(!isSp1On && isEp1On){
            ArcEnptOnArc(ep1,arc2,isSp2On,isEp2On,param,iRes);
        }else if(isSp1On && isEp1On){
            if(isSp2On && isEp2On){
                std::pair<ofts::Point,double>dists[4];
                auto Val = [](const ofts::Point& p,const ofts::Point& p1,
                        const ofts::Point& p2,const ofts::Point& p3)->double{
                    double d1 = p.DistSquare2D(p1),
                            d2 = p.DistSquare2D(p2),d3 = p.DistSquare2D(p3);
                    if(d1 < d2){d1 = d2;}if(d1 < d3){d1 = d3;}
                    return d1;
                };
                double d1 = Val(sp1,ep1,sp2,ep2),d2 = Val(ep1,sp1,sp2,ep2),
                        d3 = Val(sp2,ep2,sp1,ep1),d4 = Val(ep2,sp2,sp1,ep1);
                dists[0] = std::make_pair(sp1,d1);
                dists[1] = std::make_pair(ep1,d2);
                dists[2] = std::make_pair(sp2,d3);
                dists[3] = std::make_pair(ep2,d4);
                std::sort(std::begin(dists), std::end(dists),[](
                          const std::pair<ofts::Point,double>& a,
                          const std::pair<ofts::Point, double>& b) {
                    return a.second < b.second;});
                if(dists[3].second < param._endptPre * param._endptPre){
                    ofts::Point op1 = sp1.Midpoint(ep1);
                    ofts::Point op2 = sp2.Midpoint(ep2);
                    iRes.SetIntPt(op1.Midpoint(op2));
                    iRes.SetNum(1);
                }else{
                    auto p1 = dists[0].first,p2 = dists[1].first,
                            p3 = dists[2].first,p4 = dists[3].first;
                    ofts::Point op1,op2;
                    if(p3.DistSquare2D(p1) < p3.DistSquare2D(p2)){op1 = p1;}
                    else{op1 = p2;}
                    if(p4.DistSquare2D(p1) < p4.DistSquare2D(p2)){op2 = p1;}
                    else{op2 = p2;}
                    op1 = op1.Midpoint(p3);
                    op2 = op2.Midpoint(p4);
                    iRes.SetIntPts(op1,op2);
                    iRes.SetNum(2);
                }
            }else{
                iRes.SetIntPts(sp1,ep1);iRes.SetNum(2);
            }
        }else if(isSp2On && !isEp2On){
            ArcEnptOnArc(sp2,arc1,isSp1On,isEp1On,param,iRes);
        }else if(!isSp2On && isEp2On){
            ArcEnptOnArc(ep2,arc1,isSp1On,isEp1On,param,iRes);
        }else{
            iRes.SetIntPts(sp2,ep2);
            iRes.SetNum(2);
        }
        ///iRes.SetExistAll();
    }
    static void ArcEnptOnArc(const ofts::Point& endpt,
                             const ofts::DefArc& anoArc,
                             bool isAnoSpOn,bool isAnoEpOn,
                             const numst::IntParam& param,
                             ofts::GeomIntRes& iRes)
    {
        const auto& sp = anoArc.GetSp();
        const auto& ep = anoArc.GetEp();
        if(!isAnoSpOn && !isAnoEpOn){
            iRes.SetIntPt(endpt);
            iRes.SetNum(1);
        }else if(isAnoSpOn && !isAnoEpOn){
            if(endpt.IsSamePoint2D(sp,param.EndptPre())){
                iRes.SetIntPt(endpt.Midpoint(sp));
                iRes.SetNum(1);
            }else{
                iRes.SetIntPts(endpt,sp);
                iRes.SetNum(2);
            }
        }else if(!isAnoSpOn && isAnoEpOn){
            if(endpt.IsSamePoint2D(ep,param.EndptPre())){
                iRes.SetIntPt(endpt.Midpoint(ep));
                iRes.SetNum(1);
            }else{
                iRes.SetIntPts(endpt,ep);
                iRes.SetNum(2);
            }
        }else{
            iRes.SetIntPts(sp,ep);
            iRes.SetNum(2);
        }
        iRes.SetExistAll();
    }
    static void IsTwoArcSameEndpt(const ofts::DefArc& arc1,
                                  const ofts::DefArc& arc2,
                                  double pre,
                                  ofts::GeomIntRes& iRes)
    {
        const auto& sp1 = arc1.GetSp();
        const auto& ep1 = arc1.GetEp();
        const auto& sp2 = arc2.GetSp();
        const auto& ep2 = arc2.GetEp();
        ofts::Point p1,p2;
        bool isSamePt1 = false,isSamePt2 = false;
        isSamePt1 = sp1.IsSamePoint2D(sp2,pre);
        if(isSamePt1){p1 = sp1.Midpoint(sp2);}
        else{
            isSamePt1 = sp1.IsSamePoint2D(ep2,pre);
            if(isSamePt1){p1 = sp1.Midpoint(ep2);}
        }
        isSamePt2 = ep1.IsSamePoint2D(sp2,pre);
        if(isSamePt2){p2 = ep1.Midpoint(sp2);}
        else{
            isSamePt2 = ep1.IsSamePoint2D(ep2,pre);
            if(isSamePt2){p2 = ep1.Midpoint(ep2);}
        }
        if(!isSamePt1 && !isSamePt2){return;}
        if(isSamePt1 && !isSamePt2){
            iRes._intNum = 1;
            iRes.SetIntPt(p1);
        }else if(!isSamePt1 && isSamePt2){
            iRes._intNum = 1;
            iRes.SetIntPt(p2);
        }else {
            iRes._intNum = 2;
            iRes.SetIntPts(p1,p2);
        }
        iRes._isExistInt = true;
    }
    static void GetCurveIntersect(const ofts::DefElem& c1,
                                  const ofts::DefElem& c2,
                                  const numst::IntParam& param,
                                  ofts::GeomIntRes& iRes)
    {
        switch (c1._type) {
        case Ofte::LINETYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                GetSegmentIntersect(c1._segment.GetSp(),c1._segment.GetEp(),
                                    c2._segment.GetSp(),c2._segment.GetEp(),param,iRes);
                break;
            case Ofte::ARCTYPE:
                GetSegmentArcInt(c1._segment.GetSp(),c1._segment.GetEp(),
                                 c2._arc,param,iRes);
                break;
            case Ofte::CIRCLETYPE:
                GetSegmentCircleInt(c1._segment.GetSp(),c1._segment.GetEp(),
                                    c2._circle._cirCp,c2._circle._cirR,param,iRes);
                break;
            }
            break;
        case Ofte::ARCTYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                GetSegmentArcInt(c2._segment.GetSp(),c2._segment.GetEp(),
                                 c1._arc,param,iRes);
                break;
            case Ofte::ARCTYPE:
                GetArcIntersect(c1._arc,c2._arc,param,iRes);
                break;
            case Ofte::CIRCLETYPE:
                GetArcCircleInt(c1._arc,c2._circle._cirCp,
                                c2._circle._cirR,param,iRes);
                break;
            }
            break;
        case Ofte::CIRCLETYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                GetSegmentCircleInt(c2._segment.GetSp(),c2._segment.GetEp(),
                                    c1._circle._cirCp,c1._circle._cirR,param,iRes);
                break;
            case Ofte::ARCTYPE:
                GetArcCircleInt(c2._arc,c1._circle._cirCp,c1._circle._cirR,param,iRes);
                break;
            case Ofte::CIRCLETYPE:
                GetCircleIntersect(c1._circle._cirCp,c1._circle._cirR,
                                   c2._circle._cirCp,c2._circle._cirR,param,iRes);
                break;
            }
            break;
        default: break;
        }
    }

    static void GetCurveGeomInt(const ofts::DefElem& c1,
                                const ofts::DefElem& c2,
                                const numst::IntParam& param,
                                ofts::GeomIntRes& iRes)
    {
        switch (c1._type) {
        case Ofte::LINETYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                GetLineIntersect(c1._segment.GetSp(),c1._segment.GetEp(),
                                 c2._segment.GetSp(),c2._segment.GetEp(),param,iRes);
                break;
            case Ofte::ARCTYPE:
                GetLineCircleInt(c1._segment.GetSp(),c1._segment.GetEp(),
                                 c2._arc.GetCp(),c2._arc.R(),param,iRes);
                break;
            case Ofte::CIRCLETYPE:
                GetLineCircleInt(c1._segment.GetSp(),c1._segment.GetEp(),
                                 c2._circle.GetCp(),c2._circle.R(),param,iRes);
                break;
            }
            break;
        case Ofte::ARCTYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                GetLineCircleInt(c2._segment.GetSp(),c2._segment.GetEp(),
                                 c1._arc.GetCp(),c1._arc.R(),param,iRes);
                break;
            case Ofte::ARCTYPE:
                GetCircleIntersect(c1._arc.GetCp(),c1._arc.R(),
                                   c2._arc.GetCp(),c2._arc.R(),param,iRes);
                break;
            case Ofte::CIRCLETYPE:
                GetCircleIntersect(c1._arc.GetCp(),c1._arc.R(),
                                   c2._circle.GetCp(),c2._circle.R(),param,iRes);
                break;
            }
            break;
        case Ofte::CIRCLETYPE:
            switch (c2._type) {
            case Ofte::LINETYPE:
                GetLineCircleInt(c2._segment.GetSp(),c2._segment.GetEp(),
                                 c1._circle.GetCp(),c1._circle.R(),param,iRes);
                break;
            case Ofte::ARCTYPE:
                GetCircleIntersect(c1._circle.GetCp(),c1._circle.R(),
                                   c2._arc.GetCp(), c2._arc.R(),param,iRes);
                break;
            case Ofte::CIRCLETYPE:
                GetCircleIntersect(c1._circle.GetCp(),c1._circle.R(),
                                   c2._circle.GetCp(),c2._circle.R(),param,iRes);
                break;
            }
            break;
        default: break;
        }
    }
};
#endif // CURVEINT2D_H

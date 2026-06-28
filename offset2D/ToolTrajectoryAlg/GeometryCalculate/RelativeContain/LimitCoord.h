#ifndef LIMITCOORD2D_H
#define LIMITCOORD2D_H
#include "../GeomCalculate/CurveLog2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
class LimitCoord2D
{
public:
    LimitCoord2D(){}


    static bool IsLimitCoordOut(const numst::LimCoord& out,const numst::LimCoord& in,double e)
    {
        return (out.MinX() - e) > in.MaxX() || (out.MaxX() + e) < in.MinX() ||
                (out.MinY() - e) > in.MaxY() || (out.MaxY() + e) < in.MinY();
    }
    static bool IsLimitCoordCont(const numst::LimCoord& out,const numst::LimCoord& in,double e)
    {
        return (out.MinX() - e) < in.MinX() && (out.MaxX() + e) > in.MaxX() &&
                (out.MinY() - e) < in.MinY() && (out.MaxY() + e) > in.MaxY();
    }
    /*static bool IsLimitCoordOut(const numst::LimCoord& out,const numst::LimCoord& in,double e)
    {
        return (in.MinX() - e) > out.MaxX() || (in.MaxX() + e) < out.MinX() ||
                (in.MinY() - e) > out.MaxY() || (in.MaxY() + e) < out.MinY();
    }*/
    static double SpecifyHorVerCoord(const double& minX,const double& maxX,
                                     const double& minY,const double& maxY,
                                     bool isHor,bool isMax)
    {
        if(isHor){
            if(isMax){return maxX;}
            else{return minX;}
        }else{
            if(isMax){return maxY;}
        }
        return minY;
    }
    static double TwoPtsHorVerLimit(const ofts::Point& sp,
                                    const ofts::Point& ep,
                                    bool isHor, bool isMax)
    {
        double minX,maxX,minY,maxY;
        minX = sp.X() < ep.X() ? sp.X() : ep.X();
        maxX = sp.X() > ep.X() ? sp.X() : ep.X();
        minY = sp.Y() < ep.Y() ? sp.Y() : ep.Y();
        maxY = sp.Y() > ep.Y() ? sp.Y() : ep.Y();
        return SpecifyHorVerCoord(minX,maxX,minY,maxY,isHor,isMax);
    }
    static void TwoPtLimitCoord(const ofts::Point& p0,const ofts::Point& p1,
                                double& minx,double& maxx,double& miny,double& maxy)
    {
        if(p0.X() < p1.X()){minx = p0.X();maxx = p1.X();}
        else{minx = p1.X();maxx = p0.X();}
        if(p0.Y() < p1.Y()){miny = p0.Y();maxy = p1.Y();}
        else{miny = p1.Y();maxy = p0.Y();}
    }
    static ofts::Point SegLimitPoint(const ofts::Point& sp,
                                     const ofts::Point& ep,
                                     bool isHor,bool isMax)
    {
        ofts::Point P;
        if(isHor){
            if(isMax){
                P = sp.X() > ep.X() ? sp : ep;
            }else{
                P = sp.X() < ep.X() ? sp : ep;
            }
        }else{
            if(isMax){
                P = sp.Y() > ep.Y() ? sp : ep;
            }else{
                P = sp.Y() < ep.Y() ? sp : ep;
            }
        }
        return P;
    }
    static ofts::Point ArcLimitPoint(const ofts::DefArc& arc,bool isHor,bool isMax)
    {
        ofts::Point P;
        double sAgl,eAgl;
        BaseGeom2D::ArcEndpointAngle(arc,sAgl,eAgl);
        bool isInAgl = false;
        if(isHor){
            if(isMax){
                isInAgl = pnum::AglIsBetweenAgls(sAgl,eAgl,arc.IsCW(),0,PreErr5_10);
                if(isInAgl){
                    P.SetX(arc.GetCp().X() + arc.GetOptR());
                    P.SetY(arc.GetCp().Y());
                }else{
                    P = arc.GetSp().X() > arc.GetEp().X() ? arc.GetSp() : arc.GetEp();
                }
                const auto& endPt = arc.GetSp().X() > arc.GetEp().X() ? arc.GetSp() : arc.GetEp();
                if(endPt.X() > P.X()){
                    P = endPt;
                }
                return P;
            }
            isInAgl = pnum::AglIsBetweenAgls(sAgl,eAgl,arc.IsCW(),180,PreErr5_10);
            if(isInAgl){
                P.SetX(arc.GetCp().X() - arc.GetOptR());
                P.SetY(arc.GetCp().Y());
            }else{
                P = arc.GetSp().X() < arc.GetEp().X() ? arc.GetSp() : arc.GetEp();
            }
            const auto& endPt = arc.GetSp().X() < arc.GetEp().X() ? arc.GetSp() : arc.GetEp();
            if(endPt.X() < P.X()){
                P = endPt;
            }
            return P;
        }
        if(isMax){
            isInAgl = pnum::AglIsBetweenAgls(sAgl,eAgl,arc.IsCW(),90,PreErr5_10);
            if(isInAgl){
                P.SetX(arc.GetCp().X());
                P.SetY(arc.GetCp().Y() + arc.GetOptR());
            }else{
                P = arc.GetSp().Y() > arc.GetEp().Y() ? arc.GetSp() : arc.GetEp();
            }
            const auto& endPt = arc.GetSp().Y() > arc.GetEp().Y() ? arc.GetSp() : arc.GetEp();
            if(endPt.Y() > P.Y()){
                P = endPt;
            }
            return P;
        }
        isInAgl = pnum::AglIsBetweenAgls(sAgl,eAgl,arc.IsCW(),270,PreErr5_10);
        if(isInAgl){
            P.SetX(arc.GetCp().X());
            P.SetY(arc.GetCp().Y() - arc.GetOptR());
        }else{
            P = arc.GetSp().Y() < arc.GetEp().Y() ? arc.GetSp() : arc.GetEp();
        }
        const auto& endPt = arc.GetSp().Y() < arc.GetEp().Y() ? arc.GetSp() : arc.GetEp();
        if(endPt.Y() < P.Y()){
            P = endPt;
        }
        return P;
    }
    static double CircleLimitHorVerCoord(const ofts::Point& cp,double R,bool isHor,bool isMax)
    {
        if(isHor){
            if(isMax){return cp.X() + R;}
            else{return cp.X() - R;}
        }else{
            if(isMax){return cp.Y() + R;}
        }
        return cp.Y() - R;
    }
    static ofts::Point CircleLimitPoint(const ofts::Point& cp,double R,bool isHor,bool isMax)
    {
        ofts::Point P;
        P.SetZ(cp.Z());
        if(isHor){
            P.SetY(cp.Y());
            if(isMax){P.SetX(cp.X() + R);
            }else{P.SetX(cp.X() - R);}
        }else{
            P.SetX(cp.X());
            if(isMax){P.SetY(cp.Y() + R);
            }else{ P.SetY(cp.Y() - R);}
        }
        return P;
    }
    static double CurveLimitCoord(const ofts::DefElem& c,bool isHor,bool isMax)
    {
        switch (c.Type()) {
        case Ofte::LINETYPE:
            return TwoPtsHorVerLimit(c._segment.GetSp(),c._segment.GetEp(),isHor,isMax);
        case Ofte::ARCTYPE:{
            auto p = ArcLimitPoint(c._arc,isHor,isMax);
            if(isHor){return p.X();}
            else{return p.Y();}
        }
        case Ofte::CIRCLETYPE:
            return CircleLimitHorVerCoord(c._circle.GetCp(),c._circle.R(),isHor,isMax);
        default:return Max_Value;
        }
        return Max_Value;
    }
    static numst::LimCoord SegLimCoord(const ofts::DefSeg& s){
        double minx,maxx,miny,maxy;
        TwoPtLimitCoord(s.GetSp(),s.GetEp(),minx,maxx,miny,maxy);
        return numst::LimCoord(minx,maxx,miny,maxy);
    }
    static numst::LimCoord ArcLimCoord(const ofts::DefArc& a){
        double minx = ArcLimitPoint(a,true,false).X();
        double maxx = ArcLimitPoint(a,true,true).X();
        double miny = ArcLimitPoint(a,false,false).Y();
        double maxy = ArcLimitPoint(a,false,true).Y();
        return numst::LimCoord(minx,maxx,miny,maxy);
    }
    static numst::LimCoord CurveLimitCoord(const ofts::DefElem& c){
        double minx = CurveLimitCoord(c,true,false);
        double maxx = CurveLimitCoord(c,true,true);
        double miny = CurveLimitCoord(c,false,false);
        double maxy = CurveLimitCoord(c,false,true);
        return numst::LimCoord(minx,maxx,miny,maxy);
    }
    static void SetCurveLimitCoord(ofts::DefElem& c)
    {
        c.SetLimCoord(CurveLimitCoord(c));
    }
    static ofts::Point CurveLimitPoint(const ofts::DefElem &c,bool isHor,bool isMax)
    {
        switch (c.Type()) {
        case LINETYPE:{
            return SegLimitPoint(c._segment.GetSp(),c._segment.GetEp(),isHor,isMax);
        }
        case ARCTYPE:{
            return  ArcLimitPoint(c._arc,isHor,isMax);
        }
        case CIRCLETYPE:{
            CircleLimitPoint(c._circle.GetCp(),c._circle.R(),isHor,isMax);
        }
        default: return ofts::Point();
        }
        return ofts::Point();
    }
    static ofts::Point LoopLimitPoint(const ofts::DefLoop& loop,bool isHor,bool isMax)
    {
        if(loop.IsEmpty()){return ofts::Point();}
        if(loop[0].Type() == Ofte::CIRCLETYPE){
            return CircleLimitPoint(loop[0]._circle.GetCp(),
                    loop[0]._circle.R(),isHor,isMax);
        }
        std::vector<ofts::Point>ps;
        ps.reserve(loop.Size());
        for(const auto& c : loop){
            if(c.Type() == Ofte::LINETYPE){
                ps.emplace_back(c._segment.GetSp());
            }else{
                ps.emplace_back(ArcLimitPoint(c._arc,isHor,isMax));
            }
        }
        auto p = ps.front();
        if(isHor){
            if(isMax){
                for(auto& p_ : ps) {
                    if(p_.X() > p.X()){
                        p = p_;
                    }
                }
            }else{
                for(auto& p_ : ps) {
                    if(p_.X() < p.X()){
                        p = p_;
                    }
                }
            }
        }else{
            if(isMax){
                for(auto& p_ : ps) {
                    if(p_.Y() > p.Y()){
                        p = p_;
                    }
                }
            }else{
                for(auto& p_ : ps) {
                    if(p_.Y() < p.Y()){
                        p = p_;
                    }
                }
            }
        }
        return p;
    }
    static double LoopLimitCoord(const ofts::DefLoop& loop,bool isHor,bool isMax)
    {
        auto p = LoopLimitPoint(loop,isHor,isMax);
        if(isHor){return p.X();}
        return p.Y();
    }
    static numst::LimCoord LoopLimitCoord(const ofts::DefLoop& loop)
    {
        double minX = LoopLimitCoord(loop,true,false);
        double maxX = LoopLimitCoord(loop,true,true);
        double minY = LoopLimitCoord(loop,false,false);
        double maxY = LoopLimitCoord(loop,false,true);
        return numst::LimCoord(minX,maxX,minY,maxY);
    }
    static void SetLoopLimCoord(ofts::DefLoop& loop){
        double x1 = Max_Value,x2 = Min_Value,y1 = Max_Value,y2 = Min_Value;
        for (size_t i = 0; i < loop.Size(); i++){
            auto& c = loop[i];
            auto lim = CurveLimitCoord(c);
            c.SetLimCoord(lim);
            if(x1 > lim.MinX()){x1 = lim.MinX();}
            if(x2 < lim.MaxX()){x2 = lim.MaxX();}
            if(y1 > lim.MinY()){y1 = lim.MinY();}
            if(y2 < lim.MaxY()){y2 = lim.MaxY();}
        }
        loop.SetLimCoord(numst::LimCoord(x1,x2,y1,y2));
    }
    static void LoopMinimumBox(const ofts::DefLoop& loop,double& xSpan,double& ySpan)
    {
        auto c = LoopLimitCoord(loop);
        xSpan = c.MaxX() - c.MinX();
        ySpan = c.MaxY() - c.MinY();
    }
    static void LoopMinCircumcircle(const ofts::DefLoop &loop,
                                    ofts::DefCircle& circle,
                                    bool isAgl = true,double err = 5)
    {
        if(loop.IsEmpty()){return;}
        if(loop.IsCircleLoop()){
            circle = loop.First()._circle;return;
        }
        double step;
        std::vector<ofts::Point>pts;
        for(const auto& d : loop){
            if(d.Type() == LINETYPE){
                pts.emplace_back(d._segment._segSp);
            }else if(d.Type() == ARCTYPE){
                step = err;
                std::vector<ofts::Point>ps;
                if(!isAgl){step = BaseGeom2D::AngleBaseBowH(d.GetArc().R(),err);}
                BaseGeom2D::DiscreteArcToPoint(d._arc,step,ps);
                if(!ps.empty()){ps.pop_back();}
                for(auto& p : ps){
                    pts.emplace_back(p);
                }
            }
        }
#if 0
        DisplayGeom().DisplayLoop(loop,_colors.back(),1);
        for (int i = 0; i < pts.size(); ++i) {
            DisplayGeom().DisplayPoint(pts[i],_colors[6]);
        }
        _mainwind->myOccView->fitAll();
#endif
        auto current = ofts::DefCircle(pts[0], 0);
        for (size_t i = 1; i < pts.size(); ++i) {
            /// 如果当前点不在圆内，需要更新圆
            if (current.IsContPt(pts[i], PreErr5_10))
                continue;
            /// 新圆初始化为只包含这个点
            current = ofts::DefCircle(pts[i], 0);
            /// 重新检查前面的所有点
            for (size_t j = 0; j < i; ++j) {
                if (current.IsContPt(pts[j], PreErr5_10))
                    continue;
                /// 构造包含这两个点的圆
                current = ofts::DefCircle(pts[i].Midpoint(pts[j]),
                                          pts[i].Distance2D(pts[j]) / 2);
                /// 再次检查前面的点
                for (size_t k = 0; k < j; ++k) {
                    if (current.IsContPt(pts[k],PreErr5_10))
                        continue;
                    /// 构造包含这三个点的圆
                    const auto& a = pts[i];
                    const auto& b = pts[j];
                    const auto& c = pts[k];
                    double A = b.X() - a.X();
                    double B = b.Y() - a.Y();
                    double C = c.X() - a.X();
                    double D = c.Y() - a.Y();
                    double E = A * (a.X() + b.X()) + B * (a.Y() + b.Y());
                    double F = C * (a.X() + c.X()) + D * (a.Y() + c.Y());
                    double G = 2 * (A * (c.Y() - b.Y()) - B * (c.X() - b.X()));
                    /// 三点共线,返回直径最大的圆
                    if (std::abs(G) < PreErr5_10) {
                        auto c1 = ofts::DefCircle(a.Midpoint(b),a.Distance2D(b)/2);
                        auto c2 = ofts::DefCircle(a.Midpoint(c),a.Distance2D(c)/2);
                        auto c3 = ofts::DefCircle(b.Midpoint(c),b.Distance2D(c)/2);
                        if (c1._cirR >= c2._cirR && c1._cirR >= c3._cirR) {
                            current = c1;
                        }else if (c2._cirR >= c1._cirR && c2._cirR >= c3._cirR) {
                            current = c2;
                        }else {
                            current = c3;
                        }
                        continue;
                    }
                    double x = (D * E - B * F) / G;
                    double y = (A * F - C * E) / G;
                    ofts::Point cp(x, y);
                    double r = cp.Distance2D(a);
                    current = ofts::DefCircle(cp,r);
                }
            }
        }
        circle = current;
#if 0
        DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,circle),
                                     _colors.back(),1);
        _mainwind->myOccView->fitAll();
#endif
    }
};
#endif // LIMITCOORD2D_H

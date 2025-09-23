#include <tbb/tbb.h>
#include "LimitCoordinate.h"
#if DebugModel
#pragma optimize("", off)
#endif
void LimitCoordinate::SpecifyHorVerCoordValue(double minX,double maxX,
                                              double minY,double maxY,
                                              bool isHor,bool isMax,
                                              double& specifyVal)
{
    if(isHor){
        specifyVal = minX;
        if(isMax)
            specifyVal = maxX;
    }
    else{
        specifyVal = minY;
        if(isMax)
            specifyVal = maxY;
    }
}
void LimitCoordinate::SegLimitHorVerCoord(const Point& lSp,
                                          const Point& lEp,
                                          bool isHor, bool isMax,
                                          double& specifyVal)
{
    double minX,maxX,minY,maxY;
    minX = lSp.X() < lEp.X() ? lSp.X() : lEp.X();
    maxX = lSp.X() >= lEp.X() ? lSp.X() : lEp.X();
    minY = lSp.Y() < lEp.Y() ? lSp.Y() : lEp.Y();
    maxY = lSp.Y() >= lEp.Y() ? lSp.Y() : lEp.Y();
    SpecifyHorVerCoordValue(minX,maxX,minY,maxY,
                            isHor,isMax,specifyVal);
}
void LimitCoordinate::SegLimitCoordPoint(const Point& lSp,const Point& lEp,
                                         bool isHor, bool isMax,Point& P)
{
    if(isHor){
        if(isMax){
            P = lSp.X() >= lEp.X() ? lSp : lEp;
        }
        else{
            P = lSp.X() <= lEp.X() ? lSp : lEp;
        }
    }
    else{
        if(isMax){
            P = lSp.Y() >= lEp.Y() ? lSp : lEp;
        }
        else{
            P = lSp.Y() <= lEp.Y() ? lSp : lEp;
        }
    }
}
void LimitCoordinate::ArcLimitHorVerCoord(DefArc arc,bool isHor,
                                          bool isMax,Point& P)
{
    GeomCalculate gocal;
    double startAgl,endAgl;    gocal.ArcEndpointAngle(arc,startAgl,endAgl);
    bool isInAngle = false;
    arc.ChoiceSetRadius(PreErr5_2);
    if(isHor){
        if(isMax){
            gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._arcRot,
                                       0,PreErr5_10,isInAngle);
            if(isInAngle){
                P.SetX(arc.ArcCenterPt().X() + arc.ArcRadius());
                P.SetY(arc.ArcCenterPt().Y());
            }
            else{
                P = arc.ArcStartPt().X() >= arc.ArcEndPt().X() ?
                            arc.ArcStartPt() : arc.ArcEndPt();
            }
            Point endPt = arc.ArcStartPt().X() > arc.ArcEndPt().X() ?
                        arc.ArcStartPt() : arc.ArcEndPt();
            if(endPt.X() > P.X()){
                P = endPt;
            }
        }
        else{
            gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._arcRot,
                                       180,PreErr5_10,isInAngle);
            if(isInAngle){
                P.SetX(arc.ArcCenterPt().X() - arc.ArcRadius());
                P.SetY(arc.ArcCenterPt().Y());
            }
            else{
                P = arc.ArcStartPt().X() <= arc.ArcEndPt().X() ?
                            arc.ArcStartPt() : arc.ArcEndPt();
            }
            Point endPt = arc.ArcStartPt().X() < arc.ArcEndPt().X() ?
                        arc.ArcStartPt() : arc.ArcEndPt();
            if(endPt.X() < P.X()){
                P = endPt;
            }
        }
    }
    else{
        if(isMax){
            gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._arcRot,
                                       90,PreErr5_10,isInAngle);
            if(isInAngle){
                P.SetX(arc.ArcCenterPt().X());
                P.SetY(arc.ArcCenterPt().Y() + arc.ArcRadius());
            }
            else{
                P = arc.ArcStartPt().Y() >= arc.ArcEndPt().Y() ?
                            arc.ArcStartPt() : arc.ArcEndPt();
            }
            Point endPt = arc.ArcStartPt().Y() > arc.ArcEndPt().Y() ?
                        arc.ArcStartPt() : arc.ArcEndPt();
            if(endPt.Y() > P.Y()){
                P = endPt;
            }
        }
        else{
            gocal.AngleIsBetweenAngles(startAgl,endAgl,arc._arcRot,
                                       270,PreErr5_10,isInAngle);
            if(isInAngle){
                P.SetX(arc.ArcCenterPt().X());
                P.SetY(arc.ArcCenterPt().Y() - arc.ArcRadius());
            }            else{
                P = arc.ArcStartPt().Y() <= arc.ArcEndPt().Y() ?
                            arc.ArcStartPt() : arc.ArcEndPt();
            }
            Point endPt = arc.ArcStartPt().Y() < arc.ArcEndPt().Y() ?
                        arc.ArcStartPt() : arc.ArcEndPt();
            if(endPt.Y() < P.Y()){
                P = endPt;
            }
        }
    }/**
    Point cp = arc.ArcCenterPt(),extPnt;
    double len = 200000;
    if(isHor){
        extPnt.SetY(cp.Y());
        if(isMax)
            extPnt.SetX(cp.X() + len);
        else
            extPnt.SetY(cp.X() - len);
    }
    else{
        extPnt.SetX(cp.X());
        if(isMax)
            extPnt.SetY(cp.Y() + len);
        else
            extPnt.SetY(cp.Y() - len);
    }
    GeomIntRes intRes;
    GeomIntersect goInt;
    goInt.CalculateSegmentArcInt(cp,extPnt,arc,PreErr_10,
                                 PreErr_10,PreErr_10,intRes);
    if(intRes._isExistInt){
        P = intRes._intPnt1;
        return;
    }
    if(isHor){
        if(isMax)
            P = arc._arcSp.X() >= arc._arcEp.X() ?
                        arc._arcSp : arc._arcEp;
        else
            P = arc._arcSp.X() <= arc._arcEp.X() ?
                        arc._arcSp : arc._arcEp;
    }
    else{
        if(isMax)
            P = arc._arcSp.Y() >= arc._arcEp.Y() ?
                        arc._arcSp : arc._arcEp;
        else
            P = arc._arcSp.Y() <= arc._arcEp.Y() ?
                        arc._arcSp : arc._arcEp;
    }*/
}
void LimitCoordinate::CircleLimitHorVerCoord(const Point& cp,
                                             double R,bool isHor,
                                             bool isMax,
                                             double& specifyVal)
{
    if(isHor){
        if(isMax)
            specifyVal = cp.X() + R;
        else            specifyVal = cp.X() - R;
    }
    else{
        if(isMax)
            specifyVal = cp.Y() + R;
        else
            specifyVal = cp.Y() - R;
    }
}
void LimitCoordinate::CircleLimitCoordPoint(const Point& cp,
                                            double R,bool isHor,
                                            bool isMax,Point& P)
{
    P.SetZ(cp.Z());
    if(isHor){
        P.SetY(cp.Y());
        if(isMax){
            P.SetX(cp.X() + R);
        }
        else{
            P.SetX(cp.X() - R);
        }
    }
    else{
        P.SetX(cp.X());
        if(isMax){
            P.SetY(cp.Y() + R);
        }
        else{
            P.SetY(cp.Y() - R);
        }
    }
}
void LimitCoordinate::ElementLimitCoord(const DefElem& ele,
                                        bool isHor, bool isMax,
                                        double &value)
{
    if(ele._elemType == LINETYPE){
        ElemCalculate elecal;
        Point eleSp,eleEp;
        elecal.ElementEndpoint(ele,false,eleSp);
        elecal.ElementEndpoint(ele,true,eleEp);
        SegLimitHorVerCoord(eleSp,eleEp,isHor,isMax,value);
    }
    else if(ele._elemType == ARCTYPE){
        Point P;
        ArcLimitHorVerCoord(ele.ElemArc(),isHor,isMax,P);
        if(isHor){
            value = P.X();
        }
        else{
            value = P.Y();
        }
    }
    else if(ele._elemType == CIRCLETYPE){
        CircleLimitHorVerCoord(ele._circle._cirCp,
                               ele._circle._cirR,
                               isHor,isMax,value);
    }
}
void LimitCoordinate::ElemLimitCoordPoint(const DefElem &ele,                                          bool isHor, bool isMax,
                                          Point &P)
{
    switch (ele.ElemeType()) {
    case LINETYPE:{
        SegLimitCoordPoint(ele.ElemSeg().SegStartPt(),
                           ele.ElemSeg().SegEndPt(),
                           isHor,isMax,P);
        break;
    }
    case ARCTYPE:{
        ArcLimitHorVerCoord(ele.ElemArc(),isHor,isMax,P);
        break;
    }
    case CIRCLETYPE:{
        CircleLimitCoordPoint(ele.ElemCircle().CircleCenter(),
                              ele.ElemCircle().CircleRadius(),
                              isHor,isMax,P);
        break;
    }
    default:
        break;
    }
}
void LimitCoordinate::LoopLimitCoordPoint(const DefLoop& loop,
                                          bool isHor,bool isMax,Point &p)
{
    if(loop.IsEmpty())
        return;
    if(loop.ElemNum() == 1 &&
            loop.IndexElem(0)._elemType == CIRCLETYPE){
        const auto& c = loop.IndexElem(0)._circle;
        p.SetZ(c._cirCp.Z());
        if(isHor){
            if(isMax){
                p.SetX(c._cirCp.X() + c._cirR);
            }else{
                p.SetX(c._cirCp.X() - c._cirR);
            }
            p.SetY(c._cirCp.Y());
        }else{
            if(isMax){
                p.SetY(c._cirCp.Y() + c._cirR);
            }else{
                p.SetY(c._cirCp.Y() - c._cirR);
            }
            p.SetX(c._cirCp.X());
        }
        return;
    }
    vector<Point>pts;
    for(const auto& d : loop.m_vElem){
        Point p_;
        if(d._elemType == LINETYPE){
            p_ = d._segment._segSp;
        }else{
            ArcLimitHorVerCoord(d._arc,isHor,isMax,p_);
        }
        pts.push_back(p_);
    }
    p = pts.front();
    if(isHor){
        if(isMax){            for (size_t i = 1; i < pts.size(); ++i) {
                if(pts[i].X() > p.X()){
                    p = pts[i];
                }
            }
        }else{
            for (size_t i = 1; i < pts.size(); ++i) {
                if(pts[i].X() < p.X()){
                    p = pts[i];
                }
            }
        }
    }else{
        if(isMax){
            for (size_t i = 1; i < pts.size(); ++i) {
                if(pts[i].Y() > p.Y()){
                    p = pts[i];
                }
            }
        }else{
            for (size_t i = 1; i < pts.size(); ++i) {
                if(pts[i].Y() < p.Y()){
                    p = pts[i];
                }
            }
        }
    }
}
void LimitCoordinate::LoopLimitCoordinate(const DefLoop& loop,
                                          bool isHor,bool isMax,
                                          double& limVal)
{
    if(loop.IsEmpty())
        return;
    if(loop.m_vElem.size() == 1 &&
            loop.m_vElem[0]._elemType == CIRCLETYPE){
        CircleLimitHorVerCoord(loop.m_vElem[0]._circle._cirCp,
                loop.m_vElem[0]._circle._cirR,
                isHor,isMax,limVal);
        return;
    }
    vector<Point>vPoint;
#if 1
    for (size_t i = 0; i < loop.ElemNum(); ++i) {
        Point p;
        if(loop.m_vElem[i]._elemType == LINETYPE)
            p = loop.m_vElem[i].ElemSeg().SegStartPt();
        else if(loop.m_vElem[i]._elemType == ARCTYPE)
            ArcLimitHorVerCoord(loop.m_vElem[i].ElemArc(),
                                isHor,isMax,p);
        vPoint.push_back(p);
    }
#else
    vPoint.reserve(loop.ElemNum());
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t e = r.begin(); e < r.end(); ++e) {
            Point P;
            if(loop.m_vElem[e]._elemType == LINETYPE)
                P = loop.m_vElem[e].ElemSeg().SegStartPt();
            else if(loop.m_vElem[e]._elemType == ARCTYPE)
                ArcLimitHorVerCoord(loop.m_vElem[e].ElemArc(),                                    isHor,isMax,P);
            vPoint[e] = P;
        }
    });
#endif
    vector<double>vValue;
    if(isHor){
        for (int p = 0; p < vPoint.size(); ++p){
            vValue.push_back(vPoint[p].X());
        }
    }
    else{
        for (int p = 0; p < vPoint.size(); ++p){
            vValue.push_back(vPoint[p].Y());
        }
    }
    if(isMax){
        auto maxPos = std::max_element(vValue.begin(),vValue.end());
        limVal = *maxPos;
    }
    else{
        auto minPos = std::min_element(vValue.begin(),vValue.end());
        limVal = *minPos;
    }
}
void LimitCoordinate::IsPointInsideLoopCoord(const DefLoop& loop,
                                             const Point& P,double pre,
                                             bool& isIncCoor)
{
    double minX,maxX,minY,maxY;
    LoopLimitCoordinate(loop,true,false,minX);
    LoopLimitCoordinate(loop,true,true,maxX);
    LoopLimitCoordinate(loop,false,false,minY);
    LoopLimitCoordinate(loop,false,true,maxY);
    isIncCoor = ((minX - pre) <= P.X() && (maxX + pre) >= P.X() &&
                 (minY - pre) <= P.Y() && (maxY + pre) >= P.Y());
}
void LimitCoordinate::IsEleInsideLoopCoord(const DefLoop& loop,
                                           const DefElem& ele,double pre,
                                           bool& isIncCoor)
{
    double minX,maxX,minY,maxY;
    LoopLimitCoordinate(loop,true,false,minX);
    LoopLimitCoordinate(loop,true,true,maxX);
    LoopLimitCoordinate(loop,false,false,minY);
    LoopLimitCoordinate(loop,false,true,maxY);
    double minX_,maxX_,minY_,maxY_;
    ElementLimitCoord(ele,true,false,minX_);
    ElementLimitCoord(ele,true,true,maxX_);
    ElementLimitCoord(ele,false,false,minY_);
    ElementLimitCoord(ele,false,true,maxY_);
    isIncCoor = ((minX - pre) <= minX_ && (maxX + pre) >= maxX_ &&
                 (minY - pre) <= minY_ && (maxY + pre) >= maxY_);
}
void LimitCoordinate::IsElemOutsideLoopCoord(const DefLoop& loop,
                                             const DefElem& ele,                                             double pre,
                                             bool& isOutCoord)
{
    double minX,maxX,minY,maxY;
    LoopLimitCoordinate(loop,true,false,minX);
    LoopLimitCoordinate(loop,true,true,maxX);
    LoopLimitCoordinate(loop,false,false,minY);
    LoopLimitCoordinate(loop,false,true,maxY);
    double minX_,maxX_,minY_,maxY_;
    ElementLimitCoord(ele,true,false,minX_);
    ElementLimitCoord(ele,true,true,maxX_);
    ElementLimitCoord(ele,false,false,minY_);
    ElementLimitCoord(ele,false,true,maxY_);
    isOutCoord = (maxX_ <= (minX - pre) || minX_ >= (maxX - pre) ||
                  maxY_ <= (minY - pre) || minY_ >= (maxY - pre));
}
void LimitCoordinate::LoopLimitCoordinate(const DefLoop& loop,
                                          double& minX,double& maxX,
                                          double& minY,double& maxY)
{
    LoopLimitCoordinate(loop,true,false,minX);
    LoopLimitCoordinate(loop,true,true,maxX);
    LoopLimitCoordinate(loop,false,false,minY);
    LoopLimitCoordinate(loop,false,true,maxY);
}
bool LimitCoordinate::IsPointInLoopCoord(const DefLoop &loop,
                                         const Point & P,
                                         double errVal)
{
    bool isIncCoor = false;
    IsPointInsideLoopCoord(loop,P,errVal,isIncCoor);
    return isIncCoor;
}
void LimitCoordinate::LoopMinCircumcircle(const DefLoop &loop,
                                          DefCircle& circle)
{
    if(GeometryLogic().IsCircleStructureLoop(loop)){
        GeometryLogic().CircleStructureCircle(loop,circle);
        return;
    }
    double z = ElemCalculate().LoopZCoordinate(loop);
    double minX,maxX,minY,maxY;
    LoopLimitCoordinate(loop,minX,maxX,minY,maxY);
    Point cp((maxX + minX) / 2,(maxY + minY) / 2,z);
    circle.SetCircleCenter(cp);
    double R;
    GeomDistance().PointToLoopMaxDistance(loop,cp,R);/**
    double D = std::sqrt(std::pow(maxX - minX,2) +
                         std::pow(maxY - minY,2));
    circle.SetCircleRadius(D / 2);*/
    circle.SetCircleRadius(R);
}
void LimitCoordinate::LoopMinimumBox(const DefLoop&loop,double &xSpan,double 
&ySpan)
{
    double minX,maxX,minY,maxY;
    LoopLimitCoordinate(loop,minX,maxX,minY,maxY);    xSpan = maxX - minX;
    ySpan = maxY - minY;
}
bool LimitCoordinate::IsLoopMinBoxMeetLenth(const DefLoop&loop,double len,
                                            bool isGreater)
{
    double xSpan,ySpan;
    LoopMinimumBox(loop,xSpan,ySpan);
    if(isGreater)
        return 2 * (xSpan + ySpan) >= len;
    return 2 * (xSpan + ySpan) <= len;
}
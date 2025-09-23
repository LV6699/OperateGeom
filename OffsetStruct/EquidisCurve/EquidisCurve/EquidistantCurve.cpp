#include "EquidistantCurve.h"
#if DebugModel
#pragma optimize("", off)
#endif
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sys/stat.h>
using std::vector;
using std::string;
using std::fstream;
using std::ios;
void EquidistantCurve::EquidistantArc(const DefArc& oriArc,
                                      const double& offsetVal,
                                      const ProfType loopRot,
                                      bool isSetRadius,
                                      bool &isExiEquArc,
                                      DefArc& equArc)
{
    isExiEquArc = false;
    int sym = loopRot == OUTSIDEPROFILE ? 1 : -1;
    Point SP =oriArc.ArcStartPt();
    Point EP =oriArc.ArcEndPt();
    Point CP =oriArc.ArcCenterPt();
    double R = oriArc.ArcRadius();
    if(isSetRadius){
        double R1 = std::sqrt(pow(SP.X() - CP.X(), 2) +
                              std::pow(SP.Y() - CP.Y(), 2));
        double R2 = std::sqrt(pow(EP.X() - CP.X(), 2) +
                              std::pow(EP.Y() - CP.Y(), 2));
        R = (R1 + R2) / 2;
    }
    double R_ = oriArc._arcRot == CLOCKWISE ? R - offsetVal * sym :
                                               R + offsetVal * sym;
    if(R_ < Oft_CircleVal_Err){
        return;
    }
    double startAgl = std::atan2((SP.Y() - CP.Y()),(SP.X() - CP.X()));
    double endAgl = std::atan2((EP.Y() - CP.Y()),(EP.X() - CP.X()));
    equArc._arcSp._x = CP.X() + R_ * std::cos(startAgl);
    equArc._arcSp._y = CP.Y() + R_ * std::sin(startAgl);
    equArc._arcEp._x = CP.X() + R_ * std::cos(endAgl);
    equArc._arcEp._y = CP.Y() + R_ * std::sin(endAgl);
    equArc._arcCp = CP;
    equArc._arcR = R_;
    equArc._arcRot = oriArc._arcRot;
    isExiEquArc = true;
}
void EquidistantCurve::EquidistantSegment(const Point& orilSp,
                                          const Point& orilEp,
                                          const double& offsetVal,
                                          const ProfType prof,
                                          Point& lSp, Point& lEp)
{
    double sym = prof == OUTSIDEPROFILE ? 1 : -1;    double spx = orilSp.X();
    double spy = orilSp.Y();
    double epx = orilEp.X();
    double epy = orilEp.Y();
    TupleQuality tuple;
    if(tuple.IsHorizontalLine(orilSp,orilEp,Base_LineHor_Err)){
        epy = spy;
    }
    if(tuple.IsVerticalLine(orilSp,orilEp,Base_LineVer_Err)){
        epx = spx;
    }
    double deltaX = epx - spx;
    double deltaY = epy - spy;
    double A = deltaY;
    double B = -deltaX;
    double C1 = epx * spy - spx * epy;
    if (A == 0 && B != 0) {
        double yResult;
        if (deltaX > 0){
            yResult = -C1 / B - sym * offsetVal;
        }
        else{
            yResult = -C1 / B + sym * offsetVal;
        }
        lSp.SetX(spx);
        lSp.SetY(yResult);
        lEp.SetX(epx);
        lEp.SetY(yResult);
    }
    else if (A != 0 && B == 0){
        double xResult;
        if (deltaY > 0){
            xResult = orilEp.X() + sym * offsetVal;
        }
        else{
            xResult = orilEp.X() - sym * offsetVal;
        }
        lSp.SetX(xResult);
        lSp.SetY(spy);
        lEp.SetX(xResult);
        lEp.SetY(epy);
    }
    else{
#if 1
        ///direction
        double dir[2] = {orilEp.X()- orilSp.X(),
                         orilEp.Y() - orilSp.Y()};
        ///normalizeVector
        double a,b;
        double length = std::sqrt(dir[0]*dir[0] + dir[1]*dir[1]);
        double normalize[2] = {dir[0]/length, dir[1]/length};
        if(prof == OUTSIDEPROFILE){
            a = normalize[1];
            b = -normalize[0];
        }
        else {
            a = -normalize[1];
            b = normalize[0];
        }
        lSp.SetX(orilSp.X() + offsetVal * a);
        lSp.SetY(orilSp.Y() + offsetVal * b);        lEp.SetX(orilEp.X() + offsetVal * a);
        lEp.SetY(orilEp.Y() + offsetVal * b);
#else
        double vaule = sym * offsetVal;
        double delta = deltaY / deltaX;
        double k = std::atan(delta);
        if(deltaX > 0 && deltaY > 0){
            lSp.SetX(spx + vaule * std::abs(sin(k)));
            lSp.SetY(spy - vaule * std::abs(cos(k)));
            lEp.SetX(epx + vaule * std::abs(sin(k)));
            lEp.SetY(epy - vaule * std::abs(cos(k)));
        }
        else if(deltaX > 0 && deltaY < 0){
            lSp.SetX(spx - vaule * std::abs(sin(k)));
            lSp.SetY(spy - vaule * std::abs(cos(k)));
            lEp.SetX(epx - vaule * std::abs(sin(k)));
            lEp.SetY(epy - vaule * std::abs(cos(k)));
        }
        else if(deltaX < 0 && deltaY < 0){
            lSp.SetX(spx - vaule * std::abs(sin(k)));
            lSp.SetY(spy + vaule * std::abs(cos(k)));
            lEp.SetX(epx - vaule * std::abs(sin(k)));
            lEp.SetY(epy + vaule * std::abs(cos(k)));
        }
        else {  /*(deltaX > 0 && deltaY > 0)*/
            lSp.SetX(spx + vaule * std::abs(sin(k)));
            lSp.SetY(spy + vaule * std::abs(cos(k)));
            lEp.SetX(epx + vaule * std::abs(sin(k)));
            lEp.SetY(epy + vaule * std::abs(cos(k)));
        }
        lSp.SetZ(orilSp.Z());
        lEp.SetZ(orilEp.Z());
#endif
    }
    if(tuple.IsHorizontalLine(orilSp,orilEp,Base_LineHor_Err)){
        lEp.SetY(lSp.Y());
    }
    if(tuple.IsVerticalLine(orilSp,orilEp,Base_LineVer_Err)){
        lEp.SetX(lSp.X());
    }
}
void EquidistantCurve::EquidistantCircle(const Point& cp,double radius,
                                         ProfType proType,double offsetVal,
                                         DefCircle& circle)
{
    double R = proType == OUTSIDEPROFILE ?
                (radius - offsetVal) : (radius + offsetVal);
    if(R < Oft_CircleVal_Err){
        return;
    }
    circle.SetCircleCenter(cp);
    circle.SetCircleRadius(R);
}
bool EquidistantCurve::IsExistEquElement(const DefElem& ele,
                                         ProfType proType,
                                         double value)
{
    if(proType == WITHINPROFILE || ele._elemType == LINETYPE){
        return true;    }
    if(ele._elemType == ARCTYPE){
        DefArc arc = ele._arc;
        CheckGeomInt().JudgeArcRadius(arc);
        if(arc._arcR >= (value + Oft_CircleVal_Err)){
            return true;
        }
    }
    else if(ele._elemType == CIRCLETYPE){
        if(ele._circle._cirR >= (value + Oft_CircleVal_Err)){
            return true;
        }
    }
    return false;
}
void EquidistantCurve::EquidistantElement(const DefElem& ele,
                                          double offsetVal,
                                          const ProfType prof,
                                          bool& isExiEquiEle,
                                          DefElem& equEle)
{
    if(ele._elemType == LINETYPE){
        EquidistantSegment(ele._segment._segSp,
                           ele._segment._segEp,
                           offsetVal,prof,
                           equEle._segment._segSp,
                           equEle._segment._segEp);
        equEle._elemType = LINETYPE;
        isExiEquiEle = true;
    }
    else if(ele._elemType == ARCTYPE){
        EquidistantArc(ele._arc,offsetVal,prof,true,
                       isExiEquiEle,equEle._arc);
        equEle._elemType = ARCTYPE;
    }
    else if(ele._elemType == CIRCLETYPE){
        DefCircle circle;
        EquidistantCircle(ele._circle.CircleCenter(),
                          ele._circle.CircleRadius(),
                          prof,offsetVal,circle);
        equEle.SetElemCircle(circle);
    }
}
void EquidistantCurve::SpecifyChangeRadiusArc(const DefArc& arcData,
                                              double val,bool isLarger,
                                              DefArc &arc)
{
    arc._arcCp = arcData._arcCp;
    arc._arcRot = arcData._arcRot;
    double R = arcData._arcR;
    R = isLarger == true ? R + val : R - val;
    arc._arcR = R;
    double startAgl = std::atan2((arcData._arcSp.Y() -
                                  arcData._arcCp.Y()),
                                 (arcData._arcSp.X() -
                                  arcData._arcCp.X()));
    double endAgl = std::atan2((arcData._arcEp.Y() -
                                arcData._arcCp.Y()),
                               (arcData._arcEp.X() -
                                arcData._arcCp.X()));
    arc._arcSp._x = arc._arcCp.X() + R * std::cos(startAgl);    arc._arcSp._y = arc._arcCp.Y() + R * std::sin(startAgl);
    arc._arcEp._x = arc._arcCp.X() + R * std::cos(endAgl);
    arc._arcEp._y = arc._arcCp.Y() + R * std::sin(endAgl);
}
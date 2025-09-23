#include "GeomIntersect.h"
#if DebugModel
#pragma optimize("", off)
#endif
void GeomIntersect::CalCircleCircleExiIntPnt(const Point& C1,
                                             const double& R1,
                                             const Point& C2,
                                             const double& R2,
                                             Point& interP1,
                                             Point& interP2)
{
    double x1 = C1.X(),x2 = C2.X(),y1 = C1.Y(),y2 = C2.Y();
    double d = std::sqrt(std::pow(x1 - x2,2) + std::pow(y1 - y2,2));
    double l = ((R1 * R1) - (R2 * R2) + (d * d)) / (2 * d);
    double temVal = (R1 * R1) - (l * l);
    if(temVal < 0){
        temVal = 0;
    }
    double h = std::sqrt(temVal);
    double c1 = l/d;
    double c2 = h/d;
    double interX1 = c1*(x2 - x1) + c2 * (y2-y1) + x1;
    double interY1 = c1*(y2 - y1) - c2 * (x2-x1) + y1;
    double interX2 = c1*(x2 - x1) - c2 * (y2-y1) + x1;
    double interY2 = c1*(y2 - y1) + c2 * (x2-x1) + y1;
    interP1 = {interX1,interY1};
    interP2 = {interX2,interY2};
}
void GeomIntersect::CalculateLineLineInt(const Point &l1Sp,
                                         const Point &l1Ep,
                                         const Point &l2Sp,
                                         const Point &l2Ep,
                                         bool &isExiIntPnt,
                                         Point &intPnt)
{
    isExiIntPnt = false;
    GeomCalculate gocal;
    double linePara1[3],linePara2[3];
    gocal.CalLineParameters(l1Sp,l1Ep,linePara1);
    gocal.CalLineParameters(l2Sp,l2Ep,linePara2);
    double A1 = linePara1[0],B1 = linePara1[1],C1 = linePara1[2];
    double A2 = linePara2[0],B2 = linePara2[1],C2 = linePara2[2];
    double disc = A1*B2 - A2*B1;
    if(disc == 0){
        isExiIntPnt = false;
        return;
    }
    intPnt._x = (B1*C2 - B2*C1) / (disc);
    intPnt._y = (C1*A2 - C2*A1) / (disc);
    isExiIntPnt = true;
    GeomIntSolve().SetHorVerLineIntPnt(l1Sp,l1Ep,intPnt);
    GeomIntSolve().SetHorVerLineIntPnt(l2Sp,l2Ep,intPnt);
}void GeomIntersect::CalculateLineParameterInt(double A1, double B1, double C1,
                                              double A2, double B2, double C2,
                                              bool &isExiInt,Point &P)
{
    double disc = A1*B2 - A2*B1;
    if(std::abs(disc) < PreErr5_10){
        isExiInt = false;
        return;
    }
    P.SetX((B1*C2 - B2*C1) / disc);
    P.SetY((C1*A2 - C2*A1) / disc);
    isExiInt = true;
}
void GeomIntersect::LineParameterCircleInt(double linePara[3],const Point& cp,
double R,double tanPre, double intPre,GeomIntRes& intRes)
{
    double x0 = cp.X();
    double y0 = cp.Y();
    double A = linePara[0];
    double B = linePara[1];
    double C = linePara[2];
    double a = 1 + (A * A) / (B * B);
    double b = 2 * (A * C / (B * B) - x0 - (A * y0) / B);
    double c = x0 * x0 + (C / B + y0) * (C / B + y0) - R * R;
    double discriminant = b * b - 4 * a * c + tanPre;
    if (discriminant < 0) {
        return;
    }
    else if (discriminant == 0) {
        double x = -b / (2 * a);
        double y = -(A * x + C) / B;
        intRes._intPnt1.SetX(x);
        intRes._intPnt1.SetY(y);
        intRes._intPnt2.SetX(x);
        intRes._intPnt2.SetY(y);
        intRes._intNum = 1;
    }
    else {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double y1 = -(A * x1 + C) / B;
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        double y2 = -(A * x2 + C) / B;
        intRes._intPnt1.SetX(x1);
        intRes._intPnt1.SetY(y1);
        intRes._intPnt2.SetX(x2);
        intRes._intPnt2.SetY(y2);
        intRes._intNum = 2;
    }
    GeomCalculate gocal;
    double distance = gocal.TwoPointDistance(intRes._intPnt1,
                                             intRes._intPnt2);
    if(distance <= intPre){
        Point intPnt;
        gocal.TwoPointMidpoint(intRes._intPnt1,
                               intRes._intPnt2,intPnt);
        intRes._intPnt1 = intPnt;
        intRes._intPnt2 = intPnt;
        intRes._intNum = 1;
    }
    intRes._isExistInt = true;}
void GeomIntersect::CalculateLineCircleInt(const Point& lSp,
                                           const Point& lEp,
                                           const Point& cp,
                                           double R,
                                           double tanPre,
                                           double intPre,
                                           GeomIntRes& intRes)
{
    GeomCalculate gocal;
    double linePara[3];
    gocal.CalLineParameters(lSp, lEp,linePara);
    double A = linePara[0];
    double B = linePara[1];
    double C = linePara[2];
    if(A == 0 && B == 0){
        return;
    }
    CheckGeomInt cheInt;
    bool isExiInt = false;
    cheInt.IsLineCircleIntersect(lSp,lEp,cp,R,intPre,isExiInt);
    if(!isExiInt){
        return;
    }
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
    if(delta < 0){
        delta = 0;
    }
    double int_x1,int_y1,int_x2,int_y2;
    if(delta > 0){
        int_x1 = (-b + std::sqrt(delta)) / (2 * a);
        int_x2 = (-b - std::sqrt(delta)) / (2 * a);
        int_y1 = (-A * int_x1 - C) / B;
        int_y2 = (-A * int_x2 - C) / B;
    }
    else{
        int_x1 = int_x2 = -b / (2 * a);
        int_y1 = int_y2 = (-A * int_x2 - C) / B;
    }
    intRes._intPnt1._x = int_x1;
    intRes._intPnt1._y = int_y1;
    intRes._intPnt2._x = int_x2;
    intRes._intPnt2._y = int_y2;
#elif 1
    Point vec = lEp - lSp;
    vec.SetZ(0);
    Point projPnt = lSp + vec * ((cp - lSp) * vec)/ (vec.PointSqrt2D());
    ///gocal.PointOnLineProjection(cp,lSp,lEp,projPnt);
    double tem = R * R - (projPnt - cp).PointSqrt2D();
    if(tem < 0){
        tem = 0;
    }
    double base = std::sqrt(tem);/**        if(base <= tanPre){
            intRes._intPnt1 = projPnt;
            intRes._intPnt2 = projPnt;
        }
        else {
            Point unitVec = vec / vec.PntAbs();
            intRes._intPnt1 = projPnt + unitVec * base;
            intRes._intPnt2 = projPnt - unitVec * base;
        }*/
    Point unitVec = vec / vec.PointAbs2D();
    intRes._intPnt1 = projPnt + unitVec * base;
    intRes._intPnt2 = projPnt - unitVec * base;
#else
    Point Q;
    gocal.PointOnLineProjection(cp,lSp,lEp,Q);
    double toLDis;
    gocal.PointToLineDistance(lSp,lEp,cp,toLDis);
    double slop = std::atan2(lEp.Y() - lSp.Y(),
                             lEp.X() - lSp.X());
    double base = (R * R) - (toLDis * toLDis);
    if(base < 0){
        base = 0;
    }
    base = std::sqrt(base);
    double int_x1 = Q.X() - base * std::cos(slop);
    double int_y1 = Q.Y() - base * std::sin(slop);
    double int_x2 = Q.X() + base * std::cos(slop);
    double int_y2 = Q.Y() + base * std::sin(slop);
    intRes._intPnt1 = {int_x1,int_y1};
    intRes._intPnt2 = {int_x2,int_y2};
#endif
    intRes._intNum = 2;
    double lcDis;
    gocal.PointToLineDistance(lSp,lEp,cp,lcDis);
    if(std::abs(lcDis - R) < tanPre){
        intRes._intNum = 1;
        Point intPnt((intRes._intPnt1.X() + intRes._intPnt2.X()) / 2,
                     (intRes._intPnt1.Y() + intRes._intPnt2.Y()) / 2);
        intRes._intPnt1 = intPnt;
        intRes._intPnt2 = intPnt;
    }
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
    GeomIntSolve().SetHorVerLineIntPnt(lSp,lEp,intRes._intPnt1);
    GeomIntSolve().SetHorVerLineIntPnt(lSp,lEp,intRes._intPnt2);
    /**
    double dis1,dis2;
    gocal.TwoPointDistance(cp,intRes._intPnt1,dis1);
    gocal.TwoPointDistance(cp,intRes._intPnt2,dis2);
    double pre1 = std::abs(dis1 - R);
    double pre2 = std::abs(dis2 - R);
    double sigDis1,sigDis2;
    gocal.PointToLineDistance(lSp,lEp,intRes._intPnt1,sigDis1);
    gocal.PointToLineDistance(lSp,lEp,intRes._intPnt2,sigDis2);
    double err = pre1 + pre2 + sigDis1 + sigDis2;*/
}
void GeomIntersect::CalculateLineArcInt(const Point& lSp,
                                        const Point& lEp,
                                        DefArc arc,double tanPre,
                                        double endptPre,double intPre,
                                        GeomIntRes &intRes)
{
    GeomIntSolve().JudgeArcRadius(arc);    PointOnElement pton;
    bool isSpOn = false,isEpOn = false;
    isSpOn = pton.IsPointOnLine(lSp,lEp,arc._arcSp,endptPre);
    isEpOn = pton.IsPointOnLine(lSp,lEp,arc._arcEp,endptPre);
    Point onEndpnt;
    bool isEndpntOn = false;
    GeomIntSolve().ExicuteEndpointOnTuple(isSpOn,isEpOn,arc._arcSp,arc._arcEp,
                                          isEndpntOn,onEndpnt,intRes);
    if(isSpOn && isEpOn){
        return;
    }
    GeomIntRes intRes_;
    CalculateLineCircleInt(lSp,lEp,arc._arcCp,arc._arcR,
                           tanPre,intPre,intRes_);
    if(!intRes_._isExistInt){
        return;
    }
    bool isPnt1OnArc = false,isPnt2OnArc = false;
    pton.IsPointOnArc(arc,intRes_._intPnt1,endptPre,
                      intPre,false,isPnt1OnArc);
    if(intRes_._intNum == 2){
        pton.IsPointOnArc(arc,intRes_._intPnt2,endptPre,
                          intPre,false,isPnt2OnArc);
    }
    if(!isPnt1OnArc && !isPnt2OnArc){
        return;
    }
    if(isPnt1OnArc && !isPnt2OnArc){
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt1;
        intRes._intNum = 1;
    }
    else if(!isPnt1OnArc && isPnt2OnArc){
        intRes._intPnt1 = intRes_._intPnt2;
        intRes._intPnt2 = intRes_._intPnt2;
        intRes._intNum = 1;
    }
    else{
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt2;
        intRes._intNum = 2;
    }
    GeomIntSolve().HandleEndpointOnTuple(isEndpntOn,onEndpnt,intRes_,intRes);
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
}
void GeomIntersect::CalculateCircleCircleInt(const Point &cp1,double R1,
                                             const Point &cp2,double R2,
                                             double tanPre,double intPre,
                                             GeomIntRes &intRes)
{
    GeomCalculate gocal;
    CheckGeomInt intch;
    bool isExiCCInt = false;
    intch.IsCircleCircleIntersect(cp1,R1,cp2,R2,intPre,isExiCCInt);
    if(!isExiCCInt){
        return;
    }
    double cpDis;
    gocal.TwoPointDistance(cp1,cp2,cpDis);
    intRes._intNum = 2;
    double maxR = (R1 >= R2) ? R1 : R2;    double minR = (R1 < R2) ? R1 : R2;
    double dis1 = std::abs(cpDis - R1 - R2);
    double dis2 = std::abs(maxR - minR - cpDis);
    if(dis1 <= tanPre || dis2 <= tanPre){
        intRes._intNum = 1;
    }
    CalCircleCircleExiIntPnt(cp1,R1,cp2,R2,intRes._intPnt1,intRes._intPnt2);
    if(intRes._intNum == 1){
        Point intPnt((intRes._intPnt1.X() + intRes._intPnt2.X()) / 2,
                     (intRes._intPnt1.Y() + intRes._intPnt2.Y()) / 2);
        intRes._intPnt1 = intPnt;
        intRes._intPnt2 = intPnt;
    }
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
}
void GeomIntersect::CalculateSegSegIntersect(const Point& lSp1,
                                             const Point& lEp1,
                                             const Point& lSp2,
                                             const Point& lEp2,
                                             bool isCalParall,
                                             double endptPre,
                                             double intPre,
                                             GeomIntRes& intRes)
{
    ElemBoxIntersect boxint;
    if(!boxint.IsRectangleExistOverlap(lSp1,lEp1,lSp2,lEp2,
                                       intPre * ErrTimes_10))
        return;
    TupleQuality tup;ElemCalculate numcal;PointOnElement pton;
    if(!isCalParall && tup.IsLineLineParallel(lSp1,lEp1,lSp2,
                                              lEp2,PreErr5_6)){
        return;
    }
    bool isL1SpOnL2 = false,isL1EpOnL2 = false;
    pton.IsPointOnSegment(lSp2,lEp2,lSp1,endptPre,
                          endptPre,true,isL1SpOnL2);
    pton.IsPointOnSegment(lSp2,lEp2,lEp1,endptPre,
                          endptPre,true,isL1EpOnL2);
    vector<Point>vIntPnt;
    if(isL1SpOnL2){
        vIntPnt.push_back(lSp1);
    }
    if(isL1EpOnL2){
        vIntPnt.push_back(lEp1);
    }
    bool isL2SpOnL1 = false,isL2EpOnL1 = false;
    pton.IsPointOnSegment(lSp1,lEp1,lSp2,endptPre,
                          endptPre,true,isL2SpOnL1);
    pton.IsPointOnSegment(lSp1,lEp1,lEp2,endptPre,
                          endptPre,true,isL2EpOnL1);
    if(isL2SpOnL1){
        vIntPnt.push_back(lSp2);
    }
    if(isL2EpOnL1){
        vIntPnt.push_back(lEp2);
    }
    numcal.DeduplicationPoint(vIntPnt,PreErr5_10);
    if(vIntPnt.empty()){
        Point intPnt;
        bool isExiLLInt = false;
        CalculateLineLineInt(lSp1,lEp1,lSp2,lEp2,isExiLLInt,intPnt);        if(isExiLLInt){
            bool isIntPntOnL1 = false,isIntPntOnL2 = false;
            pton.IsPointOnSegment(lSp1,lEp1,intPnt,intPre,
                                  intPre,false,isIntPntOnL1);
            pton.IsPointOnSegment(lSp2,lEp2,intPnt,intPre,
                                  intPre,false,isIntPntOnL2);
            if(isIntPntOnL1 && isIntPntOnL2){
                intRes._intPnt1 = intPnt;
                intRes._intPnt2 = intPnt;
                intRes._intNum = 1;
                intRes._isExistInt = true;
                intRes._isExiGeoInt = true;
                return;
            }
        }
        return;
    }
    int intNum = vIntPnt.size();
    if(intNum == 2){
        intRes._intPnt1 = vIntPnt[0];
        intRes._intPnt2 = vIntPnt[1];
    }
    else{
        intRes._intPnt1 = vIntPnt[0];
        intRes._intPnt2 = vIntPnt[0];
    }
    intRes._isExistInt = true;
    intRes._intNum = intNum > 2 ? 2 : intNum;
    intRes._isExiGeoInt = !tup.IsLineLineParallel(lSp1,lEp1,lSp2,
                                                  lEp2,PreErr5_6);
}
void GeomIntersect::CalculateSegCircleIntersect(const Point& lSp,
                                                const Point& lEp,
                                                const Point& cp,
                                                double R,double tanPre,
                                                double endptPre,
                                                double intPre,
                                                GeomIntRes& intRes)
{
    PointOnElement pton;
    bool isSpOn = false,isEpOn = false;
    pton.IsPointOnCircle(lSp,cp,R,endptPre,isSpOn);
    pton.IsPointOnCircle(lEp,cp,R,endptPre,isEpOn);
    Point onEndpnt;
    bool isEndpntOn = false;
    GeomIntSolve().ExicuteEndpointOnTuple(isSpOn,isEpOn,lSp,lEp,
                                          isEndpntOn,onEndpnt,intRes);
    if(isSpOn && isEpOn){
        return;
    }
    GeomIntRes intRes_;
    CalculateLineCircleInt(lSp,lEp,cp,R,tanPre,intPre,intRes_);
    if(!intRes_._isExistInt && !isEndpntOn){
        return;
    }
    ElemBoxIntersect boxint;
    if(!boxint.IsIntResInsideBox(lSp,lEp,intRes_,intPre * 10)){
        return;
    }
    bool isPnt1OnSig = false,isPnt2OnSig = false;
    pton.IsPointOnSegment(lSp,lEp,intRes_._intPnt1,
                          intPre,intPre,false,isPnt1OnSig);    if(intRes_._intNum == 2){
        pton.IsPointOnSegment(lSp,lEp,intRes_._intPnt2,
                              intPre,intPre,false,isPnt2OnSig);
    }
    if(!isPnt1OnSig && !isPnt2OnSig && !isEndpntOn){
        return;
    }
    if(isPnt1OnSig && !isPnt2OnSig){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt1;
    }
    else if(!isPnt1OnSig && isPnt2OnSig){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt2;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    else{
        intRes._intNum = 2;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    GeomIntSolve().HandleEndpointOnTuple(isEndpntOn,onEndpnt,intRes_,intRes);
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
}
void GeomIntersect::CalculateArcCircleInt(DefArc arc,
                                          const Point &cp,
                                          double R,double tanPre,
                                          double endptPre,
                                          double intPre,
                                          GeomIntRes &intRes)
{
    GeomIntSolve().JudgeArcRadius(arc);
    PointOnElement pton;
    bool isSpOn = false,isEpOn = false;
    pton.IsPointOnCircle(arc._arcSp,cp,R,endptPre,isSpOn);
    pton.IsPointOnCircle(arc._arcEp,cp,R,endptPre,isEpOn);
    Point onEndpnt;
    bool isEndpntOn = false;
    GeomIntSolve().ExicuteEndpointOnTuple(isSpOn,isEpOn,arc._arcSp,arc._arcEp,
                                          isEndpntOn,onEndpnt,intRes);
    if(isSpOn && isEpOn){
        return;
    }
    int intNum_ = 0;
    GeomIntRes intRes_;
    CalculateCircleCircleInt(arc._arcCp,arc._arcR,cp,R,
                             tanPre,intPre,intRes_);
    if(!intRes_._isExistInt && !isEndpntOn){
        return;
    }
    bool isPnt1OnArc = false,isPnt2OnArc = false;
    pton.IsPointOnArc(arc,intRes_._intPnt1,endptPre,
                      intPre,false,isPnt1OnArc);
    if(intNum_ == 2){
        pton.IsPointOnArc(arc,intRes_._intPnt2,endptPre,
                          intPre,false,isPnt2OnArc);
    }
    if(!isPnt1OnArc && !isPnt2OnArc){
        return;
    }
    if(isPnt1OnArc && isPnt2OnArc){        intRes._intNum = 2;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    else if(isPnt1OnArc && !isPnt2OnArc){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt1;
    }
    else if(!isPnt1OnArc && isPnt2OnArc){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt2;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    GeomIntSolve().HandleEndpointOnTuple(isEndpntOn,onEndpnt,intRes_,intRes);
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
}
void GeomIntersect::CalculateSegmentArcInt(const Point& lSp,
                                           const Point& lEp,
                                           DefArc arc,double tanPre,
                                           double endptPre,
                                           double intPre,
                                           GeomIntRes &intRes)
{
    HandExtreInt endInt;
    GeomIntRes intRes__;
    GeomIntSolve().JudgeArcRadius(arc);
    endInt.SegArcIntersectEndpoint(lSp,lEp,arc,endptPre,intRes__);
    if(intRes__._isExistInt && intRes__._intNum == 2){
        intRes._intNum = 2;
        intRes._isExistInt = true;
        intRes._intPnt1 = intRes__._intPnt1;
        intRes._intPnt2 = intRes__._intPnt2;
        return;
    }
    GeomIntRes intRes_;
    CalculateLineCircleInt(lSp,lEp,arc._arcCp,arc._arcR,
                           tanPre,intPre,intRes_);
    if(!intRes_._isExistInt && !intRes__._isExistInt){
        return;
    }
    ElemBoxIntersect boxint;
    if(!intRes__._isExistInt && intRes_._isExistInt &&
            !boxint.IsIntResInsideBox(lSp,lEp,intRes_,
                                      intPre * ErrTimes_10)){
        return;
    }
    PointOnElement pton;
    bool isPnt1OnSig = false,isPnt2OnSig = false,isPnt1OnArc = false,
            isPnt2OnArc = false,isPnt1Val = false,isPnt2Val = false;
    pton.IsPointOnSegment(lSp,lEp,intRes_._intPnt1,
                          intPre,intPre,false,isPnt1OnSig);
    if(isPnt1OnSig){
        pton.IsPointOnArc(arc,intRes_._intPnt1,endptPre,
                          intPre,false,isPnt1OnArc);
        if(isPnt1OnArc){
            isPnt1Val = true;
        }
    }
    if(intRes_._intNum == 2){
        pton.IsPointOnSegment(lSp,lEp,intRes_._intPnt2,endptPre,                              intPre,false,isPnt2OnSig);
        if(isPnt2OnSig){
            pton.IsPointOnArc(arc,intRes_._intPnt2,endptPre,
                              intPre,false,isPnt2OnArc);
            if(isPnt2OnArc){
                isPnt2Val = true;
            }
        }
    }
    if(!isPnt1Val && !isPnt2Val && !intRes__._isExistInt){
        return;
    }
    if(isPnt1Val && !isPnt2Val){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt1;
    }
    else if(!isPnt1Val && isPnt2Val){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt2;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    else if(isPnt1Val && isPnt2Val){
        intRes._intNum = 2;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    GeomIntSolve intsol;
    intsol.HandleEndpointOnTuple(intRes__._isExistInt,
                                 intRes__._intPnt1,
                                 intRes_,intRes);
    if(intRes._intNum == 1){
        intsol.IntersectPointNotOnArc(arc,intRes._intPnt1);
        intRes._intPnt2 = intRes._intPnt1;
    }
    else{
        intsol.IntersectPointNotOnArc(arc,intRes._intPnt1);
        intsol.IntersectPointNotOnArc(arc,intRes._intPnt2);
    }
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
}
void GeomIntersect::CalculateArcArcInt(DefArc arc1,DefArc arc2,
                                       double tanPre,double endptPre,
                                       double intPre,GeomIntRes& intRes)
{
    GeometryLogic golg;PointOnElement pton;
    GeomIntRes intRes__;
    golg.IsTwoArcEndPntSame(arc1,arc2,endptPre,intRes__);
    if(intRes__._isExistInt && intRes__._intNum == 2){
        intRes._intNum = 2;
        intRes._isExistInt = true;
        intRes._isExiGeoInt = true;
        intRes._intPnt1 = intRes__._intPnt1;
        intRes._intPnt2 = intRes__._intPnt2;
        return;
    }
    GeomIntSolve().JudgeArcRadius(arc1);
    GeomIntSolve().JudgeArcRadius(arc2);
    GeomIntRes intRes_;
    CalculateCircleCircleInt(arc1._arcCp,arc1._arcR,arc2._arcCp,
                             arc2._arcR,tanPre,intPre,intRes_);    if(!intRes_._isExistInt && !intRes__._isExistInt){
        return;
    }
    bool isPnt1OnArc1 = false,isPnt1OnArc2 = false,isPnt2OnArc1 = false,
            isPnt2OnArc2 = false,isPnt1Val = false,isPnt2Val = false;
    pton.IsPointOnArc(arc1,intRes_._intPnt1,endptPre,
                      intPre,false,isPnt1OnArc1);
    if(isPnt1OnArc1){
        pton.IsPointOnArc(arc2,intRes_._intPnt1,endptPre,
                          intPre,false,isPnt1OnArc2);
        if(isPnt1OnArc2){
            isPnt1Val = true;
        }
    }
    if(intRes_._intNum == 2){
        pton.IsPointOnArc(arc1,intRes_._intPnt2,endptPre,
                          intPre,false,isPnt2OnArc1);
        if(isPnt2OnArc1){
            pton.IsPointOnArc(arc2,intRes_._intPnt2,endptPre,
                              intPre,false,isPnt2OnArc2);
            if(isPnt2OnArc2){
                isPnt2Val = true;
            }
        }
    }
    if(!isPnt1Val && !isPnt2Val && !intRes__._isExistInt){
        return;
    }
    if(isPnt1Val && !isPnt2Val){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt1;
    }
    else if(!isPnt1Val && isPnt2Val){
        intRes._intNum = 1;
        intRes._intPnt1 = intRes_._intPnt2;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    else if(isPnt1Val && isPnt2Val){
        intRes._intNum = 2;
        intRes._intPnt1 = intRes_._intPnt1;
        intRes._intPnt2 = intRes_._intPnt2;
    }
    GeomIntSolve().HandleEndpointOnTuple(intRes__._isExistInt,intRes__._intPnt1,
                                         intRes_,intRes);
    if(intRes._isExistInt){
        GeomIntSolve().IntersectPointNotOnArc(arc1,intRes._intPnt1);
        GeomIntSolve().IntersectPointNotOnArc(arc2,intRes._intPnt1);
        if(intRes._intNum == 1){
            intRes._intPnt2 = intRes._intPnt1;
        }
        else {
            GeomIntSolve().IntersectPointNotOnArc(arc1,intRes._intPnt2);
            GeomIntSolve().IntersectPointNotOnArc(arc2,intRes._intPnt2);
        }
    }
    intRes._isExistInt = true;
    intRes._isExiGeoInt = true;
}
void GeomIntersect::CalculateCurveIntersect(const DefElem& ele1,
                                            const DefElem& ele2,
                                            bool isCalParallel,                                            double tanPre,
                                            double endptPre,
                                            double intPre,
                                            GeomIntRes& intRes)
{
    switch (ele1._elemType) {
    case LINETYPE:
        switch (ele2._elemType) {
        case LINETYPE:
            CalculateSegSegIntersect(ele1._segment._segSp,
                                     ele1._segment._segEp,
                                     ele2._segment._segSp,
                                     ele2._segment._segEp,
                                     isCalParallel,endptPre,
                                     intPre,intRes);
            break;
        case ARCTYPE:
            CalculateSegmentArcInt(ele1._segment._segSp,
                                   ele1._segment._segEp,
                                   ele2._arc,tanPre,
                                   endptPre,intPre,intRes);
            break;
        case CIRCLETYPE:
            CalculateSegCircleIntersect(ele1._segment._segSp,
                                        ele1._segment._segEp,
                                        ele2._circle._cirCp,
                                        ele2._circle._cirR,tanPre,
                                        endptPre,intPre,intRes);
            break;
        }
        break;
    case ARCTYPE:
        switch (ele2._elemType) {
        case LINETYPE:
            CalculateSegmentArcInt(ele2._segment._segSp,
                                   ele2._segment._segEp,
                                   ele1._arc,tanPre,
                                   endptPre,intPre,intRes);
            break;
        case ARCTYPE:
            CalculateArcArcInt(ele1._arc,ele2._arc,tanPre,
                               endptPre,intPre,intRes);
            break;
        case CIRCLETYPE:
            CalculateArcCircleInt(ele1._arc,ele2._circle._cirCp,
                                  ele2._circle._cirR,tanPre,
                                  endptPre,intPre,intRes);
            break;
        }
        break;
    case CIRCLETYPE:
        switch (ele2._elemType) {
        case LINETYPE:
            CalculateSegCircleIntersect(ele2._segment._segSp,
                                        ele2._segment._segEp,
                                        ele1._circle._cirCp,
                                        ele1._circle._cirR,tanPre,
                                        endptPre,intPre,intRes);
            break;
        case ARCTYPE:
            CalculateArcCircleInt(ele2._arc,ele1._circle._cirCp,
                                  ele1._circle._cirR,tanPre,
                                  endptPre,intPre,intRes);
            break;        case CIRCLETYPE:
            CalculateCircleCircleInt(ele1._circle._cirCp,
                                     ele1._circle._cirR,
                                     ele2._circle._cirCp,
                                     ele2._circle._cirR,tanPre,
                                     intPre,intRes);
            break;
        }
        break;
    default:
        break;
    }
}
void GeomIntersect::CalculateEleGeomIntersect(const DefElem &ele1,
                                              const DefElem &ele2,
                                              double tanPre,
                                              double intPre,
                                              GeomIntRes& intRes)
{
    if(ele1._elemType == LINETYPE){
        Point l1Sp = ele1._segment._segSp;
        Point l1Ep = ele1._segment._segEp;
        if(ele2._elemType == LINETYPE){
            Point l2Sp = ele2._segment._segSp;
            Point l2Ep = ele2._segment._segEp;
            CalculateLineLineInt(l1Sp,l1Ep,l2Sp,l2Ep,
                                 intRes._isExistInt,
                                 intRes._intPnt1);
            if(intRes._isExistInt){
                intRes._intPnt2 = intRes._intPnt1;
                intRes._intNum = 1;
            }
        }
        else if(ele2._elemType == ARCTYPE){
            DefArc arcData2 = ele2._arc;
            CalculateLineCircleInt(l1Sp,l1Ep,arcData2._arcCp,
                                   arcData2._arcR,tanPre,
                                   intPre,intRes);
        }
        else if(ele2._elemType == CIRCLETYPE){
            Point cirCdt = ele2._circle._cirCp;
            double R = ele2._circle._cirR;
            CalculateLineCircleInt(l1Sp,l1Ep,cirCdt,R,tanPre,
                                   intPre,intRes);
        }
    }
    else if(ele1._elemType == ARCTYPE){
        DefArc arcData1 = ele1._arc;
        if(ele2._elemType == LINETYPE){
            Point l2Sp = ele2._segment._segSp;
            Point l2Ep = ele2._segment._segEp;
            CalculateLineCircleInt(l2Sp,l2Ep,arcData1._arcCp,
                                   arcData1._arcR,tanPre,
                                   intPre,intRes);
        }
        else if(ele2._elemType == ARCTYPE){
            DefArc arcData2 = ele2._arc;
            CalculateCircleCircleInt(arcData1._arcCp,                                     arcData1._arcR,
                                     arcData2._arcCp,
                                     arcData2._arcR,
                                     tanPre,intPre,intRes);
        }
        else if(ele2._elemType == CIRCLETYPE){
            Point cirCdt = ele2._circle._cirCp;
            double R = ele2._circle._cirR;
            CalculateCircleCircleInt(arcData1._arcCp,
                                     arcData1._arcR,
                                     cirCdt,R,tanPre,
                                     intPre,intRes);
        }
    }
    else if(ele1._elemType == CIRCLETYPE){
        Point cirCdt = ele1._circle._cirCp;
        double R = ele1._circle._cirR;
        if(ele2._elemType == LINETYPE){
            Point l2Sp = ele2._segment._segSp;
            Point l2Ep = ele2._segment._segEp;
            CalculateLineCircleInt(l2Sp,l2Ep,cirCdt,R,tanPre,
                                   intPre,intRes);
        }
        else if(ele2._elemType == ARCTYPE){
            DefArc arcData2 = ele2._arc;
            CalculateCircleCircleInt(cirCdt,R,arcData2._arcCp,
                                     arcData2._arcR,tanPre,
                                     intPre,intRes);
        }
        else if(ele2._elemType == CIRCLETYPE){
            Point cirCdt2 = ele2._circle._cirCp;
            double R2 = ele2._circle._cirR;
            CalculateCircleCircleInt(cirCdt,R,cirCdt2,R2,
                                     tanPre,intPre,intRes);
        }
    }
}
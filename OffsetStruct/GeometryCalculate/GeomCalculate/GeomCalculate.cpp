/**
  @author: lvdong
  @date: 2023-05
**/
#include "GeomCalculate.h"
#include<iostream>
#if 0
#pragma optimize("", off)
#endif
shared_ptr<ElemCalculate>go_num = make_shared<ElemCalculate>();
shared_ptr<GeometryLogic>go_logGo = make_shared<GeometryLogic>();
shared_ptr<PointInPolygon>go_inpol = make_shared<PointInPolygon>();
shared_ptr<GeomIntersect>go_goIn = make_shared<GeomIntersect>();
shared_ptr<ConnectArcRotDir>go_arcRot = make_shared<ConnectArcRotDir>();
shared_ptr<CalculateAngle>go_calAgl = make_shared<CalculateAngle>();
shared_ptr<MathCalculate>go_math = make_shared<MathCalculate>();
void GeomCalculate::SegmentAngle(const Point& lSp, const Point& lEp,
                                 double &angle)
{
    if(IsIdenticalPoint(lSp,lEp,PreErr_6))
        return;
    double x = lEp.X() - lSp.X();
    double y = lEp.Y() - lSp.Y();
    angle = std::atan2(y,x);
    angle = angle * _AlgRad;
    if(angle < 0){
        angle = angle + 360;
    }
    MathCalculate matcal;
    if(matcal.IsNumericEqual(angle,90,Base_LineVer_Err) ||
            matcal.IsNumericEqual(angle,270,Base_LineVer_Err)){
        if(lSp.Y() >= lEp.Y())
            angle = 270;
        else
            angle = 90;
    }
    else if(matcal.IsNumericEqual(angle,0,Base_LineHor_Err) ||
            matcal.IsNumericEqual(angle,180,Base_LineHor_Err) ||
            matcal.IsNumericEqual(angle,360,Base_LineHor_Err)){
        if(lSp.X() <= lEp.X())
            angle = 0;
        else
            angle = 180;
    }
}
double GeomCalculate::SegmentAngle3D(const Point sp,const Point& ep)
{
    double dx = ep.X() - sp.X();
    double dy = ep.Y() - sp.Y();
    double dz = ep.Z() - sp.Z();
    ///计算XY平面的投影长度
    double length_xy = std::sqrt(dx*dx + dy*dy);
    ///绝对值确保角度在0到π/2之间
    double angle = atan2(std::fabs(dz),length_xy);
    return angle * _AlgRad;
}void GeomCalculate::SegmentAngleWithXY(const Point& sp, const Point& ep,
                                       double& angle)
{
    /// 计算方向向量的分量
    double dx = ep.X() - sp.X();
    double dy = ep.Y() - sp.Y();
    double dz = ep.Z() - sp.Z();
    /// 计算水平长度（xy平面上的投影长度）
    double horLen = std::sqrt(dx * dx + dy * dy);
    /// 处理特殊情况：如果水平长度为 0，说明线段垂直于xy平面，夹角为90度
    if (horLen < PreErr_10) {
        angle = 90;
        return;
    }
    /// 计算夹角的弧度值
    double theta_rad = std::atan2(std::abs(dz),horLen);
    /// 将弧度转换为度数
    angle = theta_rad * _AlgRad;
}
double GeomCalculate::SegmentSlope(const Point& lSp,const Point& lEp)
{
    double deltaX = lEp.X() - lSp.X();
    double deltaY = lEp.Y() - lSp.Y ();
    if(std::abs(deltaX) < PreErr5_10){
        if(lEp.Y() >= lSp.Y ())
            return Max_Val;
        else
            return Min_Val;
    }
    if(std::abs(deltaY) < PreErr5_10)
        return 0;
    double slope = deltaY / deltaX;
    if(std::abs(slope) > 1 / PreErr_4){
        if(slope > 0)
            return Max_Val;
        else
            return Min_Val;
    }
    if(std::abs(slope) < PreErr_4)
        return 0;
    return slope;
}
void GeomCalculate::IsSigmentsParallel(const Point& l1Sp,
                                       const Point& l1Ep,
                                       const Point& l2Sp,
                                       const Point& l2Ep,
                                       bool& isPara)
{
    isPara = false;
    MathCalculate matcal;
    if(matcal.IsNumericEqual(l1Sp._x,l1Ep._x,PreErr_10) &&
            matcal.IsNumericEqual(l2Sp._x,l2Ep._x,PreErr_10)){
        isPara = true;
        return;
    }
    if(matcal.IsNumericEqual(l1Sp._y,l1Ep._y,PreErr_10) &&
            matcal.IsNumericEqual(l2Sp._y,l2Ep._y,PreErr_10)){
        isPara = true;
        return;
    }
    double para1[3],para2[3];
    CalLineParameters(l1Sp,l1Ep,para1);    CalLineParameters(l2Sp,l2Ep,para2);
    if(matcal.IsNumericEqual(para1[0]/para2[0],
                             para1[1]/para2[1],PreErr_10) &&
            matcal.IsNumericEqual(para1[1]/para2[1],
                                  para1[2]/para2[2],PreErr_10) == false){
        isPara = true;
        return;
    }
}
void GeomCalculate::TwoPointDistance(const Point& P1,
                                     const Point& P2,
                                     double& dis)
{
    dis = std::sqrt(std::pow(P2.X() - P1.X(),2) +
                    std::pow(P2.Y() - P1.Y(),2));
}
double GeomCalculate::TwoPointDistance(const Point& P1,
                                       const Point& P2)
{
    return std::sqrt(std::pow(P2.X() - P1.X(),2) +
                     std::pow(P2.Y() - P1.Y(),2));
}
double GeomCalculate::TwoPointDistance2D(const Point& P1,
                                         const Point& P2)
{
    return std::sqrt(std::pow(P2.X() - P1.X(),2) +
                     std::pow(P2.Y() - P1.Y(),2));
}
double GeomCalculate::TwoPointDistance3D(const Point& P1,
                                         const Point& P2)
{
    return std::sqrt(std::pow(P2.X() - P1.X(),2) +
                     std::pow(P2.Y() - P1.Y(),2) +
                     std::pow(P2.Z() - P1.Z(),2));
}
double GeomCalculate::CalculateArcRadius(const DefArc& arc)
{
    double R1 = std::sqrt(std::pow(arc.ArcStartPt().X() -
                                   arc.ArcCenterPt().X(), 2) +
                          std::pow(arc.ArcStartPt().Y() -
                                   arc.ArcCenterPt().Y(), 2)),
            R2 = std::sqrt(std::pow(arc.ArcEndPt().X() -
                                    arc.ArcCenterPt().X(), 2) +
                           std::pow(arc.ArcEndPt().Y() -
                                    arc.ArcCenterPt().Y(), 2));
    return (R1 + R2) / 2;
}
double GeomCalculate::SegmentLenth(const DefSeg& seg)
{
    return TwoPointDistance(seg.SegStartPt(),seg.SegEndPt());
}
bool GeomCalculate::IsPntDisGreaterVal(const Point& P1,
                                       const Point& P2,
                                       double val)
{
    double dis = std::sqrt(std::pow(P2.X() - P1.X(),2) +                           std::pow(P2.Y() - P1.Y(),2));
    if(dis >= val)
        return true;
    return false;
}
void GeomCalculate::CloseArcEndpointAngle(const DefArc& arc,
                                          double step,
                                          bool isCloseEp,
                                          double& angle)
{
    double startAgl,endAgl;
    ArcEndpointAngle(arc._arcSp,arc._arcEp,arc._arcCp,
                     arc._arcRot,startAgl,endAgl);
    double centerAgl;
    TwoAngleIncludedAngle(startAgl,endAgl,arc._arcRot,centerAgl);
    if(step >= centerAgl - PreErr5_12){
        ///angle = centerAgl - PreErr5_6;
        if(isCloseEp){
            angle = startAgl;
        }
        else{
            angle = endAgl;
        }
        return;
    }
    if(arc._arcRot == CLOCKWISE){
        if(startAgl < endAgl){
            if(isCloseEp){
                if(endAgl + step > 360.0)
                    angle = step - 360.0 + endAgl;
                else
                    angle = endAgl + step;
            }
            else{
                if(startAgl < step)
                    angle = 360.0 - step + startAgl;
                else
                    angle = startAgl - step;
            }
        }
        else{
            if(isCloseEp)
                angle = endAgl + step;
            else
                angle = startAgl - step;
        }
    }
    else{
        if(startAgl > endAgl){
            if(isCloseEp){
                if(endAgl < step)
                    angle = 360.0 - step + endAgl;
                else
                    angle = endAgl - step;
            }
            else{
                if(360.0 - startAgl < step)
                    angle = step - 360.0 + startAgl;
                else
                    angle = startAgl + step;
            }
        }
        else{            if(isCloseEp)
                angle = endAgl - step;
            else
                angle = startAgl + step;
        }
    }
}
void GeomCalculate::InitialArcRadius(DefArc& arc)
{
    if((arc._arcR <= PreErr5_5) ||
            ! MathCalculate().IsLegalValue(arc._arcR)){
        double radius1 = sqrt(pow(arc._arcSp._x - arc._arcCp._x, 2) +
                              pow(arc._arcSp._y - arc._arcCp._y, 2)) ;
        double radius2 = sqrt(pow(arc._arcEp._x - arc._arcCp._x, 2) +
                              pow(arc._arcEp._y - arc._arcCp._y, 2)) ;
        double radius = (radius1 + radius2) / 2;
        arc._arcR = radius;
    }
}
void GeomCalculate::DiscreteLineToPoint(const Point& lSp,
                                        const Point& lEp,
                                        double stepVal,
                                        vector<Point>& vPoint)
{
    vPoint.emplace_back(lSp);
    if(lSp.IsSamePoint2D(lEp,stepVal)){
        vPoint.emplace_back(lEp);
        return;
    }
    if(lSp.IsSamePoint2D(lEp,stepVal*2)){
        Point mp = lSp.Midpoint(lEp);
        vPoint.emplace_back(mp);
        vPoint.emplace_back(lEp);
        return;
    }
    if(TupleQuality().IsHorizontalLine(
                lSp,lEp,Base_LineHor_Err)){
        double y = (lSp.Y() + lEp.Y()) / 2;
        double startX = lSp.X(),endX = lEp.X();
        if(startX < endX){
            startX = startX + stepVal;
            while (startX < endX) {
                Point p(startX,y,lSp.Z());
                vPoint.emplace_back(p);
                startX = startX + stepVal;
            }
        }
        else{
            startX = startX - stepVal;
            while (startX > endX) {
                Point p(startX,y,lSp.Z());
                vPoint.emplace_back(p);
                startX = startX - stepVal;
            }
        }
        vPoint.emplace_back(lEp);
        return;
    }
    else if(TupleQuality().IsVerticalLine(
                lSp,lEp,Base_LineVer_Err)){        double x = (lSp.X() + lEp.X()) / 2;
        double startY = lSp.Y(),endY = lEp.Y();
        if(startY < endY){
            startY = startY + stepVal;
            while (startY < endY) {
                Point P(x,startY,lSp.Z());
                vPoint.emplace_back(P);
                startY = startY + stepVal;
            }
        }
        else {
            startY = startY - stepVal;
            while (startY > endY) {
                Point P(x,startY,lSp.Z());
                vPoint.emplace_back(P);
                startY = startY - stepVal;
            }
        }
        vPoint.emplace_back(lEp);
        return;
    }
    double slop;
    SegmentAngle(lSp,lEp,slop);
    slop = slop * _RadAlg;
    double len = lSp.Distance2D(lEp),toStartDist = stepVal;
    while (toStartDist < len) {
        double x = toStartDist * std::cos(slop);
        double y = toStartDist * std::sin(slop);
        x += lSp.X();
        y += lSp.Y();
        Point p(x,y,lSp.Z());
        vPoint.emplace_back(p);
        toStartDist = toStartDist + stepVal;
    }
    if(!vPoint.back().IsSamePoint2D(lEp,PreErr5_4)){
        vPoint.emplace_back(lEp);
    }
}
void GeomCalculate::DiscreteLineToPoint3D(const Point& lsp,
                                          const Point& lep,
                                          double stepVal,
                                          vector<Point>& points)
{
    points.emplace_back(lsp);
    if(lsp.IsSamePoint3D(lep,stepVal)){
        points.emplace_back(lep);
        return;
    }
    if(lsp.IsSamePoint3D(lep,stepVal*2)){
        Point mp = lsp.Midpoint(lep);
        points.emplace_back(mp);
        points.emplace_back(lep);
        return;
    }
    /// 计算方向向量和总长度
    double dx = lep.X() - lsp.X();
    double dy = lep.Y() - lsp.Y();
    double dz = lep.Z() - lsp.Z();
    double len = std::sqrt(dx*dx + dy*dy + dz*dz);
    /// 计算单位方向向量
    /// invTotalLength 作用:除法运算在计算机中比乘法更耗时 ,
    /// 预先计算倒数后,可以用乘法代替后续所有除法操作 .    double invTotalLength = 1 / len;
    double ux = dx * invTotalLength;
    double uy = dy * invTotalLength;
    double uz = dz * invTotalLength;
    /// 计算分段数量
    size_t num = static_cast<size_t>(std::floor(len / stepVal));
    points.emplace_back(lsp);
    double t = 0;
    for (size_t i = 1; i <= num; ++i) {
        t = i * stepVal;
        points.emplace_back(Point(lsp.X() + ux * t,
                                  lsp.Y() + uy * t,
                                  lsp.Z() + uz * t));
    }
    if(!points.back().IsSamePoint3D(lep,PreErr5_6)){
        points.emplace_back(lep);
    }
}
void GeomCalculate::DiscreteArcToPoint(const DefArc& arc,
                                       double stepVal,
                                       vector<Point>& points)
{
    double centerAgl = 0;
    ArcCenterAngle(arc,centerAgl);
    points.emplace_back(arc._arcSp);
    if(centerAgl <= stepVal || centerAgl >= 360){
        if(!arc._arcSp.IsSamePoint2D(arc._arcEp,PreErr5_12)){
            points.emplace_back(arc._arcEp);
        }
        return;
    }
    if(centerAgl <= stepVal * 2){
        Point mp;
        ArcMidpoint(arc,mp);
        points.emplace_back(mp);
        points.emplace_back(arc._arcEp);
        return;
    }
    if(stepVal < PreErr_4)
        return;
    Point pt;
    GeomCalculate gocal;
    double curAgl = stepVal;
    bool isCW = arc._arcRot == CLOCKWISE ? true : false;
    while (curAgl < centerAgl) {
        gocal.PointRotate2D(arc._arcSp,arc._arcCp,
                            curAgl,isCW,pt);
        points.emplace_back(pt);
        curAgl += stepVal;
    }
    if(!points.back().IsSamePoint2D(arc._arcEp,PreErr5_12)){
        points.emplace_back(arc.ArcEndPt());
    }
}
void GeomCalculate::DiscreteCircleToPoint(const Point & cp,
                                          const double& R,
                                          RotDir rot,
                                          double stepVal,
                                          vector<Point>& vPoint)
{
    if((R < PreErr_6) || (!go_num->IsValidPoint(cp)))
        return;    vPoint.clear();
    Point sp(cp.X() + R,cp.Y()),ep(cp.X() - R,cp.Y());
    DefArc arc_(sp,ep,cp,R,rot),arc__(ep,sp,cp,R,rot);
    vector<Point> vPoint_;
    DiscreteArcToPoint(arc_,stepVal,vPoint_);
    vPoint = vPoint_;
    vPoint_.clear();
    DiscreteArcToPoint(arc__,stepVal,vPoint_);
    if(vPoint_.size() >= 2){
        vPoint_.erase(vPoint_.begin());
        vPoint_.pop_back();
    }
    for(auto& p : vPoint_){
        vPoint.emplace_back(p);
    }
}
void GeomCalculate::TranslateElemToPoint(const DefElem& ele,
                                         bool isStepAngle,
                                         double segStep,
                                         double arcStep,
                                         vector<Point>&vPoint)
{
    MathCalculate matcal;
    switch (ele.ElemeType()) {
    case LINETYPE:{
        if(segStep < PreErr_4){
            segStep = PreErr5_3;
        }
        DiscreteLineToPoint(ele.ElemSeg().SegStartPt(),
                            ele.ElemSeg().SegEndPt(),
                            segStep,vPoint);
        break;
    }
    case ARCTYPE:{
        if(arcStep < PreErr_4){
            arcStep = PreErr5_3;
        }
        DefArc arc = ele.ElemArc();
        arc.ChoiceSetRadius(PreErr5_2);
        double step = arcStep;
        if(!isStepAngle){
            step = matcal.AngleBaseBowHeight(arc.ArcRadius(),arcStep);
        }
        DiscreteArcToPoint(arc,step,vPoint);
        break;
    }
    case CIRCLETYPE:{
        if(arcStep < PreErr_4){
            arcStep = PreErr5_3;
        }
        DefCircle circle = ele.ElemCircle();
        double step = arcStep;
        if(!isStepAngle){
            step = matcal.AngleBaseBowHeight(circle.CircleRadius(),arcStep);
        }
        DiscreteCircleToPoint(circle.CircleCenter(),
                              circle.CircleRadius(),
                              CLOCKWISE,step,vPoint);
        break;
    }    default:
        break;
    }
}
bool GeomCalculate::IsLegalSigment(const Point& lSp,const Point& lEp)
{
    bool validSp = false,validEp = false;
    validSp = go_num->IsValidPoint(lSp);
    validEp = go_num->IsValidPoint(lEp);
    if(validSp && validEp){
        if(IsIdenticalPoint(lSp,lEp,PreErr5_10))
            return false;
        else
            return true;
    }
    else
        return false;
}
bool GeomCalculate::IsLegalArc(const DefArc &arc)
{
    bool validSp = false,validEp = false,validCp = false;
    validSp = go_num->IsValidPoint(arc._arcSp);
    validEp = go_num->IsValidPoint(arc._arcEp);
    validCp = go_num->IsValidPoint(arc._arcCp);
    if(validSp && validEp && validCp){
        if(IsIdenticalPoint(arc._arcSp,arc._arcEp,PreErr5_10))
            return false;
        else
            return true;
    }
    else
        return false;
}
void GeomCalculate::IsValidElem(const DefElem& ele,
                                bool& isVal)
{
    if(ele._elemType == LINETYPE){
        isVal = IsLegalSigment(ele._segment._segSp,
                               ele._segment._segEp);
    }
    else if(ele._elemType == ARCTYPE){
        isVal = IsLegalArc(ele._arc);
    }
}
void GeomCalculate::IsArcOstBefAftSameDir(const Point& origArcSp,
                                          const Point& origArcEp,
                                          const Point& arcSp,
                                          const Point& arcEp,
                                          bool& isSameDir)
{
    double origSp_ostSp_dis,origSp_ostEp_dis,
            origEp_ostSp_dis,origEp_ostEp_dis;
    TwoPointDistance(origArcSp,arcSp,origSp_ostSp_dis);
    TwoPointDistance(origArcSp,arcEp,origSp_ostEp_dis);    TwoPointDistance(origArcEp,arcSp,origEp_ostSp_dis);
    TwoPointDistance(origArcEp,arcEp,origEp_ostEp_dis);
    if((origSp_ostSp_dis < origSp_ostEp_dis) &&
            (origEp_ostEp_dis < origEp_ostSp_dis )){
        isSameDir = true;
    }
    else{
        isSameDir = false;
    }
}
void GeomCalculate::PointOnLineProjection(const Point &P,const Point &lSp,
                                          const Point &lEp,Point& objPnt)
{
    double linePar[3];
    CalLineParameters(lSp,lEp,linePar);
    double x0 = P.X();
    double y0 = P.Y();
    double A = linePar[0];
    double B = linePar[1];
    double C = linePar[2];
    if(A == 0 && B != 0){
        objPnt._x = x0;
        ///objPnt._y = -C / B;
        objPnt._y = lSp.Y();
    }
    else if(A != 0 && B == 0){
        ///objPnt._x = -C / A;
        objPnt._x = lSp.X();
        objPnt._y = y0;
    }
    else{
        objPnt._x = (B*B*x0 - A*B*y0 - A*C) / (A*A + B*B);
        objPnt._y = (-A*B*x0 + A*A*y0 - B*C) / (A*A + B*B);
    }
    GeomIntSolve().SetHorVerLineIntPnt(lSp,lEp,objPnt);
    objPnt.SetZ(lSp.Z());
}
void GeomCalculate::AngleIsBetweenAngles(double startAgl,
                                         double endAgl,
                                         RotDir rot,
                                         double theta,
                                         double pre,
                                         bool& isIn)
{
    if(rot == ANTICLOCKWISE){
        if(startAgl > endAgl){
            if((theta > endAgl) && (theta < startAgl)){
                isIn = false;
            }else{
                isIn = true;
            }
        }else{
            if((theta + pre >= startAgl) &&
                    (theta - pre <= endAgl)){
                isIn = true;
            }else{
                isIn = false;            }
        }
    }else{
        if(startAgl < endAgl){
            if((theta > startAgl) && (theta < endAgl)){
                isIn = false;
            }else{
                isIn = true;
            }
        }else{
            if((theta - pre <= startAgl) &&
                    (theta + pre >= endAgl)){
                isIn = true;
            }else{
                isIn = false;
            }
        }
    }
}
void GeomCalculate::IsPointInFan(const DefArc& arcData,
                                 const Point& objPnt,
                                 bool& isInFan)
{
    isInFan = false;
    double dis;
    TwoPointDistance(arcData._arcCp,objPnt,dis);
    if((dis - PreErr_10) > arcData._arcR){
        return;
    }
    bool pntIsSp = false,pntIsEp = false;
    IsIdenticalPoint(objPnt,arcData._arcSp,PreErr_10,pntIsSp);
    IsIdenticalPoint(objPnt,arcData._arcEp,PreErr_10,pntIsEp);
    if(pntIsSp || pntIsEp){
        isInFan = true;
        return;
    }
    double startAgl,endAgl;
    ArcEndpointAngle(arcData._arcSp,arcData._arcEp,
                     arcData._arcCp,arcData._arcRot,
                     startAgl,endAgl);
    double theta = atan2((objPnt._y - arcData._arcCp._y),
                         (objPnt._x - arcData._arcCp._x))
            * 180.0 / M_PI;
    theta = (theta < 0) ? (theta += 360) : theta;
    AngleIsBetweenAngles(startAgl,endAgl,arcData._arcRot,
                         theta,PreErr_6,isInFan);
}
void GeomCalculate::CalLineParameters(const Point& sP, const Point& eP,
                                      double lineParas[3])
{
    bool isValidSp = go_num->IsValidPoint(sP);
    bool isValidEp = go_num->IsValidPoint(eP);
    if(!isValidSp || !isValidEp)
        return;
    if(std::abs(eP.Y() - sP.Y()) <= PreErr5_10){        lineParas[0] = 0;
    }
    else{
        lineParas[0] = eP.Y() - sP.Y();
    }
    if(std::abs(sP.X() - eP.X()) <= PreErr5_10){
        lineParas[1] = 0;
    }
    else{
        lineParas[1] = sP.X() - eP.X();
    }
    lineParas[2] = (eP.X() * sP.Y()) - (sP.X() * eP.Y());
}
void GeomCalculate::LineParameters(const Point& lSp, const Point& lEp,
                                   double& A,double& B,double& C)
{
    double lineParas[3];
    CalLineParameters(lSp,lEp,lineParas);
    A = lineParas[0];
    B = lineParas[1];
    C = lineParas[2];
}
double GeomCalculate::AngleMidAngle(double sAng,double eAng,RotDir rotDir)
{
    double absAng;
    TwoAngleIncludedAngle(sAng,eAng,rotDir,absAng);
    double chAng = absAng / 2,midAng;
    if(rotDir == CLOCKWISE){
        if(sAng < eAng){
            if(sAng >= (360.0 - eAng))
                midAng = sAng - chAng;
            else
                midAng = eAng + chAng;
        }
        else
            midAng = sAng - chAng;
    }
    else{
        if(eAng < sAng){
            if(eAng >= (360.0 - sAng))
                midAng = eAng - chAng;
            else
                midAng = sAng + chAng;
        }
        else
            midAng = sAng + chAng;
    }
    return midAng;
}
void GeomCalculate::ArcMidpoint(const DefArc& arc,
                                Point& midPnt)
{
    double sAng,eAng,absAng;
    ArcEndpointAngle(arc._arcSp,arc._arcEp,arc._arcCp,
                     arc._arcRot,sAng,eAng);
    TwoAngleIncludedAngle(sAng,eAng,arc._arcRot,absAng);
    double R = arc.ArcRadius();
    if((R <= PreErr5_2) || !MathCalculate().IsLegalValue(R)){
        DefArc arc_ = arc;        ElemCalculate ().InitialArcRadius(arc_);
        R = arc_.ArcRadius();
    }
    double chAng = absAng / 2.0,midAng;
    if(arc._arcRot == CLOCKWISE){
        if(sAng < eAng){
            if(sAng >= (360.0 - eAng))
                midAng = sAng - chAng;
            else
                midAng = eAng + chAng;
        }
        else
            midAng = sAng - chAng;
    }
    else{
        if(eAng < sAng){
            if(eAng >= (360.0 - sAng))
                midAng = eAng - chAng;
            else
                midAng = sAng + chAng;
        }
        else
            midAng = sAng + chAng;
    }
    double x = R * cos(midAng * (M_PI / 180.0));
    double y = R * sin(midAng * (M_PI / 180.0));
    midPnt._x = arc._arcCp._x + x;
    midPnt._y = arc._arcCp._y + y;
    midPnt.SetZ(arc.ArcCenterPt().Z());
}
void GeomCalculate::CalAngleBetweenTwoVectors(const Point& l1Sp,
                                              const Point& l1Ep,
                                              const Point& l2Sp,
                                              const Point& l2Ep,
                                              double& sTheta,
                                              double& eTheta,
                                              double& angle)
{
    RotDir rot,calRot;
    TriangleRotDirection(l1Sp,l1Ep,l2Ep,rot);
    calRot = rot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    double *curSig1SpAgl = new double[2];
    ArcEndpointAngle(l1Sp,l2Ep,l1Ep,calRot,sTheta,eTheta);
    curSig1SpAgl[0] = sTheta;
    curSig1SpAgl[1] = eTheta;
    //sTheta = curSig1SpAgl[0];
    //eTheta = curSig1SpAgl[1];
    double aglAbsVal;
    TwoAngleIncludedAngle(curSig1SpAgl[0],curSig1SpAgl[1],
            calRot,aglAbsVal);
    double aveAgl = aglAbsVal / 2;
    if(calRot == CLOCKWISE){
        if(curSig1SpAgl[0] < curSig1SpAgl[1]){
            if(curSig1SpAgl[0] >= (360.0 - curSig1SpAgl[1]))
                angle = curSig1SpAgl[0] - aveAgl;
            else
                angle = curSig1SpAgl[1] + aveAgl;
        }
        else
            angle = curSig1SpAgl[0] - aveAgl;    }
    else{
        if(curSig1SpAgl[1] < curSig1SpAgl[0]){
            if(curSig1SpAgl[1] >= (360.0 - curSig1SpAgl[0]))
                angle = curSig1SpAgl[1] - aveAgl;
            else
                angle = curSig1SpAgl[0] + aveAgl;
        }
        else
            angle = curSig1SpAgl[0] + aveAgl;
    }
}
double GeomCalculate::dot(const Vector& a,const Vector& b)
{
    return a._x * b._x + a._y * b._y;
}
double GeomCalculate::OuterProduct(Vector& a,const Vector& b)
{
    return abs(a._x*b._y-a._y*b._x);
}
void GeomCalculate::TwoPointMidpoint(const Point& P1,
                                     const Point& P2,Point& P)
{
    P.SetX((P1.X() + P2.X()) / 2);
    P.SetY((P1.Y() + P2.Y()) / 2);
    P.SetZ((P1.Z() + P2.Z()) / 2);
}
void GeomCalculate::TwoPointMidpoint2D(const Point& P1,
                                       const Point& P2,Point& P)
{
    P.SetX((P1.X() + P2.X()) / 2);
    P.SetY((P1.Y() + P2.Y()) / 2);
}
void GeomCalculate::GetClosedFixedPnt(const Point&fixedPnt,
                                      const Point &varPnt,
                                      const double dis,
                                      Point& endPnt)
{
    double dis_value;
    TwoPointDistance(fixedPnt,varPnt,dis_value);
    if(dis_value <= dis){
        TwoPointMidpoint(fixedPnt,varPnt,endPnt);
        return;
    }
    Point temVarPnt(varPnt._x,varPnt._y,varPnt._z);
    while(dis_value > dis){
        Point temMidPnt;
        TwoPointMidpoint(fixedPnt,temVarPnt,temMidPnt);
        temVarPnt = temMidPnt;
        TwoPointDistance(fixedPnt,temVarPnt,dis_value);
    }
    endPnt = temVarPnt;
}void GeomCalculate::CloseSegmentEndpoint(const Point& lSp,
                                         const Point& lEp,
                                         double stepVal,
                                         bool isCloseSp,
                                         Point& P)
{
    if(lSp.IsSamePoint2D(lEp,stepVal)){
        if(isCloseSp){
            P = lEp;
        }else{
            P = lSp;
        }
        return;
    }
    TupleQuality tup;
    if(isCloseSp){
        if(tup.IsHorizontalLine(lSp,lEp,Base_LineHor_Err)){
            double startX = lSp.X();
            if(startX < lEp.X())
                startX = startX + stepVal;
            else
                startX = startX - stepVal;
            P.SetX(startX);
            P.SetY((lSp.Y() + lEp.Y()) / 2);
        }
        else if(tup.IsVerticalLine(lSp,lEp,Base_LineVer_Err)){
            double startY = lSp.Y();
            if(startY < lEp.Y())
                startY = startY + stepVal;
            else
                startY = startY - stepVal;
            P.SetX((lSp.X() + lEp.X()) / 2);
            P.SetY(startY);
        }
        else{
            double slop;
            SegmentAngle(lSp,lEp,slop);
            slop = slop * _RadAlg;
            double x = stepVal * std::cos(slop);
            double y = stepVal * std::sin(slop);
            P.SetX(x + lSp.X());
            P.SetY(y + lSp.Y());
        }
    }
    else{
        if(tup.IsHorizontalLine(lSp,lEp,Base_LineHor_Err)){
            double endX = lEp.X();
            if(endX < lSp.X())
                endX = endX + stepVal;
            else
                endX = endX - stepVal;
            P.SetX(endX);
            P.SetY((lSp.Y() + lEp.Y()) / 2);
        }
        else if(tup.IsVerticalLine(lSp,lEp,Base_LineVer_Err)){
            double endY = lEp.Y();
            if(endY < lSp.Y())
                endY = endY + stepVal;
            else
                endY = endY - stepVal;
            P.SetX((lSp.X() + lEp.X()) / 2);
            P.SetY(endY);
        }
        else{            double slop;
            SegmentAngle(lSp,lEp,slop);
            slop = slop * _RadAlg;
            double x = stepVal * std::cos(slop);
            double y = stepVal * std::sin(slop);
            P.SetX(lEp.X() - x);
            P.SetY(lEp.Y() - y);
        }
    }
    P.SetZ(lSp.Z());
}
void GeomCalculate::ArcEndpointAngle(const Point &sP,const Point &eP,
                                     const Point &cP,const RotDir &rot,
                                     double& startAgl,
                                     double& endAgl)
{
    startAgl = std::atan2((sP.Y() - cP.Y()),(sP.X() - cP.X()));
    endAgl = std::atan2((eP.Y() - cP.Y()),(eP.X() - cP.X()));
    startAgl = startAgl *_AlgRad;
    endAgl = endAgl * _AlgRad;
    if (startAgl < 0){
        startAgl = 360 + startAgl;
    }
    if (endAgl < 0){
        endAgl = 360 + endAgl;
    }
}
void GeomCalculate::ArcEndpointAngle(const DefArc& arc,
                                     double& startAgl,
                                     double& endAgl)
{
    ArcEndpointAngle(arc.ArcStartPt(),arc.ArcEndPt(),
                     arc.ArcCenterPt(),arc.ArcRot(),
                     startAgl,endAgl);
}
void GeomCalculate::ArcCenterAngle(const DefArc& arc,
                                   double& angle)
{
    double startAgl,endAgl;
    ArcEndpointAngle(arc._arcSp,arc._arcEp,
                     arc._arcCp, arc._arcRot,
                     startAgl,endAgl);
    TwoAngleIncludedAngle(startAgl,endAgl,arc._arcRot,angle);
}
void GeomCalculate::PointToLineDistance(const Point &lSp,
                                        const Point &lEp,
                                        const Point &P,
                                        double& dis)
{
    double linePara[3];
    CalLineParameters(lSp,lEp,linePara);
    if(std::abs(linePara[0]) < PreErr5_12){
        double dis1 = std::abs(lSp.Y() - P.Y());
        double dis2 = std::abs(lEp.Y() - P.Y());
        dis = (dis1 + dis2) / 2;
    }    else if(std::abs(linePara[1]) < PreErr5_12){
        double dis1 = std::abs(lSp.X() - P.X());
        double dis2 = std::abs(lEp.X() - P.X());
        dis = (dis1 + dis2) / 2;
    }
    else{
        double A = linePara[0];
        double B = linePara[1];
        double C = linePara[2];
        dis = std::abs(A * P.X() + B * P.Y() + C) / std::sqrt(A * A + B * B);
    }
}
void GeomCalculate::CircleTangentEquation(const Point &cp,
                                          const double &R,
                                          const Point &P,
                                          double (&lines)[3])
{
    lines[0] = P._x - cp._x;
    lines[1] = P._y - cp._y;
    lines[2] = std::pow(cp._x,2) +  std::pow(cp._y,2) -
            P._x * cp._x - P._y * cp._y - std::pow(R,2);
}
void GeomCalculate::IsIdenticalPoint(const Point& p1,const Point& p2,
                                     double pre,bool& isSame)
{
    if(pre <= 0)
        pre = PreErr_10;
    double dis;
    TwoPointDistance(p1,p2,dis);
    if(dis <= pre)
        isSame = true;
    else
        isSame = false;
}
bool GeomCalculate::IsIdenticalPoint(const Point& p1,
                                     const Point& p2, double pre)
{
    if(pre <= 0)
        pre = PreErr_10;
    if((std::abs(p1.X() - p2.X()) < pre) &&
            (std::abs(p1.Y() - p2.Y()) <= pre) &&
            (std::abs(p1.Z() - p2.Z()) <= pre)){
        return true;
    }
    return false;
}
bool GeomCalculate::IsIdenticalPoint2D(const Point& p1,const Point& p2,
                                       double pre)
{
    if(pre <= 0)
        pre = PreErr_10;
    double distance = TwoPointDistance2D(p1,p2);
    if(distance <= pre)
        return true;
    return false;
}void GeomCalculate::IsIdenticalPoint2D(const Point& p1,
                                       const Point& p2,
                                       double pre,bool& isSame)
{
    if(pre <= 0)
        pre = PreErr_10;
    isSame = false;
    double distance = TwoPointDistance2D(p1,p2);
    if(distance <= pre)
        isSame = true;
}
void GeomCalculate::TwoPointAnotherPoint(const Point& p1,
                                         const Point& p2,
                                         const Point& cmpP,
                                         Point& p)
{
    if(IsIdenticalPoint(p1,cmpP,PreErr_10))
        p = p2;
    else
        p = p1;
}
void GeomCalculate::AngleInWhichQuadrant(const double& angle,
                                         AngleWhichQuadrant whichQuadrant)
{
    if((angle > 0) && (angle < 90)){
        whichQuadrant = FIRSTQUADRANT;
    }
    else if((angle > 90) && (angle < 180)){
        whichQuadrant = SECONDQUADRANT;
    }
    else if((angle > 180) && (angle < 270)){
        whichQuadrant = THIRDQUADRANT;
    }
    else if((angle > 270) && (angle < 360)){
        whichQuadrant = FOURTHTQUADRANT;
    }
}
void GeomCalculate::TwoAngleIncludedAngle(const double& startAgl,
                                          const double& endAgl,
                                          const RotDir& rot,
                                          double& aglAbs)
{
    if(rot == ANTICLOCKWISE){
        if(startAgl >= endAgl){
            aglAbs = endAgl + (360.0 - startAgl);
        }else{
            aglAbs = endAgl - startAgl;
        }
    }else{
        if(startAgl < endAgl){
            aglAbs = startAgl + (360.0 - endAgl);
        }else{
            aglAbs = startAgl - endAgl;
        }
    }
    if(aglAbs == 360.0){
        aglAbs = 0;
    }
}void GeomCalculate::CalculateArcLength(DefArc arc,
                                       double& len)
{
    arc.ChoiceSetRadius(PreErr5_3);
    double R = arc._arcR;
    if((!MathCalculate().IsLegalValue(R)) || R <= PreErr5_4){
        DefArc temArc = arc;
        InitialArcRadius(temArc);
        R = temArc._arcR;
    }
    if(arc.ArcStartPt().IsSamePoint2D(arc.ArcEndPt(),
                                      PreErr5_16)){
        len = 0;
        return;
    }
    double startAgl,endAgl,aglAbs;
    ArcEndpointAngle(arc.ArcStartPt(),arc.ArcEndPt(),
                     arc.ArcCenterPt(),arc.ArcRot(),
                     startAgl,endAgl);
    TwoAngleIncludedAngle(startAgl,endAgl,
                          arc._arcRot,aglAbs);
    double C = 2 * M_PI * R;
    len = (aglAbs / 360.0) * C;
}
double GeomCalculate::ElementLenth(const DefElem& ele)
{
    double lenth;
    if(ele._elemType == LINETYPE){
        lenth = ele._segment._segSp.Distance2D(ele._segment._segEp);
    }
    else if(ele._elemType == ARCTYPE){
        CalculateArcLength(ele._arc,lenth);
    }
    else if(ele._elemType == CIRCLETYPE){
        lenth = ele._circle._cirR * ele._circle._cirR * M_PI;
    }
    return lenth;
}
void GeomCalculate::GetSameDirCloseRayExtremePnt(const Point& raySp,
                                                 const Point& rayEp,
                                                 const Point& pnt1,
                                                 const Point& pnt2,
                                                 bool isCloseSp,
                                                 Point& objPnt)
{
    bool isSamePnt = false;
    IsIdenticalPoint(pnt1,pnt2,PreErr_12,isSamePnt);
    if(isSamePnt){
        objPnt._x = (pnt1._x + pnt2._x) / 2;
        objPnt._y = (pnt1._y + pnt2._y) / 2;
        return;
    }
    double slop,slop1,slop2;
    SegmentAngle(raySp,rayEp,slop);
    SegmentAngle(raySp,pnt1,slop1);
    SegmentAngle(raySp,pnt2,slop2);
    MathCalculate matcal;    //matcal.IsNumericEqual
    if((matcal.IsNumericEqual(slop,slop1,PreErr_12) &&
        matcal.IsNumericEqual(slop,slop2,PreErr_12)) ||
            (! matcal.IsNumericEqual(slop,slop1,PreErr_12) &&
             ! matcal.IsNumericEqual(slop,slop2,PreErr_12))){
        double dis1,dis2;
        TwoPointDistance(raySp,pnt1,dis1);
        TwoPointDistance(raySp,pnt2,dis2);
        if(isCloseSp){
            objPnt = dis1 <= dis2 ? pnt1 : pnt2;
        }
        else{
            objPnt = dis1 <= dis2 ? pnt2 : pnt1;
        }
    }
    else if(matcal.IsNumericEqual(slop,slop1,PreErr_12) &&
            ! matcal.IsNumericEqual(slop,slop2,PreErr_12)){
        if(isCloseSp){
            objPnt = pnt1;
        }
        else{
            objPnt = pnt2;
        }
    }
    else {
        if(isCloseSp){
            objPnt = pnt2;
        }
        else{
            objPnt = pnt1;
        }
    }
}
void GeomCalculate::TriangleRotDirection(const Point& P1,
                                         const Point& P2,
                                         const Point& P3,
                                         RotDir& rot)
{
    double cross = (P2.X() - P1.X()) * (P3.Y() - P2.Y())-
            (P2.Y() - P1.Y()) * (P3.X() - P2.X());
    if(cross > 0)
        rot = ANTICLOCKWISE;
    else
        rot = CLOCKWISE;
}
void GeomCalculate::ElementCloseEndpoint(const DefElem& ele,
                                         bool isCloseEp,
                                         double stepVal,
                                         Point &P)
{
    GeometryLogic golg;
    switch (ele._elemType) {
    case LINETYPE:
        CloseSegmentEndpoint(ele.ElemSeg().SegStartPt(),
                             ele.ElemSeg().SegEndPt(),
                             stepVal,!isCloseEp,P);
        break;
    case ARCTYPE:
        golg.CloseArcEndpointBaseAngle(ele.ElemArc(),                                       isCloseEp,stepVal,P);
        break;
    default:
        break;
    }
}
void GeomCalculate::PointAroundFixedRotate(const Point& p,
                                           const Point& fix,
                                           double A,
                                           RotDir isCW,
                                           Point& pt)
{
    A = A * _RadAlg;
    const double& x = p._x,y = p._y,cx = fix._x,cy = fix._y;
    if(isCW == CLOCKWISE){
        pt._x = (x - cx)*std::cos(-A) - (y - cy)*std::sin(-A) + cx ;
        pt._y = (x - cx)*std::sin(-A) + (y - cy)*std::cos(-A) + cy ;
    }else{
        pt._x = (x - cx)*std::cos(A) - (y - cy)*std::sin(A) + cx ;
        pt._y = (x - cx)*std::sin(A) + (y - cy)*std::cos(A) + cy ;
    }
    pt.SetZ(p.Z());
}
void GeomCalculate::PointRotate2D(const Point& p,
                                  const Point& fix,
                                  double A, bool isCW,
                                  Point& pt)
{
    A = A * _RadAlg;
    const double& x = p._x,y = p._y,cx = fix._x,cy = fix._y;
    if(isCW){
        pt._x = (x - cx)*std::cos(-A) - (y - cy)*std::sin(-A) + cx ;
        pt._y = (x - cx)*std::sin(-A) + (y - cy)*std::cos(-A) + cy ;
    }else{
        pt._x = (x - cx)*std::cos(A) - (y - cy)*std::sin(A) + cx ;
        pt._y = (x - cx)*std::sin(A) + (y - cy)*std::cos(A) + cy ;
    }
    pt.SetZ(p.Z());
}
void GeomCalculate::SegmentBisectorVertical(const Point&lSp,const Point&lEp,
                                            double& A,double& B,double& C)
{
    TupleQuality tup;
    double x = (lSp.X() + lEp.X()) / 2;
    double y = (lSp.Y() + lEp.Y()) / 2;
    if(tup.IsHorizontalLine(lSp,lEp,PreErr5_6)){
        A = 1;
        B = 0;
        C = - x;
    }
    else if(tup.IsVerticalLine(lSp,lEp,PreErr5_6)){
        A = 0;
        B = 1;
        C = - y;
    }
    else{
        double slope = (lEp.Y() - lSp.Y()) / (lEp.X() - lSp.X());
        double slope_ = -1 / slope;
        A = slope_;
        B = -1;
        C = y - slope_ * x;    }
}
void GeomCalculate::SegmentVerticalBisector(const Point& sp, const Point& ep,
                                            double& A, double& B, double& C)
{
    A = ep.X() - sp.X();
    B = ep.Y() - sp.Y();
    C = -((ep.X() * ep.X() + ep.Y() * ep.Y()) -
          (sp.X() * sp.X() + sp.Y() * sp.Y())) / 2.0;
}
double GeomCalculate::ArcBowHeight(const DefArc &arc)
{
    DefArc arc_ = arc;
    arc_.ChoiceSetRadius(PreErr5_2);
    double R = arc_.ArcRadius();
    Point mp;
    TwoPointMidpoint2D(arc.ArcStartPt(),arc.ArcEndPt(),mp);
    double dis = TwoPointDistance2D(arc.ArcCenterPt(),mp);
    double angle;
    ArcCenterAngle(arc_,angle);
    double h = angle < 180 ? R - dis : R + dis;
    return h;
}
///P在arc上.
double GeomCalculate::ArcPointTangentAngle(const DefArc &arc,const Point &P)
{
    Point P_;
    PointAroundFixedRotate(P,arc.ArcCenterPt(),90,arc.ArcRot(),P_);
    double a;
    SegmentAngle(arc.ArcCenterPt(),P_,a);
    return a;
}
bool GeomCalculate::ThreePointCollinear(const Point& p0,
                                        const Point& p1,
                                        const Point& p2,
                                        double err)
{
    Point v1 = p1 - p0;  //  向量p0 → p1
    Point v2 = p2 - p0;  //  向量p0 → p2
    Point crossProd = v1.Cross(v2); // 叉积
    return crossProd.Length() < err; //  叉积模< err 则共线
}
void GeomCalculate::SimplePointsBaseLine(double err, vector<Point> &points)
{
    size_t n = points.size();
    if (n < 3) {
        return;
    }
#if 0
    size_t s = 0;
    vector<Point>compressed;
    compressed.push_back(points[0]);
    for (size_t i = 2; i < n; ++i) {
        /// 检查当前点是否能与前两个点共线
        if (!ThreePointCollinear(points[s],points[i-1],points[i],err)) {
            /// 如果不能共线,保存当前线段
            if (s != i-1 && !points[s].IsSamePoint3D(points[i-1],err)) {                compressed.emplace_back(points[i-1]);
            }
            s = i-1;
        }
    }
    /// 处理最后一个线段
    if (s != n-1 && !points[s].IsSamePoint3D(points.back(),err)) {
        compressed.push_back(points.back());
    }
    points = compressed;
#else
    size_t left = 0;
    while (left + 2 < points.size()) {
        size_t right = left + 2;
        bool allCollinear = true;
        /// 检查v[left]和v[right]之间的点是否共线
        for (size_t i = left + 1; i < right; ++i) {
            if (!ThreePointCollinear(points[left], points[i], points[right], 
err)) {
                allCollinear = false;
                break;
            }
        }
        if (allCollinear) {
            /// 删除v[left+1 ... right-1]
            points.erase(points.begin() + left + 1, points.begin() + right);
        } else {
            left++; /// 不共线,移动左指针
        }
    }
#endif
}
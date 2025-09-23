#include "SmoothProCal.h"
shared_ptr<GeomCalculate>smoCal_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>smoCal_num = make_shared<ElemCalculate>();
shared_ptr<EquidistantCurve>smoCal_equ = make_shared<EquidistantCurve>();
shared_ptr<GeometryLogic>smoCal_golg = make_shared<GeometryLogic>();
shared_ptr<ElementIntersect>smoCal_intPnt = make_shared<ElementIntersect>();
shared_ptr<GeomIntersect>smoCal_goIn = make_shared<GeomIntersect>();
void SmoothProCal::CalSigAnglBisectorStrechLenth(const Point& l1Sp,
                                                 const Point& l1Ep,
                                                 const Point& l2Sp,
                                                 const Point& l2Ep,
                                                 const double& strechLimit,
                                                 DefArc& arcData)
{
    double sTheta,eTheta,aveAgl;
    smoCal_go-> CalAngleBetweenTwoVectors(l1Sp,l1Ep,l2Sp,l2Ep,
                                          sTheta,eTheta,aveAgl);
    RotDir calRot;
    smoCal_go->TriangleRotDirection(l1Sp,l1Ep,l2Ep,arcData._arcRot);
    calRot = arcData._arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    double aglAbsVal;
    smoCal_go-> TwoAngleIncludedAngle(sTheta,aveAgl,calRot,aglAbsVal);
    aglAbsVal = aglAbsVal * M_PI / 180.0;
    double calAveAgl = aveAgl * M_PI / 180.0;
    double xInc,yInc;
    if((abs(aveAgl - 270.0) > PreErr_6) &&
            (abs(aveAgl - 90.0) > PreErr_6)){
        xInc = strechLimit * std::cos(calAveAgl);
        yInc = strechLimit * std::sin(calAveAgl);
    }
    else{
        xInc = 0.0;
        yInc = strechLimit * std::sin(calAveAgl);
    }
    Point extendPnt;
    extendPnt._x = l1Ep._x + xInc;
    extendPnt._y = l1Ep._y + yInc;
    arcData._arcCp = extendPnt;
    smoCal_go->PointOnLineProjection(extendPnt,l1Sp,l1Ep,arcData._arcSp);
    smoCal_go->PointOnLineProjection(extendPnt,l2Sp,l2Ep,arcData._arcEp);
}
void SmoothProCal::GetArcArcConnectArcCenterPnt(const DefArc& arcData1,
                                                const DefArc& arcData2,
                                                const double& radius,
                                                bool& isExiCenterPnt,
                                                Point &centerPnt)
{
    isExiCenterPnt = false;
    DefArc arcData1_out,arcData2_out,arcData1_wit,arcData2_wit;
    bool isExiEquiArc = false;
    smoCal_equ->EquidistantArc(arcData1,radius,
                                  OUTSIDEPROFILE,
                                  false,isExiEquiArc,                                  arcData1_out);
    smoCal_equ->EquidistantArc(arcData2,radius,
                                  OUTSIDEPROFILE,
                                  false,isExiEquiArc,
                                  arcData2_out);
    smoCal_equ->EquidistantArc(arcData1,radius,
                                  WITHINPROFILE,
                                  false,isExiEquiArc,
                                  arcData1_wit);
    smoCal_equ->EquidistantArc(arcData2,radius,
                                  WITHINPROFILE,
                                  false,isExiEquiArc,
                                  arcData2_wit);
    GeomIntersect goInt;
    GeomIntRes intRes;
    goInt.CalculateArcArcInt(arcData1_out,arcData2_out,
                             PreErr_10,PreErr_10,PreErr_10,intRes);
    if(intRes._isExistInt){
        if(intRes._intNum == 1){
            centerPnt = intRes._intPnt1;
        }
        else{
            smoCal_intPnt->GetCloseArcExtremePnt(arcData1_out,intRes._intPnt1,
                                                 intRes._intPnt2,false,
                                                 centerPnt);
        }
    }
    else{
        goInt.CalculateArcArcInt(arcData1_wit,arcData2_wit,
                                 PreErr_10,PreErr_10,PreErr_10,intRes);
        if(intRes._isExistInt){
            if(intRes._intNum == 1){
                centerPnt =  intRes._intPnt1;
            }
            else{
                smoCal_intPnt-
>GetCloseArcExtremePnt(arcData1_wit,intRes._intPnt1,
                                                     intRes._intPnt2,false,
                                                     centerPnt);
            }
        }
        else{
            return;
        }
    }
    isExiCenterPnt = true;
}
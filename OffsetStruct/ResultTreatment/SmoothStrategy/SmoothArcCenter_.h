#include "SmoothArcCenter.h"
void SmoothArcCenter::SegmentSegmentSmoothArcCenter(const Point& l1Sp,
                                                    const Point& l1Ep,
                                                    const Point& l2Sp,
                                                    const Point& l2Ep,
                                                    double& radius,
                                                    bool& isExiCenter,
                                                    Point &center)
{
    isExiCenter = false;
    EquidistantCurve equi;
    GeomIntersect goInt;
    while (!isExiCenter && radius > SmoothValLimit) {
        Point outL1Sp,outL1Ep,outL2Sp,outL2Ep;
        equi.EquidistantSegment(l1Sp,l1Ep,radius,
                                   OUTSIDEPROFILE,
                                   outL1Sp,outL1Ep);
        equi.EquidistantSegment(l2Sp,l2Ep,radius,
                                   OUTSIDEPROFILE,
                                   outL2Sp,outL2Ep);
        GeomIntRes intRes;
        goInt.CalculateSegSegIntersect(outL1Sp,outL1Ep,outL2Sp,
                                       outL2Ep,true,PreErr_10,
                                       PreErr_10,intRes);
        if(intRes._isExistInt && intRes._isExiGeoInt){
            center = intRes._intPnt1;
            isExiCenter = true;
        }
        else{
            Point witL1Sp,witL1Ep,witL2Sp,witL2Ep;
            equi.EquidistantSegment(l1Sp,l1Ep,radius,
                                       WITHINPROFILE,
                                       witL1Sp,witL1Ep);
            equi.EquidistantSegment(l2Sp,l2Ep,radius,
                                       WITHINPROFILE,
                                       witL2Sp,witL2Ep);
            GeomIntRes intRes1;
            goInt.CalculateSegSegIntersect(witL1Sp,witL1Ep,witL2Sp,
                                           witL2Ep,true,PreErr_10,
                                           PreErr_10,intRes);
            if(intRes1._isExistInt && intRes1._isExiGeoInt){
                center = intRes1._intPnt1;
                isExiCenter = true;
            }
            else{
                radius = radius / SmoothTimes;
            }
        }
    }
}
void SmoothArcCenter::SegmentArcSmoothArcCenter(const Point& lSp,
                                                const Point& lEp,
                                                const DefArc& arc,
                                                bool isLineFont,
                                                double &radius,
                                                bool& isExiCenter,
                                                Point &center){
    isExiCenter = false;
    GeomIntersect goInt;
    EquidistantCurve equi;
    ElementIntersect eleint;
    while (!isExiCenter && radius > SmoothValLimit) {
        Point out_sp,out_ep;
        DefArc out_arc;
        equi.EquidistantSegment(lSp,lEp,radius,OUTSIDEPROFILE,
                                   out_sp,out_ep);
        bool isExiEquiArc = false;
        equi.EquidistantArc(arc,radius,OUTSIDEPROFILE,
                               false,isExiEquiArc,
                               out_arc);
        if(!isExiEquiArc){
            radius = radius / SmoothTimes;
            continue;
        }
        bool isCloseSp = isLineFont;
        GeomIntRes intRes;
        goInt.CalculateSegmentArcInt(out_sp,out_ep,out_arc,
                                     PreErr_10,PreErr_10,
                                     PreErr_10,intRes);
        if(intRes._isExistInt){
            if(intRes._intNum == 1){
                center = intRes._intPnt1;
            }
            else{
                eleint.GetCloseArcExtremePnt(out_arc,intRes._intPnt1,
                                             intRes._intPnt2,isCloseSp,
                                             center);
            }
            isExiCenter = true;
        }
        else{
            Point wit_sp,wit_ep;
            DefArc wit_arc;
            equi.EquidistantSegment(lSp,lEp,radius,WITHINPROFILE,
                                       wit_sp,wit_ep);
            bool isExiEquiArc1 = false;
            equi.EquidistantArc(arc,radius,WITHINPROFILE,
                                   false,isExiEquiArc1,
                                   wit_arc);
            if(!isExiEquiArc1){
                radius = radius / SmoothTimes;
                continue;
            }
            goInt.CalculateSegmentArcInt(wit_sp,wit_ep,wit_arc,
                                         PreErr_10,PreErr_10,
                                         PreErr_10,intRes);
            if(intRes._isExistInt){
                if(intRes._intNum == 1){
                    center = intRes._intPnt1;
                }
                else{
                    eleint.GetCloseArcExtremePnt(wit_arc,intRes._intPnt1,
                                                 intRes._intPnt2,isCloseSp,
                                                 center);
                }
                isExiCenter = true;
            }
            else{
                radius = radius / SmoothTimes;
                continue;            }
        }
    }
}
void SmoothArcCenter::ArcArcSmoothArcCenter(const DefArc& arc1,
                                            const DefArc& arc2,
                                            double& radius,
                                            bool& isExiCenter,
                                            Point &center)
{
    isExiCenter = false;
    GeomIntersect goInt;
    EquidistantCurve equi;
    ElementIntersect eleint;
    while (!isExiCenter && radius > SmoothValLimit){
        DefArc out_arc1,out_arc2;
        bool isExiOutEquArc1 = false,isExiOutEquArc2 = false;
        equi.EquidistantArc(arc1,radius,OUTSIDEPROFILE,
                               false,isExiOutEquArc1,out_arc1);
        equi.EquidistantArc(arc2,radius,OUTSIDEPROFILE,
                               false,isExiOutEquArc2,out_arc2);
        if(!isExiOutEquArc1 || !isExiOutEquArc2){
            radius = radius / SmoothTimes;
            continue;
        }
        GeomIntRes intRes;
        goInt.CalculateArcArcInt(out_arc1,out_arc2,PreErr_10,
                                 PreErr_10,PreErr_10,intRes);
        if(intRes._isExistInt){
            if(intRes._intNum == 1){
                center = intRes._intPnt1;
            }
            else{
                eleint.GetCloseArcExtremePnt(out_arc1,intRes._intPnt1,
                                             intRes._intPnt2,false,center);
            }
            isExiCenter = true;
        }
        else{
            DefArc wit_arc1,wit_arc2;
            bool isExiWitEquArc1 = false,isExiWitEquArc2 = false;
            equi.EquidistantArc(arc1,radius,WITHINPROFILE,false,
                                   isExiWitEquArc1,wit_arc1);
            equi.EquidistantArc(arc2,radius,WITHINPROFILE,false,
                                   isExiWitEquArc2,wit_arc2);
            if(!isExiWitEquArc1 || !isExiWitEquArc2){
                radius = radius / SmoothTimes;
                continue;
            }
            goInt.CalculateArcArcInt(wit_arc1,wit_arc2,PreErr_10,
                                     PreErr_10,PreErr_10,intRes);
            if(!intRes._isExistInt){
                radius = radius / SmoothTimes;
                continue;
            }
            if(intRes._intNum == 1){
                center = intRes._intPnt1;
            }
            else{                eleint.GetCloseArcExtremePnt(wit_arc1,intRes._intPnt1,
                                             intRes._intPnt2,false,center);
            }
            isExiCenter = true;
        }
    }
}
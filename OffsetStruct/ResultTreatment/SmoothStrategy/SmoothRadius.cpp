#include "SmoothRadius.h"
shared_ptr<GeomCalculate>smoRad_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>smoRad_num = make_shared<ElemCalculate>();
shared_ptr<ElementIntersect>smoRad_eleIn = make_shared<ElementIntersect>();
shared_ptr<GeomDistance>smoRad_godis = make_shared<GeomDistance>();
void SmoothRadius::ResetSmoonthRadiusBaseSeg(const Point& lSp,
                                             const Point& lEp,
                                             double times,
                                             double& radius)
{
    double len;
    GeomCalculate().TwoPointDistance(lSp,lEp,len);
    if(radius > len / times){
        radius = len / times;
    }
}
void SmoothRadius::ResetSmoonthRadiusBaseArc(const DefArc& arc,
                                             double times,
                                             double& radius)
{
    double len;
    GeomCalculate().CalculateArcLength(arc,len);
    double timesR = radius * times;
    if(len < timesR || arc._arcR < timesR){
        double min = len < arc._arcR ? len : arc._arcR;
        radius = min / times;
    }
}
void SmoothRadius::IsSigSigSmoothExtrOnOriEle(const Point& l1Sp,
                                              const Point& l1Ep,
                                              const Point& l2Sp,
                                              const Point& l2Ep,
                                              const Point& arcSp,
                                              const Point& arcEp,
                                              bool& isOnOriEle)
{
    isOnOriEle = false;
    PointOnElement pton;
    bool isLocated = false;
    isOnOriEle = false;
    pton.IsPointOnSegment(l1Sp,l1Ep,arcSp,PreErr5_10,
                          PreErr5_10, false,isLocated);
    if(!isLocated){
        return;
    }
    pton.IsPointOnSegment(l2Sp,l2Ep,arcEp,PreErr5_10,
                          PreErr5_10,false,isLocated);
    if(!isLocated){
        return;
    }
    double dis1,dis2;
    smoRad_go->TwoPointDistance(l1Ep,arcSp,dis1);
    if(dis1 < SmoothExtrOnOri){
        return;
    }
    smoRad_go->TwoPointDistance(l2Sp,arcEp,dis2);
    if(dis2 < SmoothExtrOnOri){
        return;
    }    isOnOriEle = true;
}
void SmoothRadius::IsSigArcSmoothExtrOnOriEle(const Point& lSp,
                                              const Point& lEp,
                                              const DefArc& arc,
                                              const Point& arcSp,
                                              const Point& arcEp,
                                              bool isLineFont,
                                              bool& isOnOriEle)
{
    isOnOriEle = false;
    PointOnElement pton;
    bool isOnEle = false;
    if(isLineFont){
        pton.IsPointOnSegment(lSp,lEp,arcSp,PreErr5_10,
                              PreErr5_10,false,isOnEle);
        if(!isOnEle){
            return;
        }
        pton.IsPointOnArc(arc,arcEp,PreErr5_10,PreErr5_10,
                          false,isOnEle);
        if(!isOnEle){
            return;
        }
        double dis1,dis2;
        smoRad_go->TwoPointDistance(lEp,arcSp,dis1);
        if(dis1 < SmoothExtrOnOri){
            return;
        }
        smoRad_godis->PointToArcEndpointDistance(arc,arcEp,true,dis2);
        if(dis2 < SmoothExtrOnOri){
            return;
        }
    }
    else{
        pton.IsPointOnArc(arc,arcSp,PreErr5_10,
                          PreErr5_10,false,isOnEle);
        if(!isOnEle){
            return;
        }
        pton.IsPointOnSegment(lSp,lEp,arcEp,PreErr5_10,
                              PreErr5_10,false,isOnEle);
        if(!isOnEle){
            return;
        }
        double dis1,dis2;
        smoRad_godis->PointToArcEndpointDistance(arc,arcSp,false,dis1);
        if(dis1 < SmoothExtrOnOri){
            return;
        }
        smoRad_go->TwoPointDistance(lSp,arcEp,dis2);
        if(dis2 < SmoothExtrOnOri){
            return;
        }
    }
    isOnOriEle = true;
}void SmoothRadius::IsArcArcSmoothExtrOnOriEle(const DefArc& arc1,
                                              const DefArc& arc2,
                                              const Point& arcSp,
                                              const Point& arcEp,
                                              bool& isOnOriEle)
{
    isOnOriEle = false;
    PointOnElement pton;
    bool isOnEle = false;
    pton.IsPointOnArc(arc1,arcSp,PreErr5_10,
                      PreErr5_10,false,isOnEle);
    if(!isOnEle){
        return;
    }
    pton.IsPointOnArc(arc2,arcEp,PreErr5_10,
                      PreErr5_10,false,isOnEle);
    if(!isOnEle){
        return;
    }
    double dis1,dis2;
    smoRad_godis->PointToArcEndpointDistance(arc1,arcSp,false,dis1);
    if(dis1 < SmoothExtrOnOri){
        return;
    }
    smoRad_godis->PointToArcEndpointDistance(arc2,arcEp,true,dis2);
    if(dis2 < SmoothExtrOnOri){
        return;
    }
    isOnOriEle = true;
}
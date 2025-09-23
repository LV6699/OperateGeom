#include "CreateCurveSolve.h"
#if 0
#pragma optimize("", off)
#endif
void CreateCurveSolve::SetArcParameters(const Point& sp,
                                        const Point& ep,
                                        const Point& cp,
                                        RotDir rotDir,
                                        double R,DefArc& arc)
{
    arc.SetArcStartPt(sp);
    arc.SetArcEndPt(ep);
    arc.SetArcCenterPt(cp);
    arc.SetArcRot(rotDir);
    arc.SetArcRadius(R);
}
void CreateCurveSolve::NotExistEquidCurve(const DefArc& oriArc,
                                          double R,DefArc& arc1,
                                          DefArc& arc2)
{
    GeomIntersect goInt;
    GeometryLogic golg;GeomCalculate gocal;
    Point arcSp = oriArc.ArcStartPt(),arcEp = oriArc.ArcEndPt();
    GeomIntRes intRes,intRes1,intRes2;
    goInt.CalculateCircleCircleInt(arcSp,R,arcEp,R,
                                   PreErr5_10,PreErr5_10,
                                   intRes);
    if(!intRes._isExistInt){
        double dis = gocal.TwoPointDistance2D(arcSp,arcEp);
        goInt.CalculateCircleCircleInt(arcSp,dis,arcEp,dis,
                                       PreErr5_10,PreErr5_10,
                                       intRes);
    }
    Point intPnt;
    if(intRes._intNum == 1){
        intPnt = intRes._intPnt1;
    }
    else{
        Point midPt;
        gocal.ArcMidpoint(oriArc,midPt);
        golg.DistancePointFromPoint(midPt,intRes._intPnt1,
                                    intRes._intPnt2,false,intPnt);
    }
    ///intRes1和intRes2一定有两个相交点.
    goInt.CalculateLineCircleInt(arcSp,arcEp,arcSp,R,
                                 PreErr5_10,PreErr5_10,intRes1);
    goInt.CalculateLineCircleInt(arcSp,arcEp,arcEp,R,
                                 PreErr5_10,PreErr5_10,intRes2);
    if(!intRes1._isExistInt || !intRes2._isExistInt )
        return;
    Point prePnt,nexPnt;
    golg.DistancePointFromPoint(arcEp,intRes1._intPnt1,
                                intRes1._intPnt2,false,prePnt);
    golg.DistancePointFromPoint(arcSp,intRes2._intPnt1,
                                intRes2._intPnt2,false,nexPnt);
    RotDir rotDir = oriArc.ArcRot() ==
            CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    SetArcParameters(prePnt,intPnt,arcSp,rotDir,R,arc1);
    SetArcParameters(intPnt,nexPnt,arcEp,rotDir,R,arc2);}
void CreateCurveSolve::CreateElemConnectArc(const DefElem& ele1_,
                                            const DefElem& ele1,
                                            const DefElem& ele2,
                                            const Point& cp,
                                            double R,ProfType proType,
                                            DefArc& arc)
{
    ConnectArcRotDir arcrot;
    RotDir rotDir = CLOCKWISE;
    arcrot.ArcRotBasePrevoiusElement(ele1_,proType,rotDir);
    Point sp = ele1.EndPt(),ep = ele2.StarPt();
    SetArcParameters(sp,ep,cp,rotDir,R,arc);
}
void CreateCurveSolve::ResetInvalidElem(vector<DefElem> &vElem,
                                        double pre)
{
    vector<int>vIndex;
    ElemCalculate elecal;GeomCalculate gocal;
    for (int i = 0; i < vElem.size(); ++i) {
        Point sp = elecal.ElementEndpoint(vElem[i],false),
                ep = elecal.ElementEndpoint(vElem[i],true);
        if(gocal.TwoPointDistance2D(sp,ep) > pre)
            continue;
        vIndex.push_back(i);
    }
    if(vIndex.empty())
        return;
    elecal.DeleteIndexElement(vIndex,vElem);
    for (int i = 0; i < vElem.size(); ++i) {
        int nex = elecal.NextIndex(vElem.size(),i);
        Point ep1 = elecal.ElementEndpoint(vElem[i],true),
                sp2 = elecal.ElementEndpoint(vElem[nex],false);
        if(gocal.TwoPointDistance2D(ep1,sp2) > pre)
            continue;
        Point mid;
        gocal.TwoPointMidpoint(ep1,sp2,mid);
        elecal.SetElementEndpoint(mid,true,vElem[i]);
        elecal.SetElementEndpoint(mid,false,vElem[nex]);
    }
}
bool CreateCurveSolve::IsCreateDoubleArc(const vector<DefElem> &vOriElem,
                                         const vector<DefElem> &vElem,
                                         ProfType proType,double R,int i)
{
    ///vOriElem 不能为空.
    EquidistantCurve equ;
    DefElem equEle1,equEle2;
    if(vElem.empty()){
        ///说明i == 0.
        bool isExiEqud_ = false;
        DefElem ele = vOriElem.back();
        equ.EquidistantElement(ele,R,proType,isExiEqud_,equEle1);
        if(!isExiEqud_)
            return true;
    }
    else{        equEle1 = vElem.back();
    }
    ElemCalculate elecal;
    bool isExiEqud = false;
    int nex = elecal.NextIndex(vOriElem.size(),i);
    DefElem nexEle = vOriElem[nex];
    equ.EquidistantElement(nexEle,R,proType,isExiEqud,equEle2);
    if(!isExiEqud)
        return true;
    GeomIntRes intRes;
    GeomIntersect goInt;
    goInt.CalculateCurveIntersect(equEle1,equEle2,true,
                                  PreErr5_13,PreErr5_10,
                                  PreErr5_10,intRes);
    GeomCalculate gocal;
    if(intRes._isExistInt){
        if(intRes._intNum == 1){
            int nnex = elecal.NextIndex(vOriElem.size(),nex),
                    pre = elecal.PreviousIndex(vOriElem.size(),i),
                    ppre = elecal.PreviousIndex(vOriElem.size(),pre);
            if(nnex != ppre){
                return false;
            }
            DefElem ele1,ele2;
            elecal.PointSplitElement(equEle1,intRes._intPnt1,false,ele1);
            elecal.PointSplitElement(equEle2,intRes._intPnt1,true,ele2);
            double len1 = gocal.ElementLenth(equEle1);
            double len2 = gocal.ElementLenth(equEle2);
            double len1_ = gocal.ElementLenth(ele1);
            double len2_ = gocal.ElementLenth(ele2);
            if(len1_ < len1 / 2 && len2_ < len2 / 2)
                return false;
        }else{
            return false;
        }
    }
    return true;
} 
void CreateCurveSolve::ChangeEndpointCase(DefElem& ele1,DefElem& ele2,
                                          const Point& ep1,const Point& sp2,
                                          vector<DefElem> &elems)
{
    if(ele1.ElemeType() == OffsetEnum::LINETYPE){
        if(ele2.ElemeType() == OffsetEnum::LINETYPE){
            Point mp = ep1.Midpoint(sp2);
            SetEquedElemEndpoint(mp,ele1.StarPt(),ele1,ele2,elems);
        }else{
            ele1.ChangeEndpoint(sp2,true);
            if(!ele1.StarPt().IsSamePoint2D(ele1.EndPt(),PreErr5_10)){
                elems.emplace_back(ele1);
            }
        }
    }else{
        if(ele2.ElemeType() == OffsetEnum::LINETYPE){
            if(!ele1.StarPt().IsSamePoint2D(ele1.EndPt(),PreErr5_10)){
                elems.emplace_back(ele1);
            }
            ele2.ChangeEndpoint(ep1,false);
        }else{
            if(!ele1.StarPt().IsSamePoint2D(ele1.EndPt(),PreErr5_10)){
                elems.emplace_back(ele1);
            }            ele2.ChangeEndpoint(ep1,false);
            ElementPreSolve().ResetArcCenter(ele2._arc);
        }
    }
}
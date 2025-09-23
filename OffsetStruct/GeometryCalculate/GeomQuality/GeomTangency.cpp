#include "GeomTangency.h"
shared_ptr<GeomCalculate>gotan_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>gotan_num = make_shared<ElemCalculate>();
shared_ptr<TupleQuality>gotan_tupqua = make_shared<TupleQuality>();
void GeomTangency::IsLineCircleTangency(const Point& lSp,
                                        const Point& lEp,
                                        const Point& cp,
                                        double R, double pre,
                                        bool& isTan)
{
    isTan = false;
    double dis;
    GeomCalculate().PointToLineDistance(lSp,lEp,cp,dis);
    if(std::abs(dis - R) <= pre){
        isTan = true;
    }
}
bool GeomTangency::IsLineCircleTangency(const Point& lSp,
                                        const Point& lEp,
                                        const Point& cp,
                                        double R, double pre)
{
    bool isTan = false;
    IsLineCircleTangency(lSp,lEp,cp,R,pre,isTan);
    return isTan;
}
void GeomTangency::IsCircleCircleTangency(const Point& cp1,
                                          double R1,
                                          const Point& cp2,
                                          double R2,
                                          double pre,
                                          bool& isTan)
{
    isTan = false;
    double maxR = (R1 >= R2) ? R1 : R2;
    double minR = (R1 < R2) ? R1 : R2;
    double cirDis = cp1.Distance2D(cp2);
    if(std::abs(cirDis - (R1 + R2)) <= pre ||
            std::abs(maxR - minR - cirDis) <= pre){
        isTan = true;
    }
}
bool GeomTangency::IsEleEleTangency(const DefElem& ele1,
                                    const DefElem& ele2,
                                    bool isJudLine,
                                    double pre)
{
    if(!isJudLine && ele1._elemType == LINETYPE && ele2._elemType == LINETYPE){
        return false;
    }
    if(ele1._elemType == LINETYPE && ele2._elemType == LINETYPE){
        Point l1Sp = ele1._segment._segSp,l1Ep = ele1._segment._segEp,
                l2Sp = ele2._segment._segSp,l2Ep = ele2._segment._segEp;
        return gotan_tupqua->IsLineExistCoincide(l1Sp,l1Ep,l2Sp,
                                                 l2Ep,pre);
    }
    bool isLineCir = false;    if(ele1._elemType == LINETYPE || ele2._elemType == LINETYPE){
        isLineCir = true;
    }
    Point lSp = ele1._segment._segSp,lEp = ele1._segment._segEp;
    if(ele2._elemType == LINETYPE){
        lSp = ele2._segment._segSp;
        lEp = ele2._segment._segEp;
    }
    double R1,R2;
    Point cp1,cp2;
    if(ele1._elemType == LINETYPE){
        if(ele2._elemType == ARCTYPE){
            cp1 = ele2._arc._arcCp;
            R1 = ele2._arc._arcR;
        }
        else if(ele2._elemType == CIRCLETYPE){
            cp1 = ele2._circle._cirCp;
            R1 = ele2._circle._cirR;
        }
    }
    else if(ele1._elemType == ARCTYPE){
        cp1 = ele1._arc._arcCp;
        R1 = ele1._arc._arcR;
        if(ele2._elemType == ARCTYPE){
            cp2 = ele2._arc._arcCp;
            R2 = ele2._arc._arcR;
        }
        else if(ele2._elemType == CIRCLETYPE){
            cp2 = ele2._circle._cirCp;
            R2 = ele2._circle._cirR;
        }
    }
    else if(ele1._elemType == CIRCLETYPE){
        cp1 = ele1._circle._cirCp;
        R1 = ele1._circle._cirR;
        if(ele2._elemType == ARCTYPE){
            cp2 = ele2._arc._arcCp;
            R2 = ele2._arc._arcR;
        }
        else if(ele2._elemType == CIRCLETYPE){
            cp2 = ele2._circle._cirCp;
            R2 = ele2._circle._cirR;
        }
    }
    bool isTan = false;
    if(isLineCir){
        IsLineCircleTangency(lSp,lEp,cp1,R1,pre,isTan);
    }
    else{
        IsCircleCircleTangency(cp1,R1,cp2,R2,pre,isTan);
    }
    return isTan;
}
void GeomTangency::IsSegmentExistCoincide(const Point &l1Sp,
                                          const Point &l1Ep,
                                          const Point &l2Sp,
                                          const Point &l2Ep,
                                          double pre,bool& isCoin)
{
    isCoin = false;
    if(!TupleQuality().IsLineLineParallel(l1Sp,l1Ep,l2Sp,
                                          l2Ep,pre)){
        return;    }
    PointOnElement onele;
    bool isPntOnSig = false;
    onele.IsPointOnSegment(l2Sp,l2Ep,l1Sp,pre,pre,true,isPntOnSig);
    if(isPntOnSig){
        isCoin = true;
        return;
    }
    onele.IsPointOnSegment(l2Sp,l2Ep,l1Ep,pre,pre,true,isPntOnSig);
    if(isPntOnSig){
        isCoin = true;
        return;
    }
    onele.IsPointOnSegment(l1Sp,l1Ep,l2Sp,pre,pre,true,isPntOnSig);
    if(isPntOnSig){
        isCoin = true;
        return;
    }
    onele.IsPointOnSegment(l1Sp,l1Ep,l2Ep,pre,pre,true,isPntOnSig);
    if(isPntOnSig){
        isCoin = true;
        return;
    }
}
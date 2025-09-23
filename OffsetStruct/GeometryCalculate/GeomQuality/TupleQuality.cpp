#include "TupleQuality.h"
bool TupleQuality::IsLineLineParallel(const Point& l1Sp,const Point& l1Ep,
                                      const Point& l2Sp,const Point& l2Ep,
                                      double pre)
{
    GeomCalculate gocal;
    MathCalculate matcal;
    if(matcal.IsNumericEqual(l1Sp.X(),l1Ep.X(),pre) &&
            matcal.IsNumericEqual(l2Sp.X(),l2Ep.X(),pre)){
        return true;
    }
    if(matcal.IsNumericEqual(l1Sp.Y(),l1Ep.Y(),pre) &&
            matcal.IsNumericEqual(l2Sp.Y(),l2Ep.Y(),pre)){
        return true;
    }
    double para1[3],para2[3];
    gocal.CalLineParameters(l1Sp,l1Ep,para1);
    gocal.CalLineParameters(l2Sp,l2Ep,para2);
    if(matcal.IsNumericEqual(para1[0]/para2[0],para1[1]/para2[1],pre)){
        return true;
    }
    return false;
}
bool TupleQuality::IsLineExistCoincide(const Point& l1Sp, const Point& l1Ep,
                                       const Point& l2Sp, const Point& l2Ep,
                                       double pre)
{
    PointOnElement onele;
    if(!IsLineLineParallel(l1Sp,l1Ep,l2Sp,l2Ep,pre))
        return false;
    bool isPntOnSig = false;
    onele.IsPointOnSegment(l2Sp,l2Ep,l1Sp,pre,pre,true,isPntOnSig);
    if(isPntOnSig)
        return true;
    onele.IsPointOnSegment(l2Sp,l2Ep,l1Ep,pre,pre,true,isPntOnSig);
    if(isPntOnSig)
        return true;
    onele.IsPointOnSegment(l1Sp,l1Ep,l2Sp,pre,pre,true,isPntOnSig);
    if(isPntOnSig)
        return true;
    onele.IsPointOnSegment(l1Sp,l1Ep,l2Ep,pre,pre,true,isPntOnSig);
    if(isPntOnSig)
        return true;
    return false;
}
bool TupleQuality::IsLineCompleteCoincide(const Point& l1Sp,
                                          const Point& l1Ep,
                                          const Point& l2Sp,
                                          const Point& l2Ep,
                                          double pre)
{
    if(!IsLineLineParallel(l1Sp,l1Ep,l2Sp,l2Ep,pre))
        return false;
    GeomCalculate go;
    if(go.IsIdenticalPoint(l1Sp,l2Sp,pre) &&
            go.IsIdenticalPoint(l1Ep,l2Ep,pre))
        return true;
    if(go.IsIdenticalPoint(l1Sp,l2Ep,pre) &&            go.IsIdenticalPoint(l1Ep,l2Sp,pre))
        return true;
    return false;
}
bool TupleQuality::IsLineLocatedOnLine(const Point& refSp,
                                       const Point& refEp,
                                       const Point& lSp,
                                       const Point& lEp,
                                       double pre)
{
    if(!IsLineLineParallel(refSp,refEp,lSp,lEp,pre))
        return false;
    PointOnElement onele;
    bool isSpOn = false,isEpOn = false;
    onele.IsPointOnSegment(refSp,refEp,lSp,pre,pre,true,isSpOn);
    onele.IsPointOnSegment(refSp,refEp,lEp,pre,pre,true,isEpOn);
    return (isSpOn && isEpOn);
}
bool TupleQuality::IsCirclesCointcoincide(const Point& cp1,double r1,
                                          const Point& cp2,double r2,
                                          double pre)
{
    GeomCalculate go;
    MathCalculate math;
    return (go.IsIdenticalPoint(cp1,cp2,pre) &&
            math.IsNumericEqual(r1,r2,pre));
}
bool TupleQuality::IsArcExistCoincide(const DefArc& arc1,
                                      const DefArc& arc2,
                                      double pre)
{
    GeomCalculate gocal;
    MathCalculate matcal;
    if(!gocal.IsIdenticalPoint(arc1._arcCp,arc2._arcCp,pre))
        return false;
    if(!matcal.IsNumericEqual(arc1._arcR,arc2._arcR,pre))
        return false;
    PointOnElement pton;
    bool isArc1SpOnArc = false,isArc1EpOnArc = false;
    pton.IsPointOnArc(arc2,arc1._arcSp,pre,PreErr5_10,false,isArc1SpOnArc);
    pton.IsPointOnArc(arc2,arc1._arcEp,pre,PreErr5_10,false,isArc1EpOnArc);
    if(isArc1SpOnArc || isArc1EpOnArc)
        return true;
    return false;
}
bool TupleQuality::IsArcCompleteCoincide(const DefArc& arc1,
                                         const DefArc& arc2,
                                         double pre)
{
    GeomCalculate go;
    MathCalculate math;
    if(!go.IsIdenticalPoint(arc1._arcCp,arc2._arcCp,pre))
        return false;
    if(!math.IsNumericEqual(arc1._arcR,arc2._arcR,pre))
        return false;
    if(go.IsIdenticalPoint(arc1._arcSp,arc2._arcSp,pre) &&
            go.IsIdenticalPoint(arc1._arcEp,arc2._arcEp,pre) &&
            arc1.ArcRot() == arc2.ArcRot())
        return true;
    if(go.IsIdenticalPoint(arc1._arcSp,arc2._arcEp,pre) &&            go.IsIdenticalPoint(arc1._arcEp,arc2._arcSp,pre) &&
            arc1.ArcRot() != arc2.ArcRot())
        return true;
    return false;
}
bool TupleQuality::IsArcLocatedOnArc(const DefArc& refArc,
                                     const DefArc& arc,
                                     double pre)
{
    GeomCalculate go;
    PointOnElement pton;
    MathCalculate math;
    if(!go.IsIdenticalPoint(refArc._arcCp,arc._arcCp,pre))
        return false;
    if(!math.IsNumericEqual(refArc._arcR,arc._arcR,pre))
        return false;
    bool isSpOn = false,isEpOn = false,isMidOn = false;
    pton.IsPointOnArc(refArc,arc._arcSp,pre,pre,false,isSpOn);
    if(isSpOn){
        pton.IsPointOnArc(refArc,arc._arcEp,pre,pre,false,isEpOn);
        if(isEpOn){
            Point midPnt;
            go.ArcMidpoint(arc,midPnt);
            pton.IsPointOnArc(refArc,midPnt,pre,pre,false,isMidOn);
            if(isMidOn)
                return true;
        }
    }
    return false;
}
bool TupleQuality::IsElementsCoincide(const DefElem& ele1,
                                      const DefElem& ele2,
                                      bool isComplete,
                                      double pre)
{
    if(ele1._elemType != ele2._elemType)
        return false;
    if(ele1._elemType == LINETYPE){
        if(isComplete){
            return IsLineCompleteCoincide(ele1._segment._segSp,
                                          ele1._segment._segEp,
                                          ele2._segment._segSp,
                                          ele2._segment._segEp,pre);
        }
        return IsLineExistCoincide(ele1._segment._segSp,
                                   ele1._segment._segEp,
                                   ele2._segment._segSp,
                                   ele2._segment._segEp,pre);
    }
    else if(ele1._elemType == ARCTYPE){
        if(isComplete){
            return IsArcCompleteCoincide(ele1._arc,ele2._arc,pre);
        }
        return IsArcExistCoincide(ele1._arc,ele2._arc,pre);
    }
    else if(ele1._elemType == CIRCLETYPE){
        return IsCirclesCointcoincide(ele1._circle._cirCp,
                                      ele1._circle._cirR,
                                      ele2._circle._cirCp,
                                      ele2._circle._cirR,pre);
    }
    return false;}
bool TupleQuality::IsEleExistCoincide(const DefElem& ele1,
                                      const DefElem& ele2,
                                      double pre)
{
    if(ele1._elemType == LINETYPE && ele2._elemType == LINETYPE){
        return IsLineExistCoincide(ele1._segment._segSp,
                                   ele1._segment._segEp,
                                   ele2._segment._segSp,
                                   ele2._segment._segEp,
                                   pre);
    }
    else if(ele1._elemType == ARCTYPE && ele2._elemType == ARCTYPE){
        return IsArcExistCoincide(ele1._arc,ele2._arc,pre);
    }
    return false;
}
bool TupleQuality::IsEleLocatedOnEle(const DefElem& refEle,
                                     const DefElem& ele,
                                     double pre)
{
    if(refEle._elemType != ele._elemType)
        return false;
    if(refEle._elemType == LINETYPE){
        return IsLineLocatedOnLine(refEle._segment._segSp,
                                   refEle._segment._segEp,
                                   ele._segment._segSp,
                                   ele._segment._segEp,
                                   pre);
    }
    else if(refEle._elemType == ARCTYPE){
        return IsArcLocatedOnArc(refEle._arc,ele._arc,pre);
    }
    else if(refEle._elemType == CIRCLETYPE){
        return IsCirclesCointcoincide(refEle._circle._cirCp,
                                      refEle._circle._cirR,
                                      ele._circle._cirCp,
                                      ele._circle._cirR,pre);
    }
    return false;
}
bool TupleQuality::IsHorizontalLine(const Point& lSp,
                                    const Point& lEp,double pre)
{
    double slop;
    MathCalculate matcal;
    GeomCalculate().SegmentAngle(lSp,lEp,slop);
    if(matcal.IsNumericEqual(slop,0,pre) ||
            matcal.IsNumericEqual(slop,180,pre) ||
            matcal.IsNumericEqual(slop,360,pre))
        return true;
    return false;
}
bool TupleQuality::IsVerticalLine(const Point& lSp,
                                  const Point& lEp,
                                  double pre)
{
    double slop;    MathCalculate matcal;
    GeomCalculate().SegmentAngle(lSp,lEp,slop);
    if(matcal.IsNumericEqual(slop,90,pre) ||
            matcal.IsNumericEqual(slop,270,pre))
        return true;
    return false;
}
void TupleQuality::GetEleSpecifyTangentLine(const DefElem& ele,bool isSp,
                                            bool isReverse,
                                            Point& lSp,Point& lEp)
{
    ///lSp表示切线起点.
    if(ele._elemType != LINETYPE && ele._elemType != ARCTYPE)
        return;
    GeomCalculate gocal;
    if(ele._elemType == LINETYPE){
        lSp = ele._segment._segSp;
        if(!isSp){
            lSp = ele._segment._segEp;
        }
        lEp = ele._segment._segEp;
        if(isReverse){
            lEp = ele._segment._segSp;
        }
        return;
    }
    Point cp = ele._arc._arcCp,basePt = ele._arc._arcSp,rotResPt;
    if(!isSp){
        basePt = ele._arc._arcEp;
    }
    double A = 90;
    RotDir cpRotDir = ele._arc._arcRot == CLOCKWISE ?
                ANTICLOCKWISE : CLOCKWISE;
    if(isReverse){
        cpRotDir = ele._arc._arcRot;
    }
    gocal.PointAroundFixedRotate(cp,basePt,A,cpRotDir,rotResPt);
    lSp = basePt;
    lEp = rotResPt;
}
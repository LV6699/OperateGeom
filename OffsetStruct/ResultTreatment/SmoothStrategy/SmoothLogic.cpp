#include "SmoothLogic.h"
void SmoothLogic::SegmentSegmentSmoothArc(DefLoop &loop,
                                          const Point& l1Sp,
                                          const Point& l1Ep,
                                          const Point& l2Sp,
                                          const Point& l2Ep,
                                          double radius,
                                          double strechLimit,
                                          int &i, int nex)
{
    SmoothRadius smorad;
    GeomCalculate gocal;
    ///SmoothProCal smocal;
    CreateSmoothArc crtrad;
    smorad.ResetSmoonthRadiusBaseSeg(l1Sp,l1Ep,SmoothTimes,
                                     radius);
    smorad.ResetSmoonthRadiusBaseSeg(l2Sp,l2Ep,SmoothTimes,
                                     radius);
    if(radius <= SmoothValLimit){
        return;
    }
    DefArc smoothArc;
    bool isExiSmooth = false;
    crtrad.CreateSegmentSegmentSmoothArc(l1Sp,l1Ep,l2Sp,l2Ep,
                                         radius,strechLimit,
                                         isExiSmooth,smoothArc);
    if(!isExiSmooth){
        return;
    }
    double streLen;
    gocal.TwoPointDistance(l1Ep,smoothArc._arcCp,streLen);
    if(streLen > strechLimit){/**
        smocal.CalSigAnglBisectorStrechLenth(l1Sp,l1Ep,l2Sp,l2Ep,
                                             strechLimit,smoothArc);
        loop.m_vElem[i]._segment._segEp = smoothArc._arcSp;
        loop.m_vElem[nex]._segment._segSp = smoothArc._arcEp;
        loop.m_vElem.insert(loop.m_vElem.begin() + i + 1,
                            DefElem(ARCTYPE, smoothArc));*/
        return;
    }
    else{
        loop.m_vElem[i]._segment._segEp = smoothArc.ArcStartPt();
        loop.m_vElem[nex]._segment._segSp = smoothArc.ArcEndPt();
        loop.m_vElem.insert(loop.m_vElem.begin() + i + 1,
                            DefElem(ARCTYPE, smoothArc));
    }
    i ++;
}
void SmoothLogic::SegmentArcSmoothArc(DefLoop &loop,
                                      const Point& lSp,
                                      const Point& lEp,
                                      const DefArc& arc,
                                      bool isLineFont,
                                      double radius,
                                      double strechLimit,
                                      int &i, int nex)
{
    SmoothRadius smorad;
    CreateSmoothArc crtrad;
    smorad.ResetSmoonthRadiusBaseSeg(lSp,lEp,SmoothTimes,radius);
    smorad.ResetSmoonthRadiusBaseArc(arc,SmoothTimes,radius);    if(radius <= SmoothValLimit){
        return;
    }
    DefArc smoothArc;
    bool isExiSmooth = false;
    crtrad.CreateSegmentArcSmoothArc(lSp,lEp,arc,radius,strechLimit,
                                     isLineFont,isExiSmooth,smoothArc);
    if(!isExiSmooth){
        return;
    }
    if(isLineFont){
        loop.m_vElem[i]._segment._segEp = smoothArc._arcSp;
        loop.m_vElem[nex]._arc._arcSp = smoothArc._arcEp;
    }
    else{
        loop.m_vElem[i]._arc._arcEp = smoothArc._arcSp;
        loop.m_vElem[nex]._segment._segSp = smoothArc._arcEp;
    }
    loop.m_vElem.insert(loop.m_vElem.begin() + i + 1,
                        DefElem(ARCTYPE, smoothArc));
    i ++;
}
void SmoothLogic::ArcArcSmoothArc(DefLoop &loop,
                                  const DefArc& arc1,
                                  const DefArc &arc2,
                                  double radius,
                                  double strechLimit,
                                  int &i, int nex)
{
    bool isExistFaiArc = false;
    SmoothRadius smorad;
    smorad.ResetSmoonthRadiusBaseArc(arc1,SmoothTimes,radius);
    smorad.ResetSmoonthRadiusBaseArc(arc2,SmoothTimes,radius);
    if(radius <= SmoothValLimit){
        return;
    }
    DefArc smoothArc;
    CreateSmoothArc crtrad;
    crtrad.CreateArcArcSmoothArc(arc1,arc2,radius,strechLimit,
                                 isExistFaiArc,smoothArc);
    if(!isExistFaiArc){
        return;
    }
    loop.m_vElem[i]._arc._arcEp = smoothArc._arcSp;
    loop.m_vElem[nex]._arc._arcSp = smoothArc._arcEp;
    loop.m_vElem.insert(loop.m_vElem.begin() + i + 1,
                        DefElem(ARCTYPE, smoothArc));
    i ++;
}
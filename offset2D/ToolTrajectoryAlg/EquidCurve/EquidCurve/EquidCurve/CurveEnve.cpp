#include "CurveEnve.h"
#if 0
#pragma optimize("", off)
#endif
void CurveEnve::SetArcParameters(const Point& sp,const Point& ep,
                                 const Point& cp,double R,
                                 bool iscw,DefArc& arc)
{
    arc._arcSp = sp;
    arc._arcEp = ep;
    arc._arcCp = cp;
    arc._arcR = R;
    arc._isCW = iscw;
}
void CurveEnve::GetSigmentEnvelopeLoop(const Point& lSp,
                                       const Point& lEp,
                                       double oftD,
                                       DefLoop& loop)
{
    loop.m_vElem.clear();
    //ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    bool leftward_ = true,leftward__ = false;
    Point lSp_,lEp_,lSp__, lEp__;
    EquidistantCurve equcur;
    equcur.EquidistantSegment(lSp,lEp,oftD,leftward_,lSp_,lEp_);
    equcur.EquidistantSegment(lSp,lEp,oftD,leftward__,lSp__,lEp__);
    loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lSp_,lEp_)));
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lEp_,lEp__,lEp,
                                                  oftD,true)));
    loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lEp__,lSp__)));
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lSp__,lSp_,lSp,
                                                  oftD,true)));
}
void CurveEnve::GetSigmentSpecifyEnvelope(const Point& lSp,const Point& lEp,
                                          bool leftward,double oftD,
                                          DefLoop& loop)
{
    loop.m_vElem.clear();
    //ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    bool leftward_ = true,leftward__ = false;
    Point lSp_,lEp_,lSp__, lEp__;
    EquidistantCurve equcur;
    equcur.EquidistantSegment(lSp,lEp,oftD,leftward_,lSp_,lEp_);
    equcur.EquidistantSegment(lSp,lEp,oftD,leftward__,lSp__,lEp__);
    if(leftward){
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lSp_,lEp_)));
    }
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lEp_,lEp__,lEp,
                                                  oftD,true)));
    if(!leftward){
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lEp__,lSp__)));
    }
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lSp__,lSp_,lSp,
                                                  oftD,true)));
}
void CurveEnve::GetNotExistEquArcStructure(const DefArc& arc,
                                           const DefArc& equArc,
                                           double oftD,
                                           DefArc& arc1,
                                           DefArc& arc2)
{
    Point intPnt;
    GeomIntRes intRes;
    GeomIntersect goInt;
    GeomCalculate gocal;
    goInt.CalculateCircleCircleInt(arc._arcSp,oftD,
                                   arc._arcEp,oftD,
                                   PreErr5_10,PreErr_10,intRes);
    if(!intRes._isExistInt){
        double dis,dis_;
        gocal.TwoPointDistance(arc._arcSp,arc._arcEp,dis);
        dis_ = dis / 2;
        goInt.CalculateCircleCircleInt(arc._arcSp,dis_,
                                       arc._arcEp,dis_,
                                       PreErr5_10,PreErr_10,intRes);
    }
    intPnt = intRes._intPnt1;
    if(intRes._intNum != 1) {
        Point midPnt;
        double dis1,dis2;
        gocal.ArcMidpoint(arc,midPnt);
        gocal.TwoPointDistance(intRes._intPnt1,midPnt,dis1);
        gocal.TwoPointDistance(intRes._intPnt2,midPnt,dis2);
        intPnt = dis1 >= dis2 ? intRes._intPnt1 : intRes._intPnt2;
    }
    SetArcParameters(equArc._arcEp,intPnt,arc._arcEp,
                     oftD,equArc._isCW,arc1);
    SetArcParameters(intPnt,equArc._arcSp,arc._arcSp,
                     oftD,equArc._isCW,arc2);
}
void CurveEnve::GetArcEnvelopeLoop(const DefArc& arc,double oftD,DefLoop& loop)
{
    loop.m_vElem.clear();
    ElemCalculate numcal;
    EquidistantCurve equcur;
    DefArc arc_,arc__;
    bool isExiEqu_,isExiEqu__ = false;
    //ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    bool leftward_ = true,leftward__ = false;
    equcur.EquidistantArc(arc,oftD,leftward_,true,isExiEqu_,arc_);
    equcur.EquidistantArc(arc,oftD,leftward__,true,isExiEqu__,arc__);
    if(isExiEqu_)
        loop.m_vElem.push_back(DefElem(ARCTYPE,arc_));
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,oftD,leftward__,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,oftD,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
    if(isExiEqu__){
        if(isExiEqu_){
            numcal.ReverseArcDirection(arc__);
            DefElem ele1(ARCTYPE,DefArc(arc_._arcEp,arc__._arcSp,
                                        arc._arcEp,oftD,
                                        /*arc._isCW*/true));
            loop.m_vElem.push_back(ele1);
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
            DefElem ele2(ARCTYPE,DefArc(arc__._arcEp,arc_._arcSp,
                                        arc._arcSp,oftD,
                                        true));
            loop.m_vElem.push_back(ele2);
        }
        else
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
    }
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,oftD,leftward_,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,oftD,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
}
void CurveEnve::GetArcSpecifyEnvelope(const DefArc& arc,bool leftward,
                                      double oftD,DefLoop& loop)
{
    loop.m_vElem.clear();
    ElemCalculate numcal;
    EquidistantCurve equcur;
    DefArc arc_,arc__;
    bool isExiEqu_,isExiEqu__ = false;
    //ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    bool leftward_ = true,leftward__ = false;
    equcur.EquidistantArc(arc,oftD,leftward_,true,isExiEqu_,arc_);
    equcur.EquidistantArc(arc,oftD,leftward__,true,isExiEqu__,arc__);
    if(isExiEqu_){
        if(leftward == leftward_){
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc_));
        }
    }
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,oftD,leftward__,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,oftD,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
    if(isExiEqu__){
        if(isExiEqu_){
            numcal.ReverseArcDirection(arc__);
            DefElem ele1(ARCTYPE,DefArc(arc_._arcEp,arc__._arcSp,
                                        arc._arcEp,oftD,
                                        /*arc._isCW*/true));
            loop.m_vElem.push_back(ele1);
            if(leftward == leftward__){
                loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
            }
            DefElem ele2(ARCTYPE,DefArc(arc__._arcEp,arc_._arcSp,
                                        arc._arcSp,oftD,
                                        true));
            loop.m_vElem.push_back(ele2);
        }
        else
            if(leftward == leftward__){
                loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
            }
    }
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,oftD,leftward_,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,oftD,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
}
void CurveEnve::GetEleEnvelopeLoop(const DefElem& ele,
                                   double oftD,
                                   DefLoop& loop)
{
    if(ele._type == LINETYPE){
        GetSigmentEnvelopeLoop(ele._segment._segSp,
                               ele._segment._segEp,
                               oftD,loop);
    }
    else if(ele._type == ARCTYPE){
        GetArcEnvelopeLoop(ele._arc,oftD,loop);
    }
}
void CurveEnve::GetEleSpecifyEnvelope(const DefElem& ele,bool leftward,
                                      double oftD,DefLoop& loop)
{
    if(ele._type == LINETYPE){
        GetSigmentSpecifyEnvelope(ele._segment._segSp,
                                  ele._segment._segEp,
                                  leftward,oftD,loop);
    }
    else if(ele._type == ARCTYPE){
        GetArcSpecifyEnvelope(ele._arc,leftward,oftD,loop);
    }
}

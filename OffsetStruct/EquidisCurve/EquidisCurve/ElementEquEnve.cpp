#include "ElementEquEnve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void ElementEquEnve::SetArcParameters(const Point& sp,const Point& ep,
                                      const Point& cp,double R,
                                      RotDir rotDir,DefArc& arc)
{
    arc._arcSp = sp;
    arc._arcEp = ep;
    arc._arcCp = cp;
    arc._arcR = R;
    arc._arcRot = rotDir;
}
void ElementEquEnve::GetSigmentEnvelopeLoop(const Point& lSp,
                                            const Point& lEp,
                                            double offsetVal,
                                            DefLoop& loop)
{
    loop.m_vElem.clear();
    ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    Point lSp_,lEp_,lSp__, lEp__;
    EquidistantCurve equcur;
    equcur.EquidistantSegment(lSp,lEp,offsetVal,prof_,lSp_,lEp_);
    equcur.EquidistantSegment(lSp,lEp,offsetVal,prof__,lSp__,lEp__);
    loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lSp_,lEp_)));
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lEp_,lEp__,lEp,
                                                  offsetVal,CLOCKWISE)));
    loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lEp__,lSp__)));
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lSp__,lSp_,lSp,
                                                  offsetVal,CLOCKWISE)));
}
void ElementEquEnve::GetSigmentSpecifyEnvelope(const Point& lSp,
                                               const Point& lEp,
                                               ProfType proType,
                                               double offsetVal,
                                               DefLoop& loop)
{
    loop.m_vElem.clear();
    ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    Point lSp_,lEp_,lSp__, lEp__;
    EquidistantCurve equcur;
    equcur.EquidistantSegment(lSp,lEp,offsetVal,prof_,lSp_,lEp_);
    equcur.EquidistantSegment(lSp,lEp,offsetVal,prof__,lSp__,lEp__);
    if(proType == WITHINPROFILE){
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lSp_,lEp_)));
    }
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lEp_,lEp__,lEp,
                                                  offsetVal,CLOCKWISE)));
    if(proType == OUTSIDEPROFILE){
        loop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(lEp__,lSp__)));
    }
    loop.m_vElem.push_back(DefElem(ARCTYPE,DefArc(lSp__,lSp_,lSp,
                                                  offsetVal,CLOCKWISE)));
}
void ElementEquEnve::GetNotExistEquArcStructure(const DefArc& arc,
                                                const DefArc& equArc,
                                                double offsetVal,                                                DefArc& arc1,
                                                DefArc& arc2)
{
    Point intPnt;
    GeomIntRes intRes;
    GeomIntersect goInt;
    GeomCalculate gocal;
    goInt.CalculateCircleCircleInt(arc._arcSp,offsetVal,
                                   arc._arcEp,offsetVal,
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
                     offsetVal,equArc._arcRot,arc1);
    SetArcParameters(intPnt,equArc._arcSp,arc._arcSp,
                     offsetVal,equArc._arcRot,arc2);
}
void ElementEquEnve::GetArcEnvelopeLoop(const DefArc& arc,
                                        double offsetVal,
                                        DefLoop& loop)
{
    loop.m_vElem.clear();
    ElemCalculate numcal;
    EquidistantCurve equcur;
    DefArc arc_,arc__;
    bool isExiEqu_,isExiEqu__ = false;
    ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    equcur.EquidistantArc(arc,offsetVal,prof_,true,isExiEqu_,arc_);
    equcur.EquidistantArc(arc,offsetVal,prof__,true,isExiEqu__,arc__);
    if(isExiEqu_)
        loop.m_vElem.push_back(DefElem(ARCTYPE,arc_));
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,offsetVal,prof__,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,offsetVal,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
    if(isExiEqu__){
        if(isExiEqu_){
            numcal.ReverseArcDirection(arc__);
            DefElem ele1(ARCTYPE,DefArc(arc_._arcEp,arc__._arcSp,
                                        arc._arcEp,offsetVal,
                                        /*arc._arcRot*/CLOCKWISE));
            loop.m_vElem.push_back(ele1);
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));            DefElem ele2(ARCTYPE,DefArc(arc__._arcEp,arc_._arcSp,
                                        arc._arcSp,offsetVal,
                                        CLOCKWISE));
            loop.m_vElem.push_back(ele2);
        }
        else
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
    }
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,offsetVal,prof_,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,offsetVal,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
}
void ElementEquEnve::GetArcSpecifyEnvelope(const DefArc& arc,
                                           ProfType proType,
                                           double offsetVal,
                                           DefLoop& loop)
{
    loop.m_vElem.clear();
    ElemCalculate numcal;
    EquidistantCurve equcur;
    DefArc arc_,arc__;
    bool isExiEqu_,isExiEqu__ = false;
    ProfType prof_ = WITHINPROFILE,prof__ = OUTSIDEPROFILE;
    equcur.EquidistantArc(arc,offsetVal,prof_,true,isExiEqu_,arc_);
    equcur.EquidistantArc(arc,offsetVal,prof__,true,isExiEqu__,arc__);
    if(isExiEqu_){
        if(proType == prof_){
            loop.m_vElem.push_back(DefElem(ARCTYPE,arc_));
        }
    }
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,offsetVal,prof__,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,offsetVal,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
    if(isExiEqu__){
        if(isExiEqu_){
            numcal.ReverseArcDirection(arc__);
            DefElem ele1(ARCTYPE,DefArc(arc_._arcEp,arc__._arcSp,
                                        arc._arcEp,offsetVal,
                                        /*arc._arcRot*/CLOCKWISE));
            loop.m_vElem.push_back(ele1);
            if(proType == prof__){
                loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
            }
            DefElem ele2(ARCTYPE,DefArc(arc__._arcEp,arc_._arcSp,
                                        arc._arcSp,offsetVal,
                                        CLOCKWISE));
            loop.m_vElem.push_back(ele2);
        }
        else
            if(proType == prof__){
                loop.m_vElem.push_back(DefElem(ARCTYPE,arc__));
            }    }
    else{
        bool isExiEqu = false;
        DefArc equArc,equArc1,equArc2;
        equcur.EquidistantArc(arc,offsetVal,prof_,true,isExiEqu,equArc);
        GetNotExistEquArcStructure(arc,equArc,offsetVal,equArc1,equArc2);
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc1));
        loop.m_vElem.push_back(DefElem(ARCTYPE,equArc2));
    }
}
void ElementEquEnve::GetEleEnvelopeLoop(const DefElem& ele,
                                        double offsetVal,
                                        DefLoop& loop)
{
    if(ele._elemType == LINETYPE){
        GetSigmentEnvelopeLoop(ele._segment._segSp,
                               ele._segment._segEp,
                               offsetVal,loop);
    }
    else if(ele._elemType == ARCTYPE){
        GetArcEnvelopeLoop(ele._arc,offsetVal,loop);
    }
}
void ElementEquEnve::GetEleSpecifyEnvelope(const DefElem& ele,
                                           ProfType proType,
                                           double offsetVal,
                                           DefLoop& loop)
{
    if(ele._elemType == LINETYPE){
        GetSigmentSpecifyEnvelope(ele._segment._segSp,
                                  ele._segment._segEp,
                                  proType,offsetVal,loop);
    }
    else if(ele._elemType == ARCTYPE){
        GetArcSpecifyEnvelope(ele._arc,proType,offsetVal,loop);
    }
}
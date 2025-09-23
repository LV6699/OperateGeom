#include "SmoothStrategy.h"
bool SmoothStrategy::CircleTranslateArcSmooth(vector<DefLoop>& vLoop,
                                              DefLoop& loop)
{
    bool isTranslate = false;
    GeometryLogic().IsCircleStructureLoop(loop,true,isTranslate);
    if(isTranslate){
        vLoop.push_back(loop);
        return true;
    }
    return false;
}
void SmoothStrategy::ArrayLoopElementSmooth(const double R,bool isAddOriLoop,
                                            vector<DefLoop>&vLoop)
{
    double strechLimit = R * 100000;
    vector<DefLoop> vArrLoop;
    for(auto& it : vLoop)
        vArrLoop.push_back(it);
    vLoop.clear();
    SmoothLogic smolg;
    ElemCalculate numcal;CalculateAngle calagl;
    double limOftNum = 1;
    if(isAddOriLoop)
        limOftNum = 0;
    for (int lo = 0; lo < vArrLoop.size(); ++lo) {/**
        if(loop.m_vElem.size() == 2){
            int tem = 1+1;
        }*/
        if(vArrLoop[lo].PastOffsetNum() > limOftNum){
            vLoop.push_back(vArrLoop[lo]);
            continue;
        }
        if(CircleTranslateArcSmooth(vLoop,vArrLoop[lo]))
            continue;
        DefLoop loop = vArrLoop[lo];
        numcal.InitialLoopArcRadius(loop);
        for (int i = 0; i < loop.ElemNum(); ++i) {/**
            if(i == loop.m_vElem.size() - 2){
                int tem = 1+1;
            }*/
            double radius = R;
            int nex = numcal.NextIndex(loop.m_vElem.size(),i);
            if(loop.IndexElem(i).ElemeType() == LINETYPE){
                Point lSp = loop.m_vElem[i]._segment._segSp;
                Point lEp = loop.m_vElem[i]._segment._segEp;
                if(loop.m_vElem[nex]._elemType == LINETYPE){
                    Point lSp1 = loop.m_vElem[nex]._segment._segSp;
                    Point lEp1 = loop.m_vElem[nex]._segment._segEp;
                    double angle;
                    calagl.AdjoinSegmentSegmentAngle(lSp,lEp,lSp1,lEp1,angle);
                    if(angle > SmoothAngle)
                        continue;
                    smolg.SegmentSegmentSmoothArc(loop,lSp,lEp,lSp1,
                                                  lEp1,radius,strechLimit,
                                                  i,nex);
                }
                else if(loop.m_vElem[nex]._elemType == ARCTYPE){
                    double angle;                    DefArc arc = loop.m_vElem[nex]._arc;
                    calagl.AdjoinSegmentArcAngle(lSp,lEp,arc,true,angle);
                    if(angle > SmoothAngle)
                        continue;
                    smolg.SegmentArcSmoothArc(loop,lSp,lEp,arc,true,radius,
                                              strechLimit,i,nex);
                }
            }
            else if(loop.IndexElem(i).ElemeType() == ARCTYPE){
                DefArc arc = loop.m_vElem[i]._arc;
                if(loop.m_vElem[nex]._elemType == LINETYPE){
                    double angle;
                    Point lSp = loop.m_vElem[nex]._segment._segSp;
                    Point lEp = loop.m_vElem[nex]._segment._segEp;
                    calagl.AdjoinSegmentArcAngle(lSp,lEp,arc,false,angle);
                    if(angle > SmoothAngle)
                        continue;
                    smolg.SegmentArcSmoothArc(loop,lSp,lEp,arc,false,radius,
                                              strechLimit,i,nex);
                }
                else if(loop.m_vElem[nex]._elemType == ARCTYPE){
                    DefArc arc1 = loop.m_vElem[nex]._arc;
                    double angle;
                    calagl.AdjoinArcArcAngle(arc,arc1,angle);
                    if(angle > SmoothAngle)
                        continue;
                    smolg.ArcArcSmoothArc(loop,arc,arc1,radius,
                                          strechLimit,i,nex);
                }
            }
        }
        vLoop.push_back(loop);
    }
}
void SmoothStrategy::CalSigAnglBisectorStrechLenth(const Point& l1Sp,
                                                   const Point& l1Ep,
                                                   const Point& l2Sp,
                                                   const Point& l2Ep,
                                                   const double& strechLimit,
                                                   DefArc& arc)
{
    GeomCalculate gocal;
    double sTheta,eTheta,aveAgl;
    gocal.CalAngleBetweenTwoVectors(l1Sp,l1Ep,l2Sp,l2Ep,
                                    sTheta,eTheta,aveAgl);
    RotDir calRot;
    gocal.TriangleRotDirection(l1Sp,l1Ep,l2Ep,arc._arcRot);
    calRot = arc._arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    double aglAbsVal;
    gocal.TwoAngleIncludedAngle(sTheta,aveAgl,calRot,aglAbsVal);
    aglAbsVal = aglAbsVal * M_PI / 180.0;
    double calAveAgl = aveAgl * M_PI / 180.0;
    double xInc,yInc;
    if((abs(aveAgl - 270.0) > PreErr_6) &&
            (abs(aveAgl - 90.0) > PreErr_6)){
        //double slope = std::tan(calAveAgl);        xInc = strechLimit * std::cos(calAveAgl);
        yInc = strechLimit * std::sin(calAveAgl);
    }
    else{
        xInc = 0.0;
        yInc = strechLimit * std::sin(calAveAgl);
    }
    Point extendPnt;
    extendPnt._x = l1Ep._x + xInc;
    extendPnt._y = l1Ep._y + yInc;
    arc._arcCp = extendPnt;
    double sig1Paras[3],sig2Paras[3];
    gocal.CalLineParameters(l1Sp,l1Ep,sig1Paras);
    gocal.CalLineParameters(l2Sp,l2Ep,sig2Paras);
    double x0 = extendPnt._x;
    double y0 = extendPnt._y;
    double A1 = sig1Paras[0];
    double B1 = sig1Paras[1];
    double C1 = sig1Paras[2];
    double A2 = sig2Paras[0];
    double B2 = sig2Paras[1];
    double C2 = sig2Paras[2];
    arc._arcSp._x = (B1*B1*x0 - A1*B1*y0 - A1*C1) / (A1*A1 + B1*B1);
    arc._arcSp._y = (-A1*B1*x0 + A1*A1*y0 - B1*C1) / (A1*A1 + B1*B1);
    arc._arcEp._x = (B2*B2*x0 - A2*B2*y0 - A2*C2) / (A2*A2 + B2*B2);
    arc._arcEp._y = (-A2*B2*x0 + A2*A2*y0 - B2*C2) / (A2*A2 + B2*B2);
}
void SmoothStrategy::CalculationArcArcRotDir(const DefArc& arc1,
                                             const DefArc& arc2,
                                             RotDir& rotDir)
{
    GeomCalculate gocal;
    double slop1,slop2;
    gocal.SegmentAngle(arc1._arcSp,arc1._arcEp,slop1);
    gocal.SegmentAngle(arc2._arcSp,arc2._arcEp,slop2);
    if(MathCalculate().IsNumericEqual(slop1,slop2,PreErr5_12)){
        Point arcMidPnt;
        gocal.ArcMidpoint(arc1,arcMidPnt);
        gocal.ArcMidpoint(arc1,arcMidPnt);
        gocal.TriangleRotDirection(arcMidPnt,
                                arc1._arcEp,
                                arc2._arcEp,rotDir);
    }
    else{
        gocal.TriangleRotDirection(arc1._arcSp,
                                arc1._arcEp,
                                arc2._arcEp,rotDir);
    }
}
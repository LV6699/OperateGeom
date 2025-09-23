#include "LoopEndToEnd.h"
#include "../../../OperateView/OperateView/DisplayGeom.h"
#if DebugModel
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void LoopEndToEnd::SetLoopLoopEndToEnd(DefLoop& loop)
{
    if(loop.Size() < 3 || loop.IsOpenLoop())
        return;
    ///DisplayGeom().DisplayLoop(loop,_colors.back(),true,true);
    GeomIntersect goInt;
    const auto& firEle = loop.m_vElem[0];
    const auto& lasEle = loop.m_vElem.back();
    const auto& eleSp = firEle.StarPt(),eleEp = lasEle.EndPt();
    if(eleSp.IsSamePoint2D(eleEp,PreErr_12))
        return;
    GeomIntRes intRes;
    goInt.CalculateEleGeomIntersect(firEle,lasEle,PreErr_10,
                                    PreErr_10,intRes);
    if(!intRes._isExistInt){
        TerminalCurveNotIntersect(loop[0],loop[loop.Size() -1]);
        return;
    }
    if(intRes.Num() == 1){
        loop[0].ChangeEndpoint(intRes.P1(),false);
        loop[loop.Size()-1].ChangeEndpoint(intRes.P1(),true);
        return;
    }
    ///DisplayGeom().DisplayPoint(intRes.P1(),_colors[2]);
    ///DisplayGeom().DisplayPoint(intRes.P2(),_colors[3]);
    double dis1 = 0,dis2 = 0,dis3 = 0,dis4 = 0;
#if 0
    GeomDistance godis;
    godis.PointToEleEndpointDistance(firEle,intRes.P1(),true,dis1);
    godis.PointToEleEndpointDistance(lasEle,intRes.P1(),false,dis2);
    godis.PointToEleEndpointDistance(firEle,intRes.P2(),true,dis3);
    godis.PointToEleEndpointDistance(lasEle,intRes.P2(),false,dis4);
#else
    dis1 = intRes.P1().DistSquare2D(firEle.StarPt());
    dis2 = intRes.P1().DistSquare2D(lasEle.EndPt());
    dis3 = intRes.P2().DistSquare2D(firEle.StarPt());
    dis4 = intRes.P2().DistSquare2D(lasEle.EndPt());
#endif
    Point P = (dis1 + dis2) <= (dis3 + dis4) ?
                intRes.P1() : intRes.P2();
    loop[0].ChangeEndpoint(P,false);
    loop[loop.Size()-1].ChangeEndpoint(P,true);
    ///DisplayGeom().DisplayLoop(loop,_colors[1],true,false);
}
void LoopEndToEnd::TerminalCurveNotIntersect(DefElem& fcurve,
                                             DefElem& ecurve)
{
    GeomIntersect goInt;ElementPreSolve presol;
    if(fcurve._elemType == LINETYPE){
        if(ecurve._elemType == LINETYPE){
            Point P;            bool isExiInt = false;
            goInt.CalculateLineLineInt(fcurve._segment._segSp,
                                       fcurve._segment._segEp,
                                       ecurve._segment._segSp,
                                       ecurve._segment._segEp,
                                       isExiInt,P);
            if(!isExiInt){
                P = fcurve._segment._segSp.Midpoint(ecurve._segment._segEp);
            }
            fcurve.ChangeEndpoint(P,false);
            ecurve.ChangeEndpoint(P,true);
        }
        else if(ecurve._elemType == ARCTYPE){
            ecurve._arc._arcEp = fcurve._segment._segSp;
            presol.ResetArcCenter(ecurve._arc);
        }
    }
    if(fcurve._elemType == ARCTYPE){
        if(ecurve._elemType == LINETYPE){
            fcurve._arc._arcSp = ecurve._segment._segEp;
            presol.ResetArcCenter(fcurve._arc);
        }
        else if(ecurve._elemType == ARCTYPE){
            Point mp = fcurve._arc._arcSp.Midpoint(ecurve._arc._arcEp);
            fcurve._arc._arcSp = mp;
            ecurve._arc._arcEp = mp;
            presol.ResetArcCenter(fcurve._arc);
            presol.ResetArcCenter(ecurve._arc);
        }
    }
}
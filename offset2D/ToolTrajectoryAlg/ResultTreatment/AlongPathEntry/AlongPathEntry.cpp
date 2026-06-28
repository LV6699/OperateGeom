#include "AlongPathEntry.h"
#include "../../GeometryCalculate/RelativeContain/LimitCoord.h"
#include "../../OperCurve2D/OperCurve2D/OperateCurve.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void AlongPathEntry::LoopOverlapSpecifyDistance(DefLoop oriLoop,
                                                double overDis,
                                                DefLoop& overLoop,
                                                DefLoop& loop)
{
    if(oriLoop.IsEmpty())
        return;
    loop = oriLoop;
    overLoop = oriLoop;
    loop.Clear();
    overLoop.Clear();
    ///当oriLoop 以引用传递时,oriLoop 会被清空.
    Point P;
    int index;
    GeometryLogic().LoopSpecifyLenthPosition(oriLoop,overDis,index,P);
    if(!oriLoop.IsValIndex(index))
        return;
    DefElem prePart;
    ElemCalculate().PointSplitElement(oriLoop.IndexElem(index),
                                      P,true,prePart);
    for (int i = 0; i < index; ++i) {
        overLoop.AddElem(oriLoop.IndexElem(i));
    }
    loop = oriLoop;
    overLoop.AddElem(prePart);
    CutEntrySolve().PointSplitReorederLoop(P,index,loop);
}
void AlongPathEntry::CreateAlongPathCutEntry(const GeomArea &geoArea,
                                             const DefLoop &oriLoop,
                                             MillDirction millDir,
                                             double toolRadius,
                                             CylinderHelix helix,
                                             bool isOverlap,
                                             double overDis,
                                             bool &isHelixPath,
                                             DefLoop& resLoop)
{
    if((std::abs(helix.HelixAngle() - 90) < PreErr_20_ ||
        oriLoop.IsEmpty() || helix.HelixAngle() < PreErr5_4 ||
        helix.HelixHeight() < PreErr5_4))
        return;
    resLoop.Clear();/**
    DefCircle circle;
    LimitCoord2D::LoopMinCircumcircle(oriLoop,circle);
    string str = OperateOftString().LoopElementString(oriLoop,false,0);
    if((circle.R() < toolRadius * 0.7 - PreErr5_4){
        std::cout<<str<<endl;
    }*/
    double xSpan = 0,ySpan = 0,len = 0;
    LimitCoord2D::LoopMinimumBox(oriLoop,xSpan,ySpan);
    len = xSpan > ySpan ? xSpan : ySpan;
    if((len < toolRadius * 2 * 0.7 - PreErr5_4)
            && helix.AlterableHelix()){
        CreateHelix crthel;
        crthel.LoopSpecifyPositionHelix(geoArea,oriLoop,millDir,
                                        toolRadius,helix,resLoop);
        isHelixPath = true;
        return;
    }
    ElemCalculate numcal;
    double iniZCoord = numcal.LoopZCoordinate(oriLoop),
            hErr = 0.006;
    DefLoop workLoop = oriLoop,overLoop,traLoop;
    numcal.ReverseLoopDirection(workLoop);
    if(isOverlap && overDis > PreErr5_2){
        LoopOverlapSpecifyDistance(workLoop,overDis,overLoop,workLoop);
        numcal.SetLoopElementZValue(iniZCoord,overLoop);
    }
    GeometryLogic().LoopToSegmentLoop(workLoop,hErr,false,traLoop);
    CreateAlongPathObliqueCurve(traLoop,helix,iniZCoord,resLoop);
    if(isOverlap && overDis > PreErr5_2){
        ///resLoop.Clear();
        for (int i = 0; i < resLoop.Size(); ++i) {
            overLoop.AddElem(resLoop.IndexElem(i));
        }
        resLoop = overLoop;
    }
    numcal.ReverseLoopDirection(resLoop);
    OperateCurve().DelInvalidGeomCurves(resLoop.m_vElem,PreErr5_4,
                                        PreErr5_4,true,PreErr5_4);
}
void AlongPathEntry::CreateAlongPathObliqueCurve(const DefLoop& loop,
                                                 CylinderHelix helix,
                                                 double iniZCoord,
                                                 DefLoop& resLoop)
{
    if(loop.IsEmpty())
        return;
    double toRadian = helix.HelixAngle() * M_PI / 180;
    double preHeight = iniZCoord,curHeight = iniZCoord,
            Zcoord = iniZCoord + helix.HelixHeight();
    double zIncStep = std::tan(toRadian);
    int i = 0;
    GeomCalculate gocal;
    ElemCalculate numcal;
    while (curHeight < Zcoord) {/**
        if(i == 132){
            int tem = 1;
        }*/
        if(loop.IndexElem(i).Type() != LINETYPE){
            i = numcal.NexId(loop.Size(),i);
            curHeight += zIncStep;
            continue;
        }
        DefSeg segment = loop.IndexElem(i).GetSeg();
        double lenth = gocal.TwoPointDistance2D(segment.GetSp(),
                                                segment.GetEp());
        curHeight = preHeight + (lenth * zIncStep);
        segment._segSp.SetZ(preHeight);
        segment._segEp.SetZ(curHeight);
        resLoop.AddElem(DefElem(LINETYPE,segment));
        preHeight = curHeight;
        i = numcal.NexId(loop.Size(),i);
    }
    if(resLoop.IsEmpty() || resLoop.Last().Type() != LINETYPE)
        return;
    DefElem lasEle = resLoop.Last();
    if(std::abs(Zcoord - lasEle.GetSeg().GetEp().Z())
            <= PreErr5_4){
        int lastId = resLoop.Size();
        numcal.SetElemEndpointZCoord(true,Zcoord,
                                     resLoop.m_vElem[lastId - 1]);
        return;
    }
    double zErr = std::abs(Zcoord - lasEle.GetSeg().GetSp().Z());
    double lasLen = zErr / zIncStep;
    Point endpoint;
    gocal.CloseSegmentEndpoint(lasEle.GetSeg().GetSp(),
                               lasEle.GetSeg().GetEp(),
                               lasLen,true,endpoint);
    endpoint.SetZ(Zcoord);
    lasEle._segment.SetEp(endpoint);
    resLoop.m_vElem.pop_back();
    resLoop.AddElem(lasEle);
}

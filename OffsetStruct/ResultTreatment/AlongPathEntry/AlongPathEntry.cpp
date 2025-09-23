#include "AlongPathEntry.h"
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
    loop.ClearElem();
    overLoop.ClearElem();
    ///当oriLoop以引用传递时,oriLoop会被清空.
    Point P;
    int index;
    GeometryLogic().LoopSpecifyLenthPosition(oriLoop,overDis,index,P);
    if(!oriLoop.IsValIndex(index))
        return;
    DefElem prePart;
    ElemCalculate().PointSplitElement(oriLoop.IndexElem(index),
                                     P,true,prePart);
    for (int i = 0; i < index; ++i) {
        overLoop.PushBackElem(oriLoop.IndexElem(i));
    }
    loop = oriLoop;
    overLoop.PushBackElem(prePart);
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
    resLoop.ClearElem();
    DefCircle circle;
    LimitCoordinate().LoopMinCircumcircle(oriLoop,circle);/**
    string str = OperateOftString().LoopElementString(oriLoop,false,0);
    if((circle.CircleRadius() < toolRadius * 0.7 - PreErr5_4){
        std::cout<<str<<endl;
    }*/
    if((circle.CircleRadius() < toolRadius * 0.7 - PreErr5_4)
            && helix.AlterableHelix()){
        CreateHelix crthel;
        crthel.LoopSpecifyPositionHelix(geoArea,oriLoop,millDir,
                                        toolRadius,helix,resLoop);
        isHelixPath = true;
        return;
    }    ElemCalculate numcal;
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
        ///resLoop.ClearElem();
        for (int i = 0; i < resLoop.ElemNum(); ++i) {
            overLoop.PushBackElem(resLoop.IndexElem(i));
        }
        resLoop = overLoop;
    }
    numcal.ReverseLoopDirection(resLoop);
    ElementPrecision().DeleteInvalidElement(PreErr5_4,
                                            PreErr5_4,resLoop);
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
        if(loop.IndexElem(i).ElemeType() != LINETYPE){
            i = numcal.NextIndex(loop.ElemNum(),i);
            curHeight += zIncStep;
            continue;
        }
        DefSeg segment = loop.IndexElem(i).ElemSeg();
        double lenth = gocal.TwoPointDistance2D(segment.SegStartPt(),
                                                segment.SegEndPt());
        curHeight = preHeight + (lenth * zIncStep);
        segment._segSp.SetZ(preHeight);
        segment._segEp.SetZ(curHeight);
        resLoop.PushBackElem(DefElem(LINETYPE,segment));
        preHeight = curHeight;
        i = numcal.NextIndex(loop.ElemNum(),i);
    }
    if(resLoop.IsEmpty() || resLoop.LastElem().ElemeType() != LINETYPE)
        return;    DefElem lasEle = resLoop.LastElem();
    if(std::abs(Zcoord - lasEle.ElemSeg().SegEndPt().Z())
            <= PreErr5_4){
        int lastId = resLoop.ElemNum();
        numcal.SetElemEndpointZCoord(true,Zcoord,
                                     resLoop.m_vElem[lastId - 1]);
        return;
    }
    double zErr = std::abs(Zcoord - lasEle.ElemSeg().SegStartPt().Z());
    double lasLen = zErr / zIncStep;
    Point endpoint;
    gocal.CloseSegmentEndpoint(lasEle.ElemSeg().SegStartPt(),
                               lasEle.ElemSeg().SegEndPt(),
                               lasLen,true,endpoint);
    endpoint.SetZ(Zcoord);
    lasEle._segment.SetSegEndPt(endpoint);
    resLoop.m_vElem.pop_back();
    resLoop.PushBackElem(lasEle);
}
#include<iostream>
#include "HorArcSteer.h"
#include"../../CommonFile/Header.h"
#if 0
#pragma optimize("", off)
#endif
using namespace std;
void HorArcSteer::HorizontalArcSteer(oft::DefLoop& loop,
                                     oft::MillDirction millDir,
                                     bool isEntry,double R,double agl,
                                     oft::DefArc& arc)
{
#if 0
    oft::Point pt(98.22983491420746,97.882060110569,5);
    string str = OperateOftString().LoopElementString(loop);
    if(FindInformation().IsPointInLoop(loop,PreErr5_4,false,0,pt)){
        std::cout<<str<<endl;
    }
#endif
    if(isEntry && loop.HasReverse()){
        std::cout<<R<<agl<<endl;
    }
    double extLen = 0;
    ElemCalculate elecal;
    if(!isEntry){
        elecal.ReverseLoopDirection(loop);
    }
    HelixCurveSolve hsol;
    bool isClose = loop.IsCloseMode();
    bool isRight = false,cpRight = false;
    hsol.ModelPositionRelativePath(isClose,millDir,isRight);
    cpRight = !isRight;
    if(!isEntry){
        cpRight = isRight;
    }
    if(loop.HasReverse()){cpRight = !cpRight;}
    oft::Point sp,cp,ep;
    oft::DefElem elem = loop.FirstElem();
    OffsetEnum::RotDir loopRot = OffsetEnum::CLOCKWISE;
    if(elem.ElemeType() == OffsetEnum::CIRCLETYPE && !elem._circle.IsCW()){
        loopRot = OffsetEnum::ANTICLOCKWISE;
    }
    ///******************** 计算圆弧相关参数********************
    elecal.ElementEndpoint(elem,false,sp);
    hsol.HelixCircleCenter(elem,sp,R,cpRight,loopRot,cp);
    oft::Point extpt;
    OffsetEnum::RotDir arcRot;
    ///在曲线切入点切线上的点
    hsol.CurvePointTangent(elem,sp,true,R,extpt);
    GeomCalculate().TriangleRotDirection(cp,sp,extpt,arcRot);
    arcRot = arcRot == OffsetEnum::CLOCKWISE ?
                OffsetEnum::ANTICLOCKWISE : OffsetEnum::CLOCKWISE;
    GeomCalculate geocal;
    geocal.PointRotate2D(sp,cp,agl,arcRot == OffsetEnum::CLOCKWISE,ep);
    arc = oft::DefArc(sp,ep,cp,R,arcRot);
    if(isEntry){
        arc.Reverse();
    }
}
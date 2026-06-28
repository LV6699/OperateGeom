#include<iostream>
#include "HorArcSteer.h"
#include"../../CommonFile/Header.h"
#if 0
#pragma optimize("", off)
#endif
using namespace std;
void HorArcSteer::HorizontalArcSteer(OffsetStruct::DefLoop& loop,
                                     OffsetStruct::MillDirction millDir,
                                     bool isEntry,double R,double agl,
                                     OffsetStruct::DefArc& arc)
{
#if 0
    OffsetStruct::Point pt(98.22983491420746,97.882060110569,5);
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
    OffsetStruct::Point sp,cp,ep;
    OffsetStruct::DefElem elem = loop.First();
    bool iscw = true;
    if(elem.Type() == OffsetEnum::CIRCLETYPE && !elem._circle.IsCW()){
        iscw = false;
    }
    ///********************计算圆弧相关参数********************
    elecal.ElementEndpoint(elem,false,sp);
    hsol.HelixCircleCenter(elem,sp,R,cpRight,iscw,cp);
    OffsetStruct::Point extpt;
    bool iscw1;
    ///在曲线切入点切线上的点
    hsol.CurvePointTangent(elem,sp,true,R,extpt);
    GeomCalculate().TriangleRotDirection(cp,sp,extpt,iscw1);
    iscw1 = !iscw1;
    ep = BaseGeom2D::PointRotate2D(sp,cp,agl,iscw1);
    arc = OffsetStruct::DefArc(sp,ep,cp,R,iscw1);
    if(isEntry){arc.Reverse();}
}

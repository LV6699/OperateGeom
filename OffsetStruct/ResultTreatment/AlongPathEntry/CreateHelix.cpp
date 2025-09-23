#include "CreateHelix.h"
#include "HelixCurveSolve.h"
/// 创建一段螺旋线,需要确定螺旋线圆心位置 (在元素当前点处切线方向的左边还是右边 )、
/// 螺旋线类型(左螺旋或右螺旋).
/// 螺旋线圆心确定:如果模型在当前元素所在环左边 ,则圆心位置在右边,否则在左边.
/// 螺旋线类型确定:由于螺旋线的创建过程与加工刀具切入过程相反 ,因此螺旋线类型
/// 基于此确定.即:
/// (1)对于元素是线段,圆心位置在左侧时,是左螺旋,否则是右螺旋.
/// (2)对于元素类型是圆(计算组成圆的圆弧)或圆弧时,圆心位置在当前点切线方向的左
/// 侧,则是左螺旋,否则是右螺旋.
void CreateHelix::LoopSpecifyPositionHelix(const GeomArea &geoArea,
                                           const DefLoop& loop,
                                           MillDirction millDir,
                                           double toolRadius,
                                           CylinderHelix helix,
                                           DefLoop& resLoop)
{
    if(!helix.HasInitHelix() || loop.IsEmpty())
        return;
    Point posPnt;
    DefElem element = loop.IndexElem(0);
    ElemCalculate().ElementEndpoint(element,false,posPnt);
    bool isRightPos = false;
    HelixCurveSolve helsol;
    RotDir loopRot = loop._loopLabel._loRotDir;
    helsol.ModelPositionRelativePath(loop,millDir,isRightPos);
    bool isCpRightPos = !isRightPos;
    Point helCenter;
    helsol.HelixCircleCenter(element,posPnt,helix.HelixRadius(),
                             isCpRightPos,loopRot,helCenter);
    helix.SetHelixCenter(helCenter);/**
    helsol.HelixRadiusInterfere(oriLoop,element,posPnt,
                                toolRadius,isCpRightPos,
                                loopRot,helix);*/
    helix.SetHelixDirection(isCpRightPos);
    CreateHelixCurves(posPnt,helix,resLoop);
    ElemCalculate().ReverseLoopDirection(resLoop);
}
void CreateHelix::CreateHelixCurves(const Point& posPnt,
                                    CylinderHelix helix,
                                    DefLoop& resLoop)
{
    Point helCenter = helix.HelixCenter();
    double helRadius = helix.HelixRadius();
    double helHeight = helix.HelixHeight();
    double helAngle = helix.HelixAngle();
    bool isRightHelix = helix.HelixDirection();
    ///螺旋线一圈上升高度 .
    double cycleHei = 2 * helRadius * M_PI * std::tan(helAngle * M_PI / 180);
    double cycleNum = helHeight / cycleHei;/// 螺旋线总圈数.
    double totalAngle = 360 * cycleNum;/// 螺旋线总角度.
    double perAngHeight = cycleHei / 360;/// 螺旋线一度的高度.
    double startAngle;
    GeomCalculate().SegmentAngle(helCenter,posPnt,startAngle);
    ///totalAngle += startAngle;
    vector<Point>vPoint;
    HelixCurveSolve helsol;    helCenter.SetZ(posPnt.Z());
    helsol.GenerateSpecifyHelixPoint(helCenter,helRadius,startAngle,
                                     perAngHeight,totalAngle,
                                     isRightHelix,vPoint);
    if(vPoint.size() < 2){
        return;
    }
    vPoint[vPoint.size() - 1].SetZ(helCenter.Z() + helHeight);
    ///std::reverse(vPoint.begin(),vPoint.end());
    int nex = 0;
    ElemCalculate numcal;
    for (int i = 0; i < vPoint.size() - 1; ++i) {
        nex = numcal.NextIndex(vPoint.size(),i);
        DefElem ele(LINETYPE,DefSeg(vPoint[i],vPoint[nex]));
        resLoop.m_vElem.push_back(ele);
    }
    ///double zCoordinate = vPoint[vPoint.size() - 1].Z() - vPoint[0].Z();
}
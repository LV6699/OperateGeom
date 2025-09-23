#include "ConnectArcRotDir.h"
shared_ptr<GeometryLogic>arcRot_goLog = make_shared<GeometryLogic>();
shared_ptr<ElemCalculate>arcRot_num = make_shared<ElemCalculate>();
shared_ptr<GeomCalculate>arcRot_go = make_shared<GeomCalculate>();
shared_ptr<GeomIntersect>arcRot_goIn = make_shared<GeomIntersect>();
void ConnectArcRotDir::ArcRotDirBaseOriginLine(Point lSp, Point lEp,
                                               bool isNex,DefArc& arc,
                                               RotDir& rotDir)
{
    ///需要保证lSp-lEp与arc首尾相连,isNex表示arc是否是后一个.
    ///lSp和lEp是偏置之后的,也可以是偏置之前的.
    double lSlope;
    arcRot_go->SegmentAngle(lSp,lEp,lSlope);
    double A = 90;
    Point rotPnt = lEp;
    if(!isNex)
        rotPnt = lSp;
    Point cwPnt,ccwPnt;
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,
                                      A,CLOCKWISE,cwPnt);
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,
                                      A,ANTICLOCKWISE,ccwPnt);
    double cwAgl,ccwAgl;
    arcRot_go->SegmentAngle(arc._arcCp,cwPnt,cwAgl);
    arcRot_go->SegmentAngle(arc._arcCp,ccwPnt,ccwAgl);
    rotDir = CLOCKWISE;
    if(std::abs(cwAgl - lSlope) >= std::abs(ccwAgl - lSlope)){
        rotDir = ANTICLOCKWISE;
    }
    arc._arcRot = rotDir;
}
void ConnectArcRotDir::ArcRotDirBaseOriginArc(const DefArc& baseArc,
                                              bool isNex, DefArc& arc,
                                              RotDir& rotDir)
{
    ///需要保证连接圆弧起点和终点分别与前后连接元素的终点和起点是同一点 ,
    ///isNex表示arc是否是后一个.baseArc是偏置之后的,也可以是偏置之前的.
    double A = 90;
    Point baseRotPnt = baseArc._arcEp;
    if(!isNex){
        baseRotPnt = baseArc._arcSp;
    }
    ///计算baseArc在连接点切线角度
    Point baseRotResPnt;
    arcRot_go->PointAroundFixedRotate(baseRotPnt,baseArc._arcCp,A,
                                      baseArc._arcRot,baseRotResPnt);
    double baseSlope;
    arcRot_go->SegmentAngle(baseArc._arcCp,baseRotResPnt,baseSlope);
    Point rotPnt = arc._arcSp;
    if(!isNex){
        rotPnt = arc._arcEp;
    }
    Point cwPnt,ccwPnt;
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,A,
                                      CLOCKWISE,cwPnt);    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,A,
                                      ANTICLOCKWISE,ccwPnt);
    double cwAgl,ccwAgl;
    arcRot_go->SegmentAngle(arc._arcCp,cwPnt,cwAgl);
    arcRot_go->SegmentAngle(arc._arcCp,ccwPnt,ccwAgl);
    rotDir = CLOCKWISE;
    if(std::abs(cwAgl - baseSlope) >= std::abs(ccwAgl - baseSlope)){
        rotDir = ANTICLOCKWISE;
    }
    arc._arcRot = rotDir;
}
void ConnectArcRotDir::ArcDirectionBaseOriginElem(const DefElem& ele,bool isNex,
                                                  DefArc& arc,RotDir& rotDir)
{
    switch (ele.ElemeType()) {
    case LINETYPE:
        ArcRotDirBaseOriginLine(ele.ElemSeg().SegStartPt(),
                                ele.ElemSeg().SegEndPt(),
                                isNex,arc,rotDir);
        break;
    case ARCTYPE:
        ArcRotDirBaseOriginArc(ele.ElemArc(),isNex,arc,rotDir);
        break;
    default:
        break;
    }
}
void ConnectArcRotDir::ArcRotBasePrevoiusElement(const DefElem& ele,
                                                 ProfType loopType,
                                                 RotDir& rotDir)
{
    switch (ele.ElemeType()) {
    case LINETYPE:
        ArcRotBasePrevoiusSeg(ele.ElemSeg().SegStartPt(),
                              ele.ElemSeg().SegEndPt(),
                              loopType,rotDir);
        break;
    case ARCTYPE:
        ArcRotBasePrevoiusArc(ele.ElemArc(),loopType,rotDir);
        break;
    default:
        break;
    }
}
void ConnectArcRotDir::ArcRotBasePrevoiusSeg(const Point& lSp,
                                             const Point& lEp,
                                             ProfType loopType,
                                             RotDir& rotDir)
{
    GeomCalculate gocal;
    EquidistantCurve equ;
    Point lSp_,lEp_,cwPt,ccwPt;
    double A = 90,slope,cwAgl,ccwAgl;
    equ.EquidistantSegment(lSp,lEp,2,loopType,lSp_,lEp_);
    gocal.PointAroundFixedRotate(lSp_,lSp,A,CLOCKWISE,cwPt);
    gocal.PointAroundFixedRotate(lSp_,lSp,A,ANTICLOCKWISE,ccwPt);
    gocal.SegmentAngle(lSp,lEp,slope);
    gocal.SegmentAngle(lSp,cwPt,cwAgl);    gocal.SegmentAngle(lSp,ccwPt,ccwAgl);
    rotDir = CLOCKWISE;
    if(std::abs(cwAgl - slope) >= std::abs(ccwAgl - slope))
        rotDir = ANTICLOCKWISE;
}
void ConnectArcRotDir::ArcRotBasePrevoiusArc(const DefArc& arc,
                                             ProfType loopType,
                                             RotDir& rotDir)
{
    double R = arc.ArcRadius();
    if(R < PreErr5_3){
        R = GeomCalculate().CalculateArcRadius(arc);
    }
    double val = R / 2;
    int sym = loopType == OUTSIDEPROFILE ? 1 : -1;
    double R_ = arc.ArcRot() == CLOCKWISE ?
                R - val * sym : R + val * sym;
    if(R_ > R)
        rotDir = arc.ArcRot();
    else{
        if(arc.ArcRot() == CLOCKWISE)
            rotDir = ANTICLOCKWISE;
        else
            rotDir = CLOCKWISE;
    }
}
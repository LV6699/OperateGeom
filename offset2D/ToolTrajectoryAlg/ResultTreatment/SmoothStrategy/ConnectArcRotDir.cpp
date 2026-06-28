#include "ConnectArcRotDir.h"
shared_ptr<GeometryLogic>arcRot_goLog = make_shared<GeometryLogic>();
shared_ptr<ElemCalculate>arcRot_num = make_shared<ElemCalculate>();
shared_ptr<GeomCalculate>arcRot_go = make_shared<GeomCalculate>();
shared_ptr<GeomIntersect>arcRot_goIn = make_shared<GeomIntersect>();
void ConnectArcRotDir::ArcRotDirBaseOriginLine(Point lSp, Point lEp,
                                               bool isNex,DefArc& arc,
                                               bool& iscw)
{
    ///需要保证lSp-lEp 与arc 首尾相连,isNex 表示arc 是否是后一个.
    ///lSp 和lEp 是偏置之后的,也可以是偏置之前的.
    double lSlope;
    arcRot_go->SegmentAngle(lSp,lEp,lSlope);
    double A = 90;
    Point rotPnt = lEp;
    if(!isNex)
        rotPnt = lSp;
    Point cwPnt,ccwPnt;
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,
                                      A,true,cwPnt);
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,
                                      A,false,ccwPnt);
    double cwAgl,ccwAgl;
    arcRot_go->SegmentAngle(arc._arcCp,cwPnt,cwAgl);
    arcRot_go->SegmentAngle(arc._arcCp,ccwPnt,ccwAgl);
    iscw = true;
    if(std::abs(cwAgl - lSlope) >= std::abs(ccwAgl - lSlope)){
        iscw = false;
    }
    arc._isCW = iscw;
}
void ConnectArcRotDir::ArcRotDirBaseOriginArc(const DefArc& baseArc,
                                              bool isNex, DefArc& arc,
                                              bool& iscw)
{
    ///需要保证连接圆弧起点和终点分别与前后连接元素的终点和起点是同一点,
    ///isNex 表示arc 是否是后一个.baseArc 是偏置之后的,也可以是偏置之前的.
    double A = 90;
    Point baseRotPnt = baseArc._arcEp;
    if(!isNex){
        baseRotPnt = baseArc._arcSp;
    }
    ///计算baseArc 在连接点切线角度
    Point baseRotResPnt;
    arcRot_go->PointAroundFixedRotate(baseRotPnt,baseArc._arcCp,A,
                                      baseArc._isCW,baseRotResPnt);
    double baseSlope;
    arcRot_go->SegmentAngle(baseArc._arcCp,baseRotResPnt,baseSlope);
    Point rotPnt = arc._arcSp;
    if(!isNex){
        rotPnt = arc._arcEp;
    }
    Point cwPnt,ccwPnt;
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,A,
                                      true,cwPnt);
    arcRot_go->PointAroundFixedRotate(rotPnt,arc._arcCp,A,
                                      false,ccwPnt);
    double cwAgl,ccwAgl;
    arcRot_go->SegmentAngle(arc._arcCp,cwPnt,cwAgl);
    arcRot_go->SegmentAngle(arc._arcCp,ccwPnt,ccwAgl);
    iscw = true;
    if(std::abs(cwAgl - baseSlope) >= std::abs(ccwAgl - baseSlope)){
        iscw = false;
    }
    arc._isCW = iscw;
}
void ConnectArcRotDir::ArcDirectionBaseOriginElem(const DefElem& ele,bool isNex,
                                                  DefArc& arc,bool& iscw)
{
    switch (ele.Type()) {
    case LINETYPE:
        ArcRotDirBaseOriginLine(ele.GetSeg().GetSp(),
                                ele.GetSeg().GetEp(),
                                isNex,arc,iscw);
        break;
    case ARCTYPE:
        ArcRotDirBaseOriginArc(ele.GetArc(),isNex,arc,iscw);
        break;
    default:
        break;
    }
}
void ConnectArcRotDir::ArcRotBasePrevoiusElement(const DefElem& ele,
                                                 bool leftward,
                                                 bool& iscw)
{
    switch (ele.Type()) {
    case LINETYPE:
        ArcRotBasePrevoiusSeg(ele.GetSeg().GetSp(),
                              ele.GetSeg().GetEp(),
                              leftward,iscw);
        break;
    case ARCTYPE:
        ArcRotBasePrevoiusArc(ele.GetArc(),leftward,iscw);
        break;
    default:
        break;
    }
}
void ConnectArcRotDir::ArcRotBasePrevoiusSeg(const Point& lSp,
                                             const Point& lEp,
                                             bool leftward,
                                             bool& iscw)
{
    GeomCalculate gocal;
    EquidistantCurve equ;
    Point lSp_,lEp_,cwPt,ccwPt;
    double A = 90,slope,cwAgl,ccwAgl;
    equ.EquidistantSegment(lSp,lEp,2,leftward,lSp_,lEp_);
    gocal.PointAroundFixedRotate(lSp_,lSp,A,true,cwPt);
    gocal.PointAroundFixedRotate(lSp_,lSp,A,false,ccwPt);
    gocal.SegmentAngle(lSp,lEp,slope);
    gocal.SegmentAngle(lSp,cwPt,cwAgl);
    gocal.SegmentAngle(lSp,ccwPt,ccwAgl);
    iscw = true;
    if(std::abs(cwAgl - slope) >= std::abs(ccwAgl - slope))
        iscw = false;
}
void ConnectArcRotDir::ArcRotBasePrevoiusArc(const DefArc& arc,
                                             bool leftward,
                                             bool& iscw)
{
    double R = arc.R();
    if(R < PreErr5_3){R = GeomCalculate().CalculateArcRadius(arc);}
    double val = R / 2;
    //int sym = loopType == OUTSIDEPROFILE ? 1 : -1;
    int sym = leftward == false ? 1 : -1;
    double R_ = arc.IsCW() ? R - val * sym : R + val * sym;
    if(R_ > R)
        iscw = arc.IsCW();
    else{
        if(arc.IsCW() == true){iscw = false;}
        else{iscw = true;}
    }
}

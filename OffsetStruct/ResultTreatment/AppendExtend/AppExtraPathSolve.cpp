#include "AppExtraPathSolve.h"
AppExtraPathSolve::AppExtraPathSolve()
{
}
void AppExtraPathSolve::AdjoinElementAppendPath(DefLoop &loop,int index,
                                                double radius,double offsetVal,
                                                vector<DefElem>&vElem)
{
    vElem.clear();
    if(index == loop.ElemNum() - 1)
        return;
    double aLim = 150,angle;
    int nex = loop.NextIndex(index);
    RotDir rotDir;
    DefElem ele1 = loop.IndexElem(index),ele2 = loop.IndexElem(nex);
    OperateElem().AdjoinElemRotDirection(ele1,ele2,rotDir);
    if(rotDir != CLOCKWISE)
        return;
    CalculateAngle().AdjoinElementAngle(ele1,ele2,angle);
    if(angle > aLim)
        return;
    switch (ele1.ElemeType()) {
    case LINETYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            CreateSegSegExtraPath(radius,offsetVal,ele1.ElemSeg(),
                                  ele2.ElemSeg(),ele1._elemLabel.
                                  _baseParam,angle,vElem);
            break;
        case ARCTYPE:
            CreateSegArcExtraPath(radius,offsetVal,ele1.ElemSeg(),
                                  ele2.ElemArc(),true,ele1._elemLabel.
                                  _baseParam,angle,vElem);
            break;
        }
        break;
    case ARCTYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            CreateSegArcExtraPath(radius,offsetVal,ele2.ElemSeg(),
                                  ele1.ElemArc(),false,ele1._elemLabel.
                                  _baseParam,angle,vElem);
            break;
        case ARCTYPE:
            CreateArcArcExtraPath(radius,offsetVal,ele1.ElemArc(),
                                  ele2.ElemArc(),ele1._elemLabel.
                                  _baseParam,angle,vElem);
            break;
        }
        break;
    default:
        break;
    }
}
void AppExtraPathSolve::CreateSegSegExtraPath(double radius,double offsetVal,
                                              const DefSeg& seg1,
                                              const DefSeg& seg2,
                                              OftBasePara basePara,                                              const double angle,
                                              vector<DefElem>&vElem)
{
    double R = basePara.ToolRadius(),
            oftVal = basePara.OffsetValue();
    double diff = oftVal - R,trans = M_PI / 180;
    double rad = angle * trans / 2;
    if(!basePara.IsInitail() || R == 0 || offsetVal == 0){
        diff = std::abs(offsetVal - radius) * PreErr_8_;
    }
    GeomCalculate gocal;
    Point cntPt = seg1.SegEndPt();
    double angle1 = 0,angle2 = 0;
    gocal.SegmentAngle(seg1.SegStartPt(),cntPt,angle1);
    gocal.SegmentAngle(seg2.SegEndPt(),cntPt,angle2);
    double len =  diff / (std::sin(angle * rad));
    RotDir rotDir = CLOCKWISE;
    gocal.TriangleRotDirection(seg1.SegStartPt(),cntPt,
                               seg2.SegEndPt(),rotDir);
    rotDir = rotDir == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    double centerAgl = gocal.AngleMidAngle(angle1,angle2,rotDir);
    double edge = len * std::cos(rad);
    double incx = len * std::cos(centerAgl * trans),
            incy = len * std::sin(centerAgl * trans),
            incx1 = edge * std::cos(angle1 * trans),
            incy1 = edge * std::sin(angle1 * trans),
            incx2 = edge * std::cos(angle2 * trans),
            incy2 = edge * std::sin(angle2 * trans);
    Point cp(cntPt.X() + incx,cntPt.Y() + incy),
            p1(cntPt.X() + incx1,cntPt.Y() + incy1),
            p2(cntPt.X() + incx2,cntPt.Y() + incy2);
    DefElem ele1(LINETYPE,DefSeg(cntPt,p1)),
            ele2(ARCTYPE,DefArc(p1,p2,cp,diff,rotDir)),
            ele3(LINETYPE,DefSeg(p2,cntPt));
    vElem.push_back(ele1);
    vElem.push_back(ele2);
    vElem.push_back(ele3);
}
void AppExtraPathSolve::CreateSegArcExtraPath(double radius,
                                              double offsetVal,
                                              const DefSeg& seg,
                                              const DefArc& arc,
                                              bool isLineFont,
                                              OftBasePara basePara,
                                              const double angle,
                                              vector<DefElem>&vElem)
{
    double R = basePara.ToolRadius(),
            oftVal = basePara.OffsetValue();
    double diff = oftVal - R;
    if(!basePara.IsInitail() || R == 0 || offsetVal == 0){
        diff = std::abs(offsetVal - radius) * PreErr_8_;
    }
    diff = std::abs(offsetVal - radius) * PreErr_7_;
    RotDir arcRot = arc.ArcRot();
    Point cntPt = seg.SegEndPt(),rotPt;
    if(isLineFont){
        arcRot = arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;    }
    else{
        cntPt = seg.SegStartPt();
    }
    GeomCalculate gocal;
    gocal.PointAroundFixedRotate(arc.ArcCenterPt(),cntPt,
                                 90,arcRot,rotPt);
    double segAg = 0,arcAg = 0;
    if(isLineFont){
        gocal.SegmentAngle(seg.SegStartPt(),cntPt,segAg);
        gocal.SegmentAngle(rotPt,cntPt,arcAg);
    }
    else{
        gocal.SegmentAngle(seg.SegEndPt(),seg.SegStartPt(),segAg);
        gocal.SegmentAngle(rotPt,cntPt,arcAg);
    }
    double len =  diff / (std::sin(angle * M_PI / 360));
    RotDir rotDir = CLOCKWISE;
    if(isLineFont){
        gocal.TriangleRotDirection(seg.SegStartPt(),cntPt,
                                   rotPt,rotDir);
    }
    else{
        gocal.TriangleRotDirection(rotPt,cntPt,
                                   seg.SegEndPt(),rotDir);
    }
    rotDir = rotDir == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    double centerAgl;
    if(isLineFont){
        centerAgl = gocal.AngleMidAngle(segAg,arcAg,rotDir);
    }
    else{
        centerAgl = gocal.AngleMidAngle(arcAg,segAg,rotDir);
    }
    double angle1,angle2;
    if(isLineFont){
        angle1 = segAg;
        angle2 = arcAg;
    }
    else{
        angle1 = arcAg;
        angle2 = segAg;
    }
    double edge = len * std::cos(angle * M_PI / 360);
    double incx = len * std::cos(centerAgl * M_PI / 180),
            incy = len * std::sin(centerAgl * M_PI / 180),
            incx1 = edge * std::cos(angle1 * M_PI / 180),
            incy1 = edge * std::sin(angle1 * M_PI / 180),
            incx2 = edge * std::cos(angle2 * M_PI / 180),
            incy2 = edge * std::sin(angle2 * M_PI / 180);
    Point cp(cntPt.X() + incx,cntPt.Y() + incy),
            p1(cntPt.X() + incx1,cntPt.Y() + incy1),
            p2(cntPt.X() + incx2,cntPt.Y() + incy2);
    DefElem ele1(LINETYPE,DefSeg(cntPt,p1)),
            ele2(ARCTYPE,DefArc(p1,p2,cp,diff,rotDir)),
            ele3(LINETYPE,DefSeg(p2,cntPt));
    vElem.push_back(ele1);
    vElem.push_back(ele2);
    vElem.push_back(ele3);
}void AppExtraPathSolve::CreateArcArcExtraPath(double radius,double offsetVal,
                                              const DefArc& arc1,
                                              const DefArc& arc2,
                                              OftBasePara basePara,
                                              const double angle,
                                              vector<DefElem>&vElem)
{
    double R = basePara.ToolRadius(),
            oftVal = basePara.OffsetValue();
    double dif = oftVal - R;
    if(!basePara.IsInitail() || R == 0 || offsetVal == 0){
        dif = std::abs(offsetVal - radius) * PreErr_8_;
    }
    dif = std::abs(offsetVal - radius) * PreErr_5_;
    if(dif < radius / 2)
        return;
    GeomCalculate gocal;
    Point cntPt = arc1.ArcEndPt(),rotPt1,rotPt2;
    RotDir rot1 = arc1.ArcRot(),
            rot2 = arc2.ArcRot() == CLOCKWISE ?
                ANTICLOCKWISE : CLOCKWISE;
    gocal.PointAroundFixedRotate(arc1.ArcCenterPt(),cntPt,
                                 90,rot1,rotPt1);
    gocal.PointAroundFixedRotate(arc2.ArcCenterPt(),cntPt,
                                 90,rot2,rotPt2);
    double angle1 = 0,angle2 = 0;
    gocal.SegmentAngle(rotPt1,cntPt,angle1);
    gocal.SegmentAngle(rotPt2,cntPt,angle2);
    double len =  dif / (std::sin(angle * M_PI / 360));
    RotDir rotDir = CLOCKWISE;
    gocal.TriangleRotDirection(cntPt,rotPt1,rotPt2,rotDir);
    ///rotDir = rotDir == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    double centerAgl = gocal.AngleMidAngle(angle1,angle2,rotDir);
    double edge = len * std::cos(centerAgl * M_PI / 180);
    double incx = len * std::cos(centerAgl * M_PI / 180),
            incy = len * std::sin(centerAgl * M_PI / 180),
            incx1 = edge * std::cos(angle1 * M_PI / 180),
            incy1 = edge * std::sin(angle1 * M_PI / 180),
            incx2 = edge * std::cos(angle2 * M_PI / 180),
            incy2 = edge * std::sin(angle2 * M_PI / 180);
    Point cp(cntPt.X() + incx,cntPt.Y() + incy),
            p1(cntPt.X() + incx1,cntPt.Y() + incy1),
            p2(cntPt.X() + incx2,cntPt.Y() + incy2);
    DefElem ele1(LINETYPE,DefSeg(cntPt,p1)),
            ele2(ARCTYPE,DefArc(p1,p2,cp,dif,rotDir)),
            ele3(LINETYPE,DefSeg(p2,cntPt));
    vElem.push_back(ele1);
    vElem.push_back(ele2);
    vElem.push_back(ele3);
}
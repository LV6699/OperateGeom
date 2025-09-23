#include "OperateElem.h"
OperateElem::OperateElem()
{
}
void OperateElem::AdjoinElemRotDirection(const DefElem &ele1,
                                         const DefElem &ele2,
                                         RotDir &rotDir)
{
    switch (ele1.ElemeType()) {
    case LINETYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            AdjoinSegSegRotDirection(ele1.ElemSeg(),
                                  ele2.ElemSeg(),rotDir);
            break;
        case ARCTYPE:
            AdjoinSegArcRotDirection(ele1.ElemSeg(),ele2.ElemArc(),
                                     true,rotDir);
            break;
        }
        break;
    case ARCTYPE:
        switch (ele2.ElemeType()) {
        case LINETYPE:
            AdjoinSegArcRotDirection(ele2.ElemSeg(),ele1.ElemArc(),
                                     false,rotDir);
            break;
        case ARCTYPE:
            AdjoinArcArcRotDirection(ele1.ElemArc(),ele2.ElemArc(),rotDir);
            break;
        }
        break;
    default:
        break;
    }
}
void OperateElem::AdjoinSegSegRotDirection(const DefSeg &seg1,
                                           const DefSeg &seg2,
                                           RotDir &rotDir)
{
    GeomCalculate gocal;
    gocal.TriangleRotDirection(seg1.SegStartPt(),
                               seg1.SegEndPt(),
                               seg2.SegEndPt(),rotDir);
}
void OperateElem::AdjoinSegArcRotDirection(const DefSeg &seg,
                                           const DefArc &arc,
                                           bool isLineFont,
                                           RotDir &rotDir)
{
    GeomCalculate gocal;
    Point basPt = seg.SegStartPt(),rotPt;
    RotDir rot = arc.ArcRot();
    if(isLineFont){
        basPt = seg.SegEndPt();
        rot = rot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;    }
    gocal.PointAroundFixedRotate(arc.ArcCenterPt(),basPt,
                                 90,rot,rotPt);
    if(isLineFont){
        gocal.TriangleRotDirection(seg.SegStartPt(),
                                   seg.SegEndPt(),
                                   rotPt,rotDir);
    }
    else{
        gocal.TriangleRotDirection(rotPt,seg.SegEndPt(),
                                   seg.SegStartPt(),rotDir);
    }
}
void OperateElem::AdjoinArcArcRotDirection(const DefArc &arc1,
                                           const DefArc &arc2,
                                           RotDir &rotDir)
{
    GeomCalculate gocal;
    Point rotPt1,rotPt2;
    RotDir rotDir2 = arc2._arcRot == CLOCKWISE ? ANTICLOCKWISE : CLOCKWISE;
    gocal.PointAroundFixedRotate(arc1.ArcCenterPt(),arc1.ArcEndPt(),
                                 90,arc1.ArcRot(),rotPt1);
    gocal.PointAroundFixedRotate(arc2.ArcCenterPt(),arc1.ArcEndPt(),
                                 90,rotDir2,rotPt2);
    gocal.TriangleRotDirection(rotPt1,arc1.ArcEndPt(),rotPt2,rotDir);
}
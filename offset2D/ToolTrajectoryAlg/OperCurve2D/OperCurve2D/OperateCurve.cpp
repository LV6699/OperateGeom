#include "OperateCurve.h"
#include "../../GeometryCalculate/GeomCalculate/BaseGeom2D.h"
#include "../../GeometryCalculate/GeomQuality/CurveQual2D.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
bool OperateCurve::AdjoinCurveRotDire(const ofts::DefElem& c1,
                                      const ofts::DefElem& c2)
{
    switch (c1.Type()) {
    case Ofte::LINETYPE:
        switch (c2.Type()) {
        case Ofte::LINETYPE:
            return AdjoinSegRotDire(c1.GetSeg(),c2.GetSeg());
            break;
        case Ofte::ARCTYPE:
            return AdjoinSegArcRotDire(c1.GetSeg(),c2.GetArc(),true);
            break;
        default:
            break;
        }
        break;
    case Ofte::ARCTYPE:
        switch (c2.Type()) {
        case Ofte::LINETYPE:
            return AdjoinSegArcRotDire(c2.GetSeg(),c1.GetArc(),false);
            break;
        case Ofte::ARCTYPE:
            return AdjoinArcRotDire(c1.GetArc(),c2.GetArc());
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return false;
}
bool OperateCurve::AdjoinSegRotDire(const ofts::DefSeg &seg1,
                                    const ofts::DefSeg &seg2)
{
    return BaseGeom2D::IsTriaRotDireCW(seg1.GetSp(),seg1.GetEp(),seg2.GetEp());
}
bool OperateCurve::AdjoinSegArcRotDire(const ofts::DefSeg &seg,
                                       const ofts::DefArc &arc,
                                       bool isLineFont)
{
    auto basPt = seg.GetSp();
    bool isCW = arc.IsCW();
    if(isLineFont){
        basPt = seg.GetEp();isCW = !isCW;
    }
    auto rotPt = BaseGeom2D::PointRotate2D(arc.GetCp(),basPt,90,isCW);
    if(isLineFont){
        return BaseGeom2D::IsTriaRotDireCW(seg.GetSp(),seg.GetEp(),rotPt);
    }
    return BaseGeom2D::IsTriaRotDireCW(rotPt,seg.GetEp(),seg.GetSp());
}
bool OperateCurve::AdjoinArcRotDire(const ofts::DefArc &arc1,
                                    const ofts::DefArc &arc2)
{
    auto rotPt1 = BaseGeom2D::PointRotate2D(arc1.GetCp(),arc1.GetEp(),90,arc1.IsCW());
    auto rotPt2 = BaseGeom2D::PointRotate2D(arc2.GetCp(),arc1.GetEp(),90,!arc2.IsCW());
    bool iscw = BaseGeom2D::IsTriaRotDireCW(rotPt1,arc1.GetEp(),rotPt2);
    return iscw;
}
void OperateCurve::DelInvalidGeomCurves(std::vector<ofts::DefElem>& cs,
                                        double segErr,double aEnd,
                                        bool isR,double minR,
                                        bool isBh,double bH,
                                        bool aIsDecNum,int aNum)
{
    //std::vector<size_t>ids;
    for(auto& d : cs){d.SetValid(true);}
    for (size_t i = 0; i < cs.size(); i++){
        auto& c = cs[i];
        if(!CurveQual2D::IsValidGeomCurve(c,segErr,aEnd,isR,minR,isBh,bH,aIsDecNum,aNum)){
            c.SetValid(false);
            //ids.push_back(i);
        }
    }
    cs.erase(std::remove_if(cs.begin(),cs.end(),[](const ofts::DefElem& c){
        return !c.IsValid();}),cs.end());
}
// ofts::DefSeg ArcPositionTangent(const ofts::DefArc& arc,const ofts::Point& p)
// {
// }

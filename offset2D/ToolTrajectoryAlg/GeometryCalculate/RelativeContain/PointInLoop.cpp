#include "PointInLoop.h"
#include "../../OperCurve2D/OperateOftString/OperateOftString.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
void PointInLoop::GetPointInsideLoop(const ofts::DefLoop& loop,
                                     const ofts::Point& P,
                                     double bPre,bool& isOnBord,
                                     bool& isInPoly,bool positive)
{
    isOnBord = false;
    isInPoly = false;
    auto limc = loop.GetLimCoord();
    if(!limc.HasInit()){limc = LimitCoord2D::LoopLimitCoord(loop);}
    if(!limc.IsInclude(P.X(),P.Y(),bPre)){return;}
    for(size_t i = 0;i < loop.Size();++i){
        isOnBord = PtOnCurve2D::IsPointOnCurve(loop[i],P,bPre,bPre,true,true);
        if(isOnBord){
            _isOnBorder = true;return;
        }
    }
    InitParam(bPre);
    auto& iData = _iData;
    auto& ipa = _iparam;
    ofts::Point raypt(positive ? (limc.MaxX() + 1) : (limc.MinX() - 1),P.Y());
    iData.horIds.reserve(loop.Size() / 4);
    iData.iIds.reserve(loop.Size() / 4);
#if 0
    Point op(113.10124885875692,72.389639909580097);
    if(!positive && P.IsSamePoint2D(op,PreErr_6)){
        DisplayGeom().DisplayLoop(loop,_colors.back(),1,true,true);
        std::vector<ofts::Point>ps;
        for(auto& c : loop){ps.push_back(c.GetSp());}
        for(auto& p : ps){DisplayGeom().DisplayPoint(p,_colors[1]);}
        DisplayGeom().DisplayPoint(P,_colors[4]);
        DisplayGeom().DisplaySegment(P,raypt,_colors[1],1);
        string str = OperateOftString().LoopElementString(loop);
        std::cout<<str<<std::endl;
    }
#endif
#if _paralell_inloop
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,loop.Size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
#else
    for (size_t i = 0; i < loop.Size(); ++i){/**
        if(i == 7){
            int tem = 0;
        }*/
#endif
        ofts::GeomIntRes iRes;
        const auto& curve = loop[i];
        auto limc1 = curve.GetLimCoord();
        if(!limc1.HasInit()){limc1 = LimitCoord2D::CurveLimitCoord(curve);}
  
        switch (curve._type) {
        case Ofte::LINETYPE:{
            const auto& sp = curve.GetSeg().GetSp();
            const auto& ep = curve.GetSeg().GetEp();
            if(CurveQual2D::IsHorizontalLine(sp,ep,iData.horPre)){
                iData.horIds.emplace_back(i);continue;
            }
            if(!PtInLoopAux::PointInRange(limc1,P,bPre,positive)){
                NotIntCase(loop,i,isInPoly);
                iData.hasNotHor = true;continue;
            }
            CurveInt2D::GetSegmentIntersect(P,raypt,sp,ep,ipa,iRes);
            if(!iRes.IsInt()){
                NotIntCase(loop,i,isInPoly);
                iData.hasNotHor = true;continue;
            }
            PtInLoopAux::IntersectSegCase(loop,i,sp,ep,iRes.P1(),iData,isInPoly);
            iData.hasNotHor = true;
            iData.AddIntIds(i,iRes);
            break;
        }
        case Ofte::ARCTYPE:{
            if(!PtInLoopAux::PointInRange(limc1,P,bPre,positive)){
                NotIntCase(loop,i,isInPoly);continue;
            }
            const auto& arc = curve._arc;
            CurveInt2D::GetSegmentArcInt(P,raypt,arc,ipa,iRes);
            if(!iRes.IsInt()){
                NotIntCase(loop,i,isInPoly);continue;
            }
            PtInLoopAux::IntersectArcCase(loop,P,raypt,i,arc,iRes,iData,isInPoly);
            iData.hasNotHor = true;
            iData.AddIntIds(i,iRes);
            break;
        }
        case Ofte::CIRCLETYPE:{
            double d = P.DistSquare2D(curve._circle.GetCp());
            if(d < curve._circle.R() * curve._circle.R() + bPre){
                isInPoly = true;_isInLoop = true;
                iData.AddIntIds(i,ofts::GeomIntRes(curve.GetCircle().RightPt()));
                return;
            }
            break;
        }
        default: break;
        }
    }
#if _paralell_inloop
});
#endif
_isInLoop = isInPoly;
}
/**
isMax:交点具备最大坐标,isMin:交点不具备最大坐标,需要(不需要)比较下一曲线起点是否有最大坐标:
nexSp(noNexSp).
1)当isOnBorder 较小(PreErr5_10),而判断交点是线端点精度较底(PreErr_6)可能出现的情况,当
射线与元素i 相交点ip,i+1 是水平线段,i+2 是最后元素且在点右边(不相交),且起点isMin,而ip 判断出是
元素i 结束点isMin,理论上点应该在环内,与i 相交后nexSp,i+1 会跳过,而i+1 起点isMin,因此判断出不在
环内(错误结果).处理方法是在环边界上不要太高,比如与判断交点是端点的精度一致,会判断出在边界上.
如果环从0 至第i 个元素均是水平线段,第i+1 个不是,而与第i+1 个相交起点,变量_isComNexSp 是由上一个元素
相交情况确定,因此会判断出与i+1 相交非端点,就可能会判断失误.处理方法是标记是否已经出现非水平线段曲线,
如果还没有出现,则变量_isComNexSp 就要单独计算交点水否是i+1 的起点.
*/
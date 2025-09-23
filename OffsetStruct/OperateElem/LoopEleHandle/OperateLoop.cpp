#include "OperateLoop.h"
OperateLoop::OperateLoop()
{
}
void OperateLoop::SetLoopSourceData(const DefLoop& sourceLoop,
                                    OffsetParam offsetPara,
                                    DefLoop& loop)
{
    /*std::shared_ptr<DefLoop>l = std::make_shared<DefLoop>();
    GenerateParam  param(offsetPara._offsetUnit.ToolRadius(),
                         offsetPara._offsetUnit.OffsetValue(),l);
    loop._loopLabel.SetGenerateParam(param);*/
}
void OperateLoop::SetElementParam(const DefElem &oriEle,DefElem &ele)
{
    if(oriEle.ElemeType() != ele.ElemeType())
        return;
    DefElem ele_ = ele;
    ele = oriEle;
    switch (ele_.ElemeType()) {
    case LINETYPE:
        ele.SetElemSeg(ele_.ElemSeg());
        break;
    case ARCTYPE:
        ele.SetElemArc(ele_.ElemArc());
        break;
    case CIRCLETYPE:
        ele.SetElemCircle(ele_.ElemCircle());
        break;
    default:
        break;
    }
}
void OperateLoop::SetLoopElemBaseParam(double r,double offsetVal,
                                       DefLoop& loop)
{
    GenerateParam gene;
    OftBasePara baseParam(r,offsetVal);
    gene.SetBaseParam(baseParam);
    loop._loopLabel.SetGenerateParam(gene);
    for(auto &d : loop.m_vElem){
        d._elemLabel._baseParam = baseParam;
    }
}
void OperateLoop::SetArrayLoopBaseParam(double r,double offsetVal,
                                        vector<DefLoop>& vLoop)
{
    for(auto &d : vLoop){
        SetLoopElemBaseParam(r,offsetVal,d);
    }
}
void OperateLoop::LoopCurveEndToEnd(DefLoop &loop)
{
    if(GeometryLogic().IsCircleStructureLoop(
                loop,PreErr5_10,PreErr5_10) || loop.IsEmpty())
        return;
    int len = loop.Size() - 1;    ElementPreSolve presol;
    for(size_t i = 0;i < loop.Size();++i){/**
        if(i == 55){
            int tem = 1;
        }*/
        if(loop.IsOpenLoop() && i == 0)
            continue;
        auto& curve = loop[i];
        const auto& pcurve = loop.PreviousElem(i);
        const auto& pep = pcurve.EndPt();
        switch (curve.ElemeType()) {
        case OffsetEnum::LINETYPE:
            curve._segment._segSp = pep;
            presol.SetLineHorVerQuality(curve._segment._segSp,
                                        curve._segment._segEp,
                                        true,Base_LineHor_Err);
            break;
        case OffsetEnum::ARCTYPE:
            curve._arc._arcSp = pep;
            presol.SetLineHorVerQuality(curve._segment._segSp,
                                        curve._segment._segEp,
                                        true,Base_LineHor_Err);
            presol.ResetArcEndpoint(i == len,loop.m_vElem[i]._arc);
            break;
        default:
            break;
        }
    }
}
void OperateLoop::AdjacentParallSegment(DefLoop& loop, double aglErr)
{
    if(loop.Size() < 2)
        return;
    GeomCalculate gocal;
    int i = 0,n = 0;
    while (i < int(loop.Size())) {
        n = loop.NextIndex(i);
        if(loop[i]._elemType != LINETYPE || loop[n]._elemType != LINETYPE){
            i ++;
            continue;
        }
        const auto& sp1 = loop[i]._segment._segSp;
        const auto& ep1 = loop[i]._segment._segEp;
        const auto& sp2 = loop[n]._segment._segSp;
        const auto& ep2 = loop[n]._segment._segEp;
        double slop1 = gocal.SegmentSlope(sp1,ep1);
        double slop2 = gocal.SegmentSlope(sp2,ep2);
        if(std::abs(slop1 - slop2) >= aglErr){
            i ++;
            continue;
        }
        loop[i]._segment._segEp = loop[n]._segment._segEp;
        loop.m_vElem.erase(loop.m_vElem.begin() + n);
    }
}
void OperateLoop::AdjacentParallArc(DefLoop& loop, double cpErr, double rErr)
{
    if(GeometryLogic().IsCircleStructureLoop(                loop,PreErr5_10,PreErr5_10) || loop.Size() < 2)
        return;
    int i = 0,n = 0;
    ElementPreSolve psol;
    while (i < int(loop.Size())) {
        if(loop.IsOpenLoop() && i == loop.Size() - 1){break;}
        n = loop.NextIndex(i);
        if(loop[i]._elemType != OffsetEnum::ARCTYPE ||
                loop[n]._elemType != OffsetEnum::ARCTYPE){
            i ++;
            continue;
        }
        auto& parc = loop[i]._arc;
        auto& narc = loop[n]._arc;
        parc.ChoiceSetRadius(PreErr5_3);
        narc.ChoiceSetRadius(PreErr5_3);
        if(std::abs(parc._arcR - narc._arcR) >= rErr){
            i ++;
            continue;
        }
        bool isSame = false;
        const auto& cp1 = parc._arcCp;
        const auto& cp2 = narc._arcCp;
        isSame = cp1.IsSamePoint2D(cp2,cpErr);
        if(!isSame){
            i ++;
            continue;
        }
        const auto& sp = parc._arcSp;
        const auto& ep = narc._arcEp;
        double R = (parc._arcR + narc._arcR)/2;
        DefArc arc (sp,ep,cp1.Midpoint(cp2),R,parc._arcRot);
        psol.ResetArcCenter(arc);
        loop.m_vElem[i]._arc = arc;
        loop.m_vElem.erase(loop.m_vElem.begin() + n);
    }
}
void OperateLoop::CircleStructureArcLoop(DefLoop& loop,double cpDisErr,double 
closeErr)
{
    if(loop.Size() < 2)
        return;
    int n = 0;
    for (size_t i = 0; i < loop.Size(); ++i) {
        n = loop.NextIndex(i);
        if(loop[i].ElemeType() != ARCTYPE || loop[n].ElemeType() != ARCTYPE)
            return;
        auto& parc = loop[i]._arc;
        auto& narc = loop[n]._arc;
        if(parc._arcRot != narc._arcRot)
            return;
        bool isSame_ = false;
        const auto& cp1 = parc._arcCp;
        const auto& cp2 = narc._arcCp;
        isSame_ = cp1.IsSamePoint2D(cp2,cpDisErr);
        if(!isSame_)
            return;
    }
    size_t last = loop.Size() - 1;
    const auto& sp_ = loop[0]._arc._arcSp;    const auto& ep_ = loop[last]._arc._arcEp;
    bool isSame = false;
    isSame = sp_.IsSamePoint2D(ep_,closeErr);
    if(!isSame)
        return;
#if 0
    const auto& sp = loop[0]._arc._arcSp;
    const auto& ep = loop[0]._arc._arcEp;
    const auto& cp = loop[0]._arc._arcCp;
    double R1 = std::sqrt(std::pow(sp._x - cp._x, 2) + pow(sp._y - cp._y, 2));
    double R2 = std::sqrt(std::pow(ep._x - cp._x, 2) + pow(ep._y - cp._y, 2));
    double R = (R1 + R2) / 2;
#else
    double cx = 0,cy = 0,rs = 0;
    for(auto& d : loop.m_vElem){
        d._arc.ChoiceSetRadius(PreErr5_3);
        cx += d._arc._arcCp.X();
        cy += d._arc._arcCp.Y();
        rs += d._arc._arcR;
    }
    double R = rs / loop.Size();
    double x = cx / loop.Size(),y = cy / loop.Size();
    Point cp(x,y,sp_.Z());
#endif
    DefCircle circle(cp,R);
    loop.ClearElem();
    loop.PushBack(DefElem(CIRCLETYPE, circle));
}
void OperateLoop::ArcLoopCircleStructure(OffsetStruct::DefLoop& loop,
                                         const NumStruct::ReLoopErr &err)
{
    int n = 0;
    bool isSame = false;
    for (size_t i = 0; i < loop.Size(); ++i) {
        n = loop.NextIndex(i);
        if(loop[i].ElemeType() != ARCTYPE || loop[n].ElemeType() != ARCTYPE)
            return;
        isSame = false;
        auto& parc = loop[i]._arc;
        auto& narc = loop[n]._arc;
        if(parc._arcRot != narc._arcRot)
            return;
        const auto& cp1 = parc._arcCp;
        const auto& cp2 = narc._arcCp;
        isSame = cp1.IsSamePoint2D(cp2,err.RelCpErr());
        if(!isSame)
            return;
        parc.ChoiceSetRadius();
        narc.ChoiceSetRadius();
        if(std::abs(parc._arcR - narc._arcR) > err.RelRadErr())
            return;
    }
    size_t last = loop.Size() - 1;
    const auto& sp_ = loop[0]._arc._arcSp;
    const auto& ep_ = loop[last]._arc._arcEp;
    bool isSame_ = false;
    isSame_ = sp_.IsSamePoint2D(ep_,err.RelCloseErr());
    if(!isSame_)
        return;
    double cx = 0,cy = 0,rs = 0;
    for(auto& d : loop.m_vElem){
        cx += d._arc._arcCp.X();        cy += d._arc._arcCp.Y();
        rs += d._arc._arcR;
    }
    double R = rs / loop.Size();
    double x = cx / loop.Size(),y = cy / loop.Size();
    Point cp(x,y,sp_.Z());
    DefCircle circle(cp,R);
    loop.ClearElem();
    loop.PushBack(DefElem(CIRCLETYPE, circle));
}
#include "LoopDirection.h"
#if 0
#include<iostream>
#pragma optimize("", off)
#endif
void LoopDirection::GetLoopRotDirection(const DefLoop& loop,
                                        RotDir& rot)
{
    if(loop.m_vElem.size() < 2){
        if(loop.m_vElem.size() == 1 &&
                loop.m_vElem[0]._elemType == CIRCLETYPE){
            rot = CLOCKWISE;
        }
        return;
    }
    ElemCalculate numcal;
    ElementPrecision elepre;
    DefLoop temLoop = loop,segLoop;
    elepre.DeleteInvalidElement(PreErr5_5,PreErr5_5,temLoop);
    ResetEleCoincideEndpoint(temLoop);
    DelSlopIdenticalLine(temLoop);
    double expStep = 5;
    GeometryLogic golg;
    golg.LoopToSegmentLoop(temLoop,expStep,segLoop);
    if(segLoop.m_vElem.size() < 2){
        rot = CLOCKWISE;
        return;
    }
    int max,pre,nex;
    GetMaxtXCoodEndPnEleIdx(segLoop,max);
    pre = numcal.PreviousIndex(segLoop.ElemNum(),max);
    nex = numcal.NextIndex(segLoop.ElemNum(),max);
    Point P1 = segLoop.m_vElem[pre]._segment._segEp;
    Point P2 = segLoop.m_vElem[max]._segment._segEp;
    Point P3 = segLoop.m_vElem[nex]._segment._segEp;
    GeomCalculate gocal;
    gocal.TriangleRotDirection(P1,P2,P3,rot);
}
void LoopDirection::ResetEleCoincideEndpoint(DefLoop& loop)
{
    TupleQuality tup;
    ElemCalculate numcal;
    int i = 0,nex = 0;
    while (i < loop.ElemNum()) {
        nex = numcal.NextIndex(loop.m_vElem.size(),i);
        bool isLocateCur = false,isLocateNex = false;
        isLocateCur = tup.IsEleLocatedOnEle(loop.m_vElem[i],
                                            loop.m_vElem[nex],
                                            PreErr5_8);
        isLocateNex = tup.IsEleLocatedOnEle(loop.m_vElem[nex],
                                            loop.m_vElem[i],
                                            PreErr5_8);
        if(!isLocateCur && !isLocateNex){
            i ++;
            continue;
        }
        if(isLocateCur && isLocateNex){            /// 两元素完全重合.
            numcal.DeleteIndexElement(vector<int>{i,nex},loop.m_vElem);
        }
        else if(isLocateCur){
            /// 下一元素结束点在当前元素上 .
            Point ep;
            numcal.ElementEndpoint(loop.m_vElem[nex],true,ep);
            numcal.SetElementEndpoint(ep,true,loop.m_vElem[i]);
            loop.m_vElem.erase(loop.m_vElem.begin() + nex);
        }
        else{
            /// 当前元素起点在下一元素上 .
            Point sp;
            numcal.ElementEndpoint(loop.m_vElem[i],false,sp);
            ///numcal.SetElementEndpoint(sp,false,loop.m_vElem[i]);
            numcal.SetElementEndpoint(sp,false,loop.m_vElem[nex]);
            loop.m_vElem.erase(loop.m_vElem.begin() + i);
        }
    }
}
void LoopDirection::DelSlopIdenticalLine(DefLoop& loop)
{
    if(loop.m_vElem.size() < 3){
        return;
    }
    int i = 0;
    GeomCalculate gocal;
    ElemCalculate numcal;PointOnElement onele;
    while (i < loop.m_vElem.size()) {
        if(loop.m_vElem[i]._elemType != LINETYPE ||
                loop.m_vElem.size() < 2){
            i ++;
            continue;
        }
        int nex;
        nex = numcal.NextIndex(loop.m_vElem.size(),i);
        if(loop.m_vElem[nex]._elemType != LINETYPE){
            i ++;
            continue;
        }
        Point lSp = loop.m_vElem[i]._segment._segSp;
        Point lEp = loop.m_vElem[i]._segment._segEp;
        Point lSp1 = loop.m_vElem[nex]._segment._segSp;
        Point lEp1 = loop.m_vElem[nex]._segment._segEp;
        double slop1,slop2;
        gocal.SegmentAngle(lSp,lEp,slop1);
        gocal.SegmentAngle(lSp1,lEp1,slop2);
        if(std::abs(std::abs(slop1 - slop2) - 180) > PreErr5_10){
            i ++;
            continue;
        }
        double len1,len2;
        gocal.TwoPointDistance(lSp,lEp,len1);
        gocal.TwoPointDistance(lSp1,lEp1,len2);
        if(std::abs(len1 - len2) < PreErr5_11){
            loop.m_vElem.erase(loop.m_vElem.begin() + i);
            ///loop.m_vElem.erase(loop.m_vElem.begin() + nex);
            if(i >= loop.m_vElem.size() - 1){
                loop.m_vElem.erase(loop.m_vElem.begin());
                continue;
            }            loop.m_vElem.erase(loop.m_vElem.begin() + i);
            continue;
        }
        bool isSp1OnSig = false;
        onele.IsPointOnSegment(lSp1,lEp1,lSp,PreErr5_10,
                               PreErr5_10,false,isSp1OnSig);
        if(isSp1OnSig){
            loop.m_vElem[nex]._segment._segSp = lSp;
            loop.m_vElem.erase(loop.m_vElem.begin() + i);
            continue;
        }
        bool isEp1OnSig = false;
        onele.IsPointOnSegment(lSp,lEp,lEp1,PreErr5_10,
                               PreErr5_10,false,isEp1OnSig);
        if(isEp1OnSig){
            loop.m_vElem[i]._segment._segEp = lEp1;
            loop.m_vElem.erase(loop.m_vElem.begin() + nex);
            continue;
        }
        i ++;
    }
}
void LoopDirection::GetMaxtXCoodEndPnEleIdx(const DefLoop& loop,int& index)
{
    if(loop.m_vElem.empty()){
        return;
    }
    index = 0;
    double maxVal = loop.m_vElem[0]._segment._segEp.X();
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._segment._segEp.X() > (maxVal + PreErr5_12)){
            maxVal = loop.m_vElem[i]._segment._segEp.X();
            index = i;
        }
    }
}
void LoopDirection::LoopDirectionPreprocess(DefLoop &loop)
{
    ResetEleCoincideEndpoint(loop);
    GeomCalculate gocal;
    vector<DefElem>vElem;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        if(gocal.ElementLenth(loop.IndexElem(i)) > PreErr5_6){
            vElem.push_back(loop.IndexElem(i));
        }
    }
    loop.SetLoopElem(vElem);
}
void LoopDirection::LoopRotateDirection(const DefLoop& loop_,
                                        RotDir& rot)
{
    GeometryLogic golg;
    if(golg.IsCircleStructureLoop(loop_) ||
            loop_.ElemNum() < 2){
        rot = CLOCKWISE;
        return;
    }
    DefLoop loop = loop_;
    LoopDirectionPreprocess(loop);    if(loop.IsEmpty()){
        rot = CLOCKWISE;
        return;
    }
    int index = 0;
    double coord = 0;
    GeomCalculate gocal;LimitCoordinate limcoo;
    limcoo.ElementLimitCoord(loop.IndexElem(0),true,true,coord);
    for (int i = 1; i < loop.ElemNum(); ++i) {
        double coord_ = 0;
        limcoo.ElementLimitCoord(loop.IndexElem(i),true,true,coord_);
        if(coord_ > coord){
            coord = coord_;
            index = i;
        }
    }
    Point P;
    DefElem ele = loop.IndexElem(index);
    if(ele.ElemeType() == LINETYPE){
        P = ele.ElemSeg().SegStartPt().X() >=
                ele.ElemSeg().SegEndPt().X() ?
                    ele.ElemSeg().SegStartPt() :
                    ele.ElemSeg().SegEndPt();
    }
    else if(ele.ElemeType() == ARCTYPE){
        ele._arc.IniArcRadius();
        limcoo.ArcLimitHorVerCoord(ele.ElemArc(),true,true,P);
    }
    ElemCalculate elecal;
    bool isSp = false,isEp = false;
    double expStep = PreErr5_3,stepVaue1 = PreErr5_3,stepVaue2 = PreErr5_3;
    elecal.IsElementEndpoint(ele,P,PreErr5_6,true,isSp);
    elecal.IsElementEndpoint(ele,P,PreErr5_6,false,isEp);/**
    double d1,d2;
    EqudDebug().ArcGeomInfo(ele.ElemArc(),d1,d2);*/
    Point P1,P3;
    if(isSp){
        int pre = elecal.PreviousIndex(loop.ElemNum(),index);
        DefElem preEle = loop.IndexElem(pre);
        elecal.ElementEndpointStepVal(preEle,expStep,stepVaue1);
        elecal.ElementEndpointStepVal(ele,expStep,stepVaue2);
        ApproachCurveEndpoint(preEle,stepVaue1,true,false,P1);
        ApproachCurveEndpoint(ele,stepVaue2,true,true,P3);
    }
    if(isEp){
        int nex = elecal.NextIndex(loop.ElemNum(),index);
        DefElem nexEle = loop.IndexElem(nex);
        elecal.ElementEndpointStepVal(ele,expStep,stepVaue1);
        elecal.ElementEndpointStepVal(nexEle,expStep,stepVaue2);
        ApproachCurveEndpoint(ele,stepVaue1,true,false,P1);
        ApproachCurveEndpoint(nexEle,stepVaue2,true,true,P3);
    }
    if(!isSp && !isEp){
        DefElem preEle,nexEle;
        elecal.PointSplitElement(ele,P,true,preEle);
        elecal.PointSplitElement(ele,P,false,nexEle);
        elecal.ElementEndpointStepVal(preEle,expStep,stepVaue1);
        elecal.ElementEndpointStepVal(nexEle,expStep,stepVaue2);
        ApproachCurveEndpoint(preEle,stepVaue1,true,false,P1);
        ApproachCurveEndpoint(nexEle,stepVaue2,true,true,P3);
    }
    gocal.TriangleRotDirection(P1,P,P3,rot);
}RotDir LoopDirection::LoopRotateDirection(const DefLoop& loop_)
{
    RotDir rot = CLOCKWISE;
#if 0
    LoopRotateDirection(loop_,rot);
#else
    GeometryLogic golg;
    if(golg.IsCircleStructureLoop(loop_) ||
            loop_.ElemNum() < 2){
        return CLOCKWISE;
    }
    DefLoop loop = loop_;
    LoopDirectionPreprocess(loop);
    if(loop.IsEmpty()){
        return CLOCKWISE;
    }
    ///OperateOftString().PrintLoopData(loop);
    ///string str = OperateOftString().LoopElementString(loop);
    vector<std::pair<int,Point>>vIdCoordPt;
    GeomCalculate gocal;LimitCoordinate limcoo;
    for (int i = 0; i < loop.ElemNum(); ++i) {
        Point p;
        limcoo.ElemLimitCoordPoint(loop.IndexElem(i),true,true,p);
        vIdCoordPt.push_back(std::make_pair(i,p));
    }
    vector<double>vCoord ;
    for(auto &d : vIdCoordPt){
        vCoord.push_back(d.second.X());
    }
    vector<int>vIndex;
    vector<std::pair<int,Point>>vIdCoordPt_;
    double coord = MathCalculate().VectorExtremum(vCoord,true);
    for (int i = 0; i < vIdCoordPt.size(); ++i) {
        if(vIdCoordPt[i].second.X() == coord){
            vIdCoordPt_.push_back(vIdCoordPt[i]);
            vIndex.push_back(i);
        }
    }
    if(vIdCoordPt_.empty()){
        return CLOCKWISE;
    }
    Point P1,P2,P3;
    if(!ApplicableLimitCoordPoint(loop,vIdCoordPt_,P1,P2,P3)){
        return CLOCKWISE;
    }
    gocal.TriangleRotDirection(P1,P2,P3,rot);
#endif
    return rot;
}
bool LoopDirection::ApplicableLimitCoordPoint(const DefLoop& loop,
                                              const 
vector<std::pair<int,Point>>&vIdCoordPt,
                                              Point &P1,Point &P2,Point &P3)
{
    double stepVal = PreErr5_3;
    for (int i = 0; i < vIdCoordPt.size(); ++i) {
        int index = vIdCoordPt[i].first;
        P2 = vIdCoordPt[i].second;
        if(SearchSmallerPoint(loop,index,P2,stepVal,P1,P3)){
            return true;
        }    }
    int index = vIdCoordPt[0].first;
    P2 = vIdCoordPt[0].second;
    return ExploreSmallerPoint(loop,index,P2,P1,P3);
}
bool LoopDirection::SearchSmallerPoint(const DefLoop &loop,int index,
                                       const Point &P,double stepVal,
                                       Point &prePt,Point &nexPt)
{
    ElemCalculate elecal;
    GeomCalculate gocal;GeometryLogic golg;
    DefElem ele = loop.IndexElem(index),preEle,nexEle;
    elecal.PointSplitElement(ele,P,true,preEle);
    elecal.PointSplitElement(ele,P,false,nexEle);
    double len1 = gocal.ElementLenth(preEle),
            len2 = gocal.ElementLenth(nexEle);
    int iter = 0,pre = index,nex = index;
    bool isPreFinish = false,isNexFinsh = false,isFinish = false;
    if(len1 >= stepVal){
        isPreFinish = true;
    }
    else{
        pre = loop.PreviousIndex(index);
    }
    if(len2 >= stepVal){
        isNexFinsh = true;
    }
    else{
        nex = loop.NextIndex(index);
    }
    if(isPreFinish && isNexFinsh){
        golg.ApproachCurveEndpoint(preEle,stepVal,false,false,prePt);
        golg.ApproachCurveEndpoint(nexEle,stepVal,false,true,nexPt);
        return prePt.X() < P.X() || nexPt.X() < P.X();
    }
    double stepVal1 = stepVal,stepVal2 = stepVal;
    while (!isFinish) {
        if(!isPreFinish){
            double l = gocal.ElementLenth(loop.IndexElem(pre));
            if(len1 + l >= stepVal){
                stepVal1 = stepVal - len1;
                isPreFinish = true;
            }
            else{
                len1 += l;
                pre = loop.PreviousIndex(pre);
            }
        }
        if(!isNexFinsh){
            double l = gocal.ElementLenth(loop.IndexElem(nex));
            if(len2 + l >= stepVal){
                stepVal2 = stepVal - len2;
                isNexFinsh = true;
            }
            else{
                len2 += l;
                nex = loop.NextIndex(nex);
            }
        }
        if(isPreFinish && isNexFinsh){
            break;
        }
        iter ++;        if(iter > loop.ElemNum() / 2){
            return false;
        }
    }
    golg.ApproachCurveEndpoint(loop.IndexElem(pre),
                                 stepVal1,false,false,prePt);
    golg.ApproachCurveEndpoint(loop.IndexElem(nex),
                                 stepVal2,false,true,nexPt);
    return prePt.X() < P.X() || nexPt.X() < P.X();
}
bool LoopDirection::ExploreSmallerPoint(const DefLoop &loop,int index,
                                        const Point &P,
                                        Point &prePt,Point &nexPt)
{
    ElemCalculate elecal;
    GeomCalculate gocal;GeometryLogic golg;
    DefElem ele = loop.IndexElem(index),preEle,nexEle;
    elecal.PointSplitElement(ele,P,true,preEle);
    elecal.PointSplitElement(ele,P,false,nexEle);
    double len1 = gocal.ElementLenth(preEle),
            len2 = gocal.ElementLenth(nexEle);
    bool isFinish1 = false,isFinish2 = false,isFinish = false;
    int iter = 0,pre = index,nex = index;
    while (!isFinish) {
        double stepVal = PreErr_2;
        if(!isFinish1){
            vector<Point>vPoint;
            if(preEle.ElemeType() == LINETYPE){
                stepVal = 2;
            }
            golg.DiscreteElemToPoint(preEle,false,stepVal,vPoint);
            std::reverse(vPoint.begin(),vPoint.end());
            for(auto &p_ : vPoint){
                double l = p_.Distance2D(P);
                if(l + len1 >= PreErr5_3){
                    bool isVal = false;
                    if(isFinish2){
                        if(p_.X() <= P.X()){
                            isVal = true;
                        }
                    }
                    else{
                        if(p_.X() < P.X()){
                            isVal = true;
                        }
                    }
                    if(isVal){
                        prePt = p_;
                        isFinish1 = true;
                        break;
                    }
                }
                len1 += l;
            }
            if(!isFinish1){
                pre = loop.PreviousIndex(pre);
                preEle = loop.IndexElem(pre);
            }
        }
        if(!isFinish2){
            vector<Point>vPoint;            if(nexEle.ElemeType() == LINETYPE){
                stepVal = 2;
            }
            golg.DiscreteElemToPoint(nexEle,false,stepVal,vPoint);
            for(auto &p_ : vPoint){
                double l = p_.Distance2D(P);
                if(l + len2 >= PreErr5_3){
                    bool isVal = false;
                    if(isFinish1){
                        if(p_.X() <= P.X()){
                            isVal = true;
                        }
                    }
                    else{
                        if(p_.X() < P.X()){
                            isVal = true;
                        }
                    }
                    if(isVal){
                        nexPt = p_;
                        isFinish2 = true;
                        break;
                    }
                }
                len2 += l;
            }
            if(!isFinish2){
                nex = loop.NextIndex(nex);
                nexEle = loop.IndexElem(nex);
            }
        }
        if(isFinish1 && isFinish2){
            break;
        }
        iter ++;
        if(iter > loop.ElemNum() / 2){
            return false;
        }
    }
    return true;
}
void LoopDirection::CloseArcEndpointBaseAngle(DefArc arc,
                                              bool isCloseEp,
                                              double stepAgl,
                                              Point& P)
{
    GeomCalculate gocal;
    double startAgl,endAgl;
    gocal.ArcEndpointAngle(arc._arcSp,arc._arcEp,arc._arcCp,
                           arc._arcRot,startAgl,endAgl);
    double centerAgl;
    gocal.TwoAngleIncludedAngle(startAgl,endAgl,arc._arcRot,centerAgl);
    double stepVal_ = stepAgl;
    if(stepVal_ >= centerAgl){
        stepVal_ = centerAgl / 1.1;
    }
    double angle;
    gocal.CloseArcEndpointAngle(arc,stepVal_,isCloseEp,angle);
    ///arc.ChoiceSetRadius(PreErr5_2);
    double incx = arc.ArcRadius() * std::cos(angle * M_PI / 180);    double incy = arc.ArcRadius() * std::sin(angle * M_PI / 180);
    P.SetX(arc._arcCp.X() + incx);
    P.SetY(arc._arcCp.Y() + incy);
    P.SetZ(arc.ArcCenterPt().Z());
}
void LoopDirection::ApproachCurveEndpoint(const DefElem& ele,
                                            double stepVal,
                                            bool isAngle,
                                            bool isNearSp,
                                            Point& P)
{
    GeomCalculate go;GeometryLogic golg;
    switch (ele.ElemeType()) {
    case LINETYPE:
        go.CloseSegmentEndpoint(ele.ElemSeg().SegStartPt(),
                                ele.ElemSeg().SegEndPt(),
                                stepVal,isNearSp,P);
        break;
    case ARCTYPE:
        if(isAngle){
            CloseArcEndpointBaseAngle(ele.ElemArc(),!isNearSp,stepVal,P);
        }
        else{
            golg.CloseArcEndpointBaseLenth(ele.ElemArc(),!isNearSp,stepVal,P);
        }
        break;
    default:
        break;
    }
}
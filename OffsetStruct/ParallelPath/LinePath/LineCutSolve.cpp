#include <unordered_map>
#include "LineCutSolve.h"
#include "../../../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
using namespace GridArea;
using namespace GeomStruct;
using namespace oft;
using namespace GeomUnitStruct;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void LineCutSolve::SetLoopClose(OftStr::DefLoop& loop)
{
    ElemCalculate elecal;
    for (int i = 0; i < loop.ElemNum()-1; ++i) {
        DefElem& pre = loop.m_vElem[i];
        DefElem& nex = loop.m_vElem[i+1];
        OftStr::Point s = elecal.ElementEndpoint(nex,false);
        elecal.SetElementEndpoint(s,true,pre);
    }
    DefElem fir = loop.FirstElem();
    DefElem las = loop.LastElem();
    OftStr::Point sp = elecal.ElementEndpoint(fir,false);
    elecal.SetElementEndpoint(sp,true,las);
    loop.SetLastElem(las);
}
double LineCutSolve::LoopMaxZCoord(const OftStr::DefLoop& loop){
    ElemCalculate elecal;
    double z = Min_Val;
    for(auto& d : loop.m_vElem){
        OftStr::Point sp = elecal.ElementEndpoint(d,false),
                ep = elecal.ElementEndpoint(d,true);
        if(z < sp.Z()){z = sp.Z();}
        if(z < ep.Z()){z = ep.Z();}
    }
    return z;
}
void LineCutSolve::ClassfyLoopsBaseZ(const vector<DefLoop>& vLoop,
                                     vector<GeomArea>& areas)
{
#if 0
    double err = 1.5;
    vector<std::pair<DefLoop,double>>loopInf;
    for(auto& d : vLoop){
        double z = LoopMaxZCoord(d);
        loopInf.emplace_back(std::make_pair(d,z));
    }
    std::sort(loopInf.begin(), loopInf.end(),
              [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    vector<pair<DefLoop,double>>curGroup;
    vector<vector<std::pair<DefLoop,double>>>result;
    double groupStart = loopInf[0].second;
    curGroup.push_back(loopInf[0]);
    for (size_t i = 1; i < loopInf.size(); ++i) {
        if (std::abs(loopInf[i].second - groupStart) <= err) {
            curGroup.emplace_back(loopInf[i]);
        }        else {
            result.emplace_back(curGroup);
            curGroup.clear();
            groupStart = loopInf[i].second;
            curGroup.emplace_back(loopInf[i]);
        }
    }
    if (!curGroup.empty()) {
        result.emplace_back(curGroup);
    }
#endif
    double r = 0.1,g = 0.1,b = 0.1;
    ///MultipleInclude mulinc;DisplayGeom disgo;
#if 0
    for (int i = 0; i < vLoop.size(); ++i) {
        Quantity_Color c(r,g,b,Quantity_TOC_RGB);
        disgo.DisplayLoop(vLoop[i],c,1.5);
        r += 0.5;g += 0.3;b += 0.2;
        if(r>1){r=0;}if(g>1){g=0;}if(b>1){b=0;}
    }
#endif
    ///string str = OperateOftString().ArrayLoopString(vLoop);
    ///cout<<str;
    ///double intPre = PreErr5_6,coinPre = PreErr5_6,incPre = PreErr5_6;
    ///mulinc.OrganizeLoopToArea(vLoop,intPre,coinPre,incPre,areas);
#if 0
    for (int i = 0; i < result.size(); ++i) {
        Quantity_Color c(r,g,b,Quantity_TOC_RGB);
        vector<DefLoop>ls;
        for(auto& d : result[i]){
            ls.emplace_back(d.first);
        }
        disgo.DisplayLoops(ls,c,1.5);
        r += 0.5;g += 0.3;b += 0.2;
        if(r>1){r=0;}if(g>1){g=0;}if(b>1){b=0;}
    }
#endif
#if 0
    for (int i = 0; i < areas.size(); ++i) {
        Quantity_Color c(r,g,b,Quantity_TOC_RGB);
        disgo.DisplayGeomArea(areas[i],c,1.5);
        r += 0.5;g += 0.3;b += 0.2;
        if(r>1){r=0;}if(g>1){g=0;}if(b>1){b=0;}
    }
#endif
}
void LineCutSolve::ResetLoopOrderBaseX(OftStr::DefLoop& loop)
{
    int id = 0;
    double x = loop.IndexElem(0).ElemSeg().SegStartPt().X();
    for (int i = 0; i < loop.ElemNum(); ++i) {
        if(loop.IndexElem(i).ElemSeg().SegStartPt().X() < x){
            x = loop.IndexElem(i).ElemSeg().SegStartPt().X();
            id = i;
        }
    }    OftStr::DefLoop res = loop;
    res.ClearElem();
    for (int i = id; i < loop.ElemNum(); ++i) {
        res.PushBackElem(loop.IndexElem(i));    }
    for (int i = 0; i < id; ++i) {
        res.PushBackElem(loop.IndexElem(i));
    }
    loop = res;
}
void LineCutSolve::LineCutStartLine(const DefLoop& loop,
                                    double dist,double agl,
                                    OftStr::Point& p1,
                                    OftStr::Point& p2)
{
#if 0
    DisplayGeom play;
    Quantity_Color grren(0,0.5,0,Quantity_TOC_RGB);
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    DisplayGeom().DisplayLoop(loop,black,1,true);
    mainwind->myOccView->fitAll();
    play.DisplaySegment(p1,p2,black,1);
    mainwind->myOccView->myView->FitAll(0.7);
#endif
    OftStr::Point minp;
    LimitCoordinate limco;
    limco.LoopLimitCoordPoint(loop,true,false,minp);
    double len = 4000,radians = agl * M_PI / 180.0;
    double dx = std::cos(radians),dy = std::sin(radians);
    ///从左上角开始,需要agl小于180.
    OftStr::Point ext1(minp.X() - dx * len, minp.Y() - dy * len);
    OftStr::Point ext2(minp.X() + dx * len, minp.Y() + dy * len);
    if(agl < 90){
        if(ext1.X() > ext2.X()){
            std::swap(ext1,ext2);
        }
    }else{
        if(ext1.Y() > ext2.Y()){
            std::swap(ext1,ext2);
        }
    }
#if 0
    play.DisplaySegment(ext1,ext2,blue,1.5);
#endif
    bool isIntersect = false;
    GeomIntersect goint;EquidistantCurve equ;
    DefElem elem(LINETYPE,DefSeg(ext1,ext2));
    double oftDist = dist,endPre = PreErr_8,intPre = PreErr_8,
            tanPre = PreErr_8;
    while (true) {
        isIntersect = false;
        for (int i = 0; i < loop.ElemNum(); ++i) {
            GeomIntRes intRes;
            goint.CalculateCurveIntersect(elem,loop.IndexElem(i),true,
                                          tanPre,endPre,intPre,intRes);
            if(intRes._isExistInt){
                isIntersect = true;
                break;
            }
        }
        if(!isIntersect){            p1 = elem._segment._segSp;
            p2 = elem._segment._segEp;
#if 0
            DisplayGeom().DisplayElement(elem,grren,1.5);
            int tem1 = 0;
#endif
            break;
        }
        OftStr::Point p1_,p2_;
        equ.EquidistantSegment(ext1,ext2,oftDist,
                               WITHINPROFILE,p1_,p2_);
        elem = DefElem(LINETYPE,DefSeg(p1_,p2_));
#if 0
        DisplayGeom().DisplayElement(elem,blue,1.5);
#endif
        oftDist += dist;
    }
}
void LineCutSolve::ResetLineIntPoint(const DefLoop& loop,
                                     const OftStr::Point& sp,
                                     const OftStr::Point& ep,
                                     double agl, double err,
                                     vector<IntPnt> &intPts)
{
   if(intPts.size() < 2)
        return;
    vector<int>indexs;
    ResetIntPntOrder(agl,intPts);
    ElemCalculate elecal;MathCalculate matcal;
    for (int i = 0; i < intPts.size() - 1; ++i) {
        if(!intPts[i]._isEndpt)
            continue;
        const int& e1 = intPts[i]._eId;
        const int& e2 = intPts[i+1]._eId;
        const Point& p1 = intPts[i]._p;
        const Point& p2 = intPts[i+1]._p;
        const DefElem& ele1 = loop.IndexElem(e1);
        const DefElem& ele2 = loop.IndexElem(e2);
        bool isSp1 = false,isSp2 = false;
        isSp1 = elecal.IsElementEndpoint(ele1,p1,err,true) ? true : false;
        isSp2 = elecal.IsElementEndpoint(ele2,p2,err,true) ? true : false;
        Point p1_,p2_;
        bool isOrder1 = isSp1,isOrder2 = isSp2,sameSign = false;
        elecal.LoopPointBaseLenth(loop,e1,isOrder1,PreErr5_6,p1_);
        elecal.LoopPointBaseLenth(loop,e2,isOrder2,PreErr5_6,p2_);
        Point vec = ep - sp;
        Point vec1 = p1_ - sp;
        Point vec2 = p2_ - sp;
        double z1 = vec.Cross2D(vec1);
        double z2 = vec.Cross2D(vec2);
        sameSign = (z1 >= 0 && z2 >= 0) || (z1 <= 0 && z2 <= 0);
        if(!sameSign){
            indexs.emplace_back(i+1);
        }
        i ++;
    }
    vector<IntPnt>intPts_;
    if(indexs.empty()){
        intPts_ = intPts;    }else{
        for (int i = 0; i < intPts.size(); ++i) {
            if(!matcal.IsValueIn2DArray(indexs,i)){
                intPts_.emplace_back(intPts[i]);
            }
        }
    }
    if(intPts_.size() < 2){
        intPts = intPts_;
        return;
    }
    indexs.clear();
    for (int i = 0; i < intPts_.size()-1; ++i) {
        if(intPts_[i]._p.Distance2D(intPts_[i+1]._p) <= err){
            indexs.emplace_back(i);
            indexs.emplace_back(i+1);
            i ++;
        }
    }
    intPts.clear();
    for (int i = 0; i < intPts_.size(); ++i) {
        if(!matcal.IsValueIn2DArray(indexs,i)){
            intPts.emplace_back(intPts_[i]);
        }
    }
    
}
/**
void LineCutSolve::ResetLineIntPoint(const vector<DefLoop>& loops,
                                     double err, vector<IntPnt> &intPts)
{
    if(intPts.size() < 2)
        return;
    vector<int>vIndex;
    ElemCalculate elecal;PntInPolySolve insol;
    for (int i = 0; i < intPts.size(); ++i) {
        bool isSp = false,isSp_ = false,isMax = false,isMax1 = false;
        const DefElem& ele = loops[intPts[i]._lId].IndexElem(intPts[i]._eId);
        const DefElem& ele_ = 
loops[intPts[i+1]._lId].IndexElem(intPts[i+1]._eId);
        isSp = elecal.IsElementEndpoint(ele,intPts[i].P(),err,true) ?
                    true : false;
        isSp_ = elecal.IsElementEndpoint(ele_,intPts[i+1].P(),err,true) ?
                    true : false;
        insol.IsEleExtrMaxOrdinate(ele,isSp,isMax);
        insol.IsEleExtrMaxOrdinate(ele_,isSp_,isMax1);
        if(isMax != isMax1){
            vIndex.push_back(i+1);
        }
        i ++;
    }
    if(vIndex.empty())
        return;
    vector<bool> to_remove(intPts.size(), false);
    for (int i : vIndex) {
        to_remove[i] = true;
    }
    intPts.erase(std::remove_if(intPts.begin(),intPts.end(),
                                [&to_remove, &intPts](const IntPnt& p) {
        return to_remove[&p - &intPts[0]];}),intPts.end());
    to_remove.clear();
    bool removed_previous = false;    to_remove.resize(intPts.size(), false);
    for (size_t i = 0; i < intPts.size() - 1; ++i) {
        if (removed_previous) {
            removed_previous = false;
            continue;
        }
        if (intPts[i]._p.Distance2D(intPts[i+1]._p) < err) {
            to_remove[i] = true;
            to_remove[i+1] = true;
            removed_previous = true; /// 跳过下一个点(避免重复处理)
        }
    }
    intPts.erase(std::remove_if(intPts.begin(),intPts.end(),
                                [&to_remove, &intPts](const IntPnt& p) {
        return to_remove[&p - &intPts[0]]; }),intPts.end());
}
*/
void LineCutSolve::OrgnizeIntPointBaseElem(const DefLoop& loop,
                                           vector<IntPnt>& intPts)
{
    ElemCalculate elecal;GeomDistance godis;
    vector<IntPnt> intPts_ = intPts;
    intPts.clear();
    std::sort(intPts_.begin(), intPts_.end(), [](const IntPnt& a,
              const IntPnt& b) {return a._eId < b._eId;});
#if 0
    DisplayGeom play;
    Quantity_Color yellow(0.7,0.7,0,Quantity_TOC_RGB);
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);
#endif
#if 0
    DisplayGeom().DisplayLoop(loop,black,1,true);
    mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts_.size(); ++i) {
        play.DisplayPoint(intPts_[i].P(),blue,true,i);
    }
#endif
    vector<IntPnt>curGroup;
    int curId = intPts_[0]._eId;
    vector<std::pair<int,vector<IntPnt>>>intPtsPair;
    for (const IntPnt& d : intPts_) {
        if (d._eId == curId) {
            curGroup.emplace_back(d);
        } else {
            intPtsPair.emplace_back(std::make_pair(curId,curGroup));
            curId = d._eId;
            curGroup = {d};
        }
    }
    intPtsPair.emplace_back(std::make_pair(curId,curGroup));
    for (int i = 0; i < intPtsPair.size(); ++i) {
        const vector<IntPnt>& ps = intPtsPair[i].second;
        vector<std::pair<IntPnt,double>>ptDist;
        for(auto& p : ps){
            double dist;
            godis.PointToEleEndpointDistance(
                        loop.IndexElem(p._eId),p._p,true,dist);
            ptDist.emplace_back(std::make_pair(p,dist));
        }        std::sort(ptDist.begin(),ptDist.end(),[](
                  const std::pair<IntPnt,double>& a,
                  const std::pair<IntPnt,double>& b) {
            return a.second < b.second;});
        for(auto& d : ptDist){
            intPts.emplace_back(d.first);
        }
    }
    for (int i = 0; i < intPts.size(); ++i) {
        intPts[i]._id = i;
        intPts[i]._lId = loop._id;
    }
    for (int i = 0; i < intPts.size(); ++i) {
        int p = elecal.PreviousIndex(intPts.size(),i),
                n = elecal.NextIndex(intPts.size(),i);
        intPts[i].pre = &intPts[p];
        intPts[i].nex = &intPts[n];
    }
#if 0
    mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayLoop(loop,black,1,true);
    mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayLoop(loop,black,1,true);
        play.DisplayPoint(intPts[i].P(),red,true,i);
        play.DisplayPoint(intPts[i].pre->P(),blue);
        play.DisplayPoint(intPts[i].nex->P(),blue);
        mainwind->myOccView->getContext()->RemoveAll(true);
    }
    int tem = 1;
#endif
}
void LineCutSolve::OrgnizeIntPointBaseLoop(const GeomArea& area,
                                           vector<IntPnt>& intPts)
{
    if(intPts.size() < 2)
        return;
#if 0
    DisplayGeom play;
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color red(0.7,0,0,Quantity_TOC_RGB);
    Quantity_Color purple(0.6,0,0.6,Quantity_TOC_RGB);
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);
    Quantity_Color yellow(0.8,0.8,0,Quantity_TOC_RGB);
    Quantity_Color brown(0.5,0.2,0.0,Quantity_TOC_RGB);
    Quantity_Color cyan(0,0.8,0.8,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    vector<Quantity_Color>_colors{green,blue,red,purple,
                orange,yellow,brown,cyan,black};
    for(auto& d : _colors){_colors.emplace_back(d);}
    for(auto& d : _colors){_colors.emplace_back(d);}
#endif
    vector<DefLoop>loops{area.BndLoop()};
    for(const auto& d : area._vIslLoop){
        loops.emplace_back(d);
    }
#if 0
    play.DisplayGeomArea(area,black,1.5);
    mainwind->myOccView->fitAll();
#endif
#if 0    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayPoint(intPts[i].P());
    }
#endif
    ElemCalculate elecal;
    std::unordered_map<int,vector<IntPnt>>groups;
    for (const auto& d : intPts) {
        groups[d._lId].emplace_back(d);
    }
    intPts.clear();
    vector<std::pair<int,vector<IntPnt>>>loIntPts(groups.begin(),groups.end());
    std::sort(loIntPts.begin(), loIntPts.end(),
              [](const std::pair<int, std::vector<IntPnt>>& a,
              const std::pair<int, std::vector<IntPnt>>& b) {
        return a.first < b.first;});
    for (int l = 0; l < loIntPts.size(); ++l) {
        DefLoop loop;
        for(auto& d : loops){
            if(d._id == loIntPts[l].first){
                loop = d;
                break;
            }
        }
        std::vector<IntPnt>& intps = loIntPts[l].second;
        OrgnizeIntPointBaseElem(loop,intps);
#if 0
        for (int i = 0; i < intps.size(); ++i) {
            const IntPnt& pt = intps[i];
            play.DisplayPoint(pt.P(),true,pt._id);
        }
#endif
        if(intps.empty())
            continue;
        int s = intPts.size();
        for (int i = 0; i < intps.size(); ++i) {
            intPts.push_back(intps[i]);
        }
        int e = intPts.size()-1,p = 0,n = 0;
        for (int i = s; i < intPts.size(); ++i) {
            p = elecal.PreviousIndex(intPts.size(),i),
                    n = elecal.NextIndex(intPts.size(),i);
            intPts[i].pre = &intPts[p];
            intPts[i].nex = &intPts[n];
        }
        intPts[s].pre = &intPts[e];
        intPts[e].nex = &intPts[s];
    }
    for (int i = 0; i < intPts.size(); ++i) {
        intPts[i]._id = i;
    }
#if 0
    mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,black,1.5);
    for (int i = 0; i < intPts.size(); ++i) {
        ///play.DisplayGeomArea(area,black,1.5);
        play.DisplayPoint(intPts[i].P(),red,true,i);
        play.DisplayPoint(intPts[i].pre->P(),blue);
        play.DisplayPoint(intPts[i].nex->P(),blue);
        ///mainwind->myOccView->getContext()->RemoveAll(true);
        int tem = 1;
    }
#endif
}void LineCutSolve::IntPointToCutLine(const GeomArea& area,
                                     double agl,
                                     vector<IntPnt>& intPts,
                                     vector<IntLine>& intLines)
{
#if 0
    DisplayGeom play;
    vector<DefLoop>loops = area.IslLoop();
    loops.emplace_back(area.BndLoop());
#endif
#if 0
    play.DisplayGeomArea(area,_colors.back(),1.5);
    mainwind->myOccView->fitAll();
#endif
#if 0
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayPoint(intPts[i].P(),_colors[1],
                true,intPts[i]._id);
    }
#endif
    std::unordered_map<int,std::vector<IntPnt*>> groups;
    for (auto& d : intPts) {
        groups[d._lineId].emplace_back(&d);
    }
    for (auto& d : groups) {
        IntLine b;
        b._id = d.first;
        b._vIntPt = d.second;
        intLines.emplace_back(b);
    }
    std::sort(intLines.begin(),intLines.end(), [](
              const IntLine& l1,const IntLine& l2){
        return l1._id < l2._id;});
    for(auto& d : intLines){
        ResetIntPntOrder(agl,d);
    }
    for (int i = 0; i < intLines.size(); ++i) {
        for (int j = 0; j < intLines[i]._vIntPt.size()-1; ++j) {
            intLines[i]._vIntPt[j]->cnt = intLines[i]._vIntPt[j+1];
            intLines[i]._vIntPt[j+1]->cnt = intLines[i]._vIntPt[j];
            j ++;
        }
    }
    auto deleteps = [](std::vector<IntPnt*>&ps,vector<int>&ids)->void{
        for (auto it = ids.rbegin();it != ids.rend(); ++it) {
            ps.erase(ps.begin() + (*it));
        }
    };
    for(auto& d : intLines){
        vector<int>ids;
        auto& ps = d._vIntPt;
        for (int i = 0; i < ps.size()-1; ++i) {
            if(ps[i]->P().Distance2D(ps[i]->cnt->P()) < PreErr5_6){
                ids.push_back(i);
                ids.push_back(i+1);
            }
            i ++;
        }
        if(!ids.empty()){
            deleteps(ps,ids);
        }
    }    intLines.erase(std::remove_if(intLines.begin(),intLines.end(),
                                  [](const IntLine& d){
        return d._vIntPt.empty();}),intLines.end());
#if 0
    double w = 1;
    mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,_colors.back(),1.5);
    mainwind->myOccView->fitAll();
    for (int i = 0; i < intLines.size(); ++i) {
        for (int j = 0; j < intLines[i]._vIntPt.size(); ++j) {
            w = intLines[i]._vIntPt[j]->P().X() <
                    intLines[i]._vIntPt[j]->cnt->P().X() ? 1.5 : 1;
            play.DisplayPoint(intLines[i]._vIntPt[j]->P(),_colors[1],
                    true,intLines[i]._vIntPt[j]->_id);
            play.DisplayPoint(intLines[i]._vIntPt[j]->cnt->P(),_colors[1],
                    true,intLines[i]._vIntPt[j]->cnt->_id);
            play.DisplaySegment(intLines[i]._vIntPt[j]->P(),
                                intLines[i]._vIntPt[j]->cnt->P(),
                                _colors[0],w,true,intLines[i]._id);
            j ++;
        }
    }
    int tem = 1;
#endif
}
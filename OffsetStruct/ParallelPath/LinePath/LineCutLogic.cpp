#include "LineCutLogic.h"
#include "LineCutSolve.h"
using namespace GeomUnit;
using namespace GridArea;
using namespace GeomStruct;
using namespace oft;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void LineCutLogic::LoopToToolLocationLoop(const vector<vector<Rectangle>>& 
vRecs,
                                          const DefLoop& loop,const double l,
                                          DefLoop& resloop)
{
    LineCutSolve linsol;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        OftStr::Point intPt1,intPt2;
        const OftStr::DefSeg& seg = loop.m_vElem[i]._segment;
        const OftStr::Point& sp = seg._segSp,ep = seg._segEp;
        const OftStr::Point& sn = *sp._lable._normal,en = *ep._lable._normal;
        if(!linsol.IsVertexNormalRecsInt(sp,sn,vRecs,l,intPt1) ||
                !linsol.IsVertexNormalRecsInt(ep,en,vRecs,l,intPt2))
            continue;
        resloop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(intPt1,intPt2)));
    }
}
void LineCutLogic::GenerateLoopLinePath(DefLoop loop,double dist,
                                        double agl,DefLoop& res)
{
    if(loop.IsEmpty())
        return;
#define _GenDebug 0
    LimitCoordinate limco;
    GeomIntersect goint;LineCutSolve linsol;
    EquidistantCurve equ;ElemCalculate elecal;
    double endptPre = PreErr5_8,intPre = PreErr5_8,tanPre = PreErr5_8;
#if _GenDebug
    Quantity_Color yellow(0.7,0.7,0,Quantity_TOC_RGB);
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);
    string str = OperateOftString().LoopElementString(loop);
    DisplayGeom().DisplayLoop(loop,black,1,true);
    mainwind->myOccView->fitAll();
    std::cout<<str<<endl;
#endif
    OftStr::Point ext1,ext2;
    linsol.LineCutStartLine(loop,dist,agl,ext1,ext2);
#if _GenDebug
    DisplayGeom().DisplayPoint(p0);
    OftStr::Point ext1_(p0.X() - dx * l, p0.Y() - dy * l);
    OftStr::Point ext2_(p0.X() + dx * l, p0.Y() + dy * l);
    DisplayGeom().DisplayElement(
                DefElem(LINETYPE,DefSeg(ext1_,ext2_)),yellow,1);
    DisplayGeom().DisplayElement(
                DefElem(LINETYPE,DefSeg(ext1,ext2)),blue,1.5);#endif
    int num = 0;
    PointInPolygon inpol;
    double oftDist = dist;
    while (true) {
        bool isExiEndpt = false;
        OftStr::Point p1_,p2_;
        vector<std::pair<OftStr::Point,bool>>vInt;
        equ.EquidistantSegment(ext1,ext2,oftDist,
                               OUTSIDEPROFILE,p1_,p2_);
        DefElem elem(LINETYPE,DefSeg(p1_,p2_));
#if _GenDebug
        DisplayGeom().DisplayElement(elem,blue,1.5);
#endif
        for (int i = 0; i < loop.ElemNum(); ++i) {
            GeomIntRes intRes;
            bool isEndpt = false;
            goint.CalculateCurveIntersect(elem,loop.m_vElem[i],
                                          false,tanPre,endptPre,
                                          intPre,intRes);
            if(!intRes._isExistInt)
                continue;
            if(elecal.IsElementEndpoint(loop.m_vElem[i],
                                        intRes._intPnt1,PreErr5_6)){
                isEndpt = true;
                isExiEndpt = true;
            }
            if(!isEndpt && intRes._intNum > 1 &&
                    elecal.IsElementEndpoint(loop.m_vElem[i],
                                             intRes._intPnt2,PreErr5_6)){
                isExiEndpt = true;
                isEndpt = true;
            }
            vInt.emplace_back(std::make_pair(intRes._intPnt1,isEndpt));
            if(intRes._intNum > 1){
                vInt.emplace_back(std::make_pair(intRes._intPnt2,isEndpt));
            }
#if _GenDebug
            DisplayGeom().DisplayPoint(intRes._intPnt1);
            DisplayGeom().DisplayPoint(intRes._intPnt2);
#endif
        }
        if(vInt.size() < 2 && num > 1)
            break;
        if(!isExiEndpt && !vInt.empty()){
            for (int i = 0; i < vInt.size()-1; ++i) {
                DefElem ele(LINETYPE,DefSeg(vInt[i].first,
                                            vInt[i+1].first));
                res.PushBackElem(ele);
                i ++;
#if _GenDebug
                DisplayGeom().DisplayElement(ele,green,1.5);
#endif
            }
        }
        else if(!vInt.empty()){
            for (int i = 0; i < vInt.size()-1; ++i) {
                bool isOnBorder = false,isInPoly = false;
                Point mp = vInt[i].first.Midpoint(vInt[i+1].first);
                inpol.IsPointInsideLoop(loop,mp,PreErr5_10,
                                        isOnBorder,isInPoly);
                if(isInPoly){
                    DefElem ele(LINETYPE,DefSeg(vInt[i].first,                                                vInt[i+1].first));
                    res.PushBackElem(ele);
#if _GenDebug
                    DisplayGeom().DisplayElement(ele,green,1.5);
#endif
                }
            }
        }
        oftDist += dist;
        num ++;
    }
    for(auto& e : res.m_vElem){
        if(e._segment._segSp.X() > e._segment._segEp.X()){
            OftStr::Point p = e._segment._segSp;
            e._segment._segSp = e._segment._segEp;
            e._segment._segEp = p;
        }
    }
#if _GenDebug
    string str1 = OperateOftString().LoopElementString(res);
    DisplayGeom().DisplayLoop(res,orange,1);
    std::cout<<str1<<endl;
    int tem = 0;
#endif
}
void LineCutLogic::LoopsClipLinePath(const vector<DefLoop>& loops,
                                     const DefLoop& loop,DefLoop& res)
{
    ElemCalculate elecal;
    double zcoord = elecal.LoopZCoordinate(loop);
    if(loops.empty()){
        res = loop;
        elecal.SetLoopElementZValue(zcoord,res);
        return;
    }
    PointInPolygon inpol;GeomIntersect goint;
    double endptPre = PreErr5_8,intPre = PreErr5_8,tanPre = PreErr5_8;
#define _GenDebug 0
#if _GenDebug
    Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
    Quantity_Color green(0,0.5,0,Quantity_TOC_RGB);
    Quantity_Color black(0,0,0,Quantity_TOC_RGB);
    string str = OperateOftString().LoopElementString(loop);
    DisplayGeom().DisplayLoop(loop,black,1,true);
    DisplayGeom().DisplayLoops(loops,black,1.5);
    mainwind->myOccView->fitAll();
    std::cout<<str<<endl;
#endif
    for (int e = 0; e < loop.ElemNum(); ++e) {
        bool isExiEndpt = false;
        const DefElem& ele = loop.m_vElem[e];
        vector<OftStr::Point>vInt;
        const OftStr::Point& sp = ele._segment._segSp;
        const OftStr::Point& ep = ele._segment._segEp;
#if _GenDebug
        DisplayGeom().DisplayElement(ele,blue,1.5);
#endif
        for (int i = 0; i < loops.size(); ++i) {
            vector<OftStr::Point>vInt_;
            for (int j = 0; j < loops[i].ElemNum(); ++j) {
                GeomIntRes intRes;                bool isEndpt = false;
                const DefElem& ele_ = loops[i].m_vElem[j];
                goint.CalculateCurveIntersect(ele,ele_,false,tanPre,
                                              endptPre,intPre,intRes);
                if(!intRes._isExistInt)
                    continue;
                if(elecal.IsElementEndpoint(ele_,intRes._intPnt1,endptPre)){
                    isEndpt = true;
                    isExiEndpt = true;
                }
                if(!isEndpt && intRes._intNum > 1 &&
                        elecal.IsElementEndpoint(ele_,intRes._intPnt2,endptPre))
{
                    isEndpt = true;
                    isExiEndpt = true;
                }
                vInt_.emplace_back(intRes._intPnt1);
                if(intRes._intNum > 1){
                    vInt_.emplace_back(intRes._intPnt2);
                }
            }
            if(vInt_.size() > 1){
                for(auto& d : vInt_){
                    vInt.emplace_back(d);
                }
            }
        }
        if(vInt.size() < 2){
            res.PushBackElem(ele);
#if _GenDebug
            DisplayGeom().DisplayElement(ele,green,1.5);
#endif
            continue;
        }
        std::sort(vInt.begin(), vInt.end(),
                  [&sp](const OftStr::Point& a,
                  const OftStr::Point& b) {
            return a.DistSquare2D(sp) < b.DistSquare2D(sp);
        });
#if _GenDebug
        for(auto& d : vInt){
            DisplayGeom().DisplayPoint(d);
        }
#endif
        for (int i = 0; i < vInt.size(); ++i) {
            if(i == 0){
                res.PushBackElem(DefElem(LINETYPE,DefSeg(
                                             sp,vInt[i])));
            }else if(i == vInt.size()-1){
                res.PushBackElem(DefElem(LINETYPE,DefSeg(
                                             vInt[i],ep)));
            }else{
                if(!isExiEndpt){
                    res.PushBackElem(DefElem(LINETYPE,DefSeg(
                                                 vInt[i],
                                                 vInt[i+1])));
                    i ++;
                }else{
                    bool isOnBorder = false,isInPoly = false;
                    Point mp = vInt[i].Midpoint(vInt[i+1]);
                    for(const auto& l : loops){
                        inpol.IsPointInsideLoop(l,mp,PreErr5_10,
                                                isOnBorder,isInPoly);
                        if(isInPoly)                            break;
                    }
                    if(isInPoly)
                        continue;
                    res.PushBackElem(DefElem(LINETYPE,DefSeg(
                                                 vInt[i],vInt[i+1])));
                }
            }
#if _GenDebug
            DisplayGeom().DisplayElement(res.m_vElem.back(),green,1.5);
#endif
        }
    }
    elecal.SetLoopElementZValue(zcoord,res);
#if _GenDebug
    DisplayGeom().DisplayLoop(loop,black,1,true);
    DisplayGeom().DisplayLoops(loops,black,1.5);
    DisplayGeom().DisplayLoop(res,green,1.5,true);
    string str1 = OperateOftString().LoopElementString(res);
    std::cout<<str1<<endl;
    int tem = 0;
#endif
}
void LineCutLogic::AreaLineIntersectPoint(const GeomArea& area,
                                          double dist, double agl,
                                          vector<IntPnt>& intPts)
{
    vector<DefLoop>loops{area.BndLoop()};
    for (size_t i = 0; i < area.IslLoop().size(); ++i) {
        loops.emplace_back(area._vIslLoop[i]);
    }
    GeomTangency gotan;
    GeomIntersect goint;LineCutSolve linsol;
    EquidistantCurve equ;ElemCalculate elecal;
    double endPre = PreErr_8,intPre = PreErr_8,tanPre = PreErr_8;
#if 0
    DisplayGeom play;
#endif
#if 0
    play.DisplayGeomArea(area,colors_.back(),1,true);
    mainwind->myOccView->fitAll();
#endif
    OftStr::Point lsp,lep;
    linsol.LineCutStartLine(loops[0],dist,agl,lsp,lep);
#if 0
    play.DisplayElement(DefElem(LINETYPE,DefSeg(lsp,lep)),
                        colors_[0],1.5);
#endif
    int num = 0,lId = 0;
    double oftDist = dist;
    while (true) {
        OftStr::Point sp,ep;
        equ.EquidistantSegment(lsp,lep,oftDist,
                               OUTSIDEPROFILE,sp,ep);
        DefElem elem(LINETYPE,DefSeg(sp,ep));
#if 0
        play.DisplayElement(elem,colors_[1],1.5);
#endif
        vector<IntPnt>lintps;
        bool isIntersect = false;
        for (int l = 0; l < loops.size(); ++l) {            vector<IntPnt>lointps;
            bool isExiEnpt = false;
            const DefLoop& loop = loops[l];
            for (int i = 0; i < loop.ElemNum(); ++i) {
                GeomIntRes intRes;
                const DefElem& elem_ = loop.IndexElem(i);
                goint.CalculateCurveIntersect(elem,elem_,false,tanPre,
                                              endPre,intPre,intRes);
                if(!intRes._isExistInt ||
                        (elem_.ElemeType() == ARCTYPE && intRes._intNum == 1 &&
                         gotan.IsLineCircleTangency(sp,ep,elem_._arc._arcCp,
                                                    elem_._arc._arcR,tanPre)))
                    continue;
                isIntersect = true;
                OftStr::Point cnt1;
                elecal.LoopPointBaseCurve(loop,intRes.P1(),i,
                                          true,PreErr5_2,cnt1);
                intRes._intPnt1.SetCnt(cnt1);
                if(elecal.IsElementEndpoint(elem_,intRes.P1(),endPre)){
                    isExiEnpt = true;
                    IntPnt intp = IntPnt(loop._id,i,lId,true,intRes.P1());
                    lointps.emplace_back(intp);
                }else{
                    IntPnt intp = IntPnt(loop._id,i,lId,false,intRes.P1());
                    lointps.emplace_back(intp);
                }
                if(intRes._intNum < 2)
                    continue;
                OftStr::Point cnt2;
                elecal.LoopPointBaseCurve(loop,intRes.P2(),i,
                                          true,PreErr5_2,cnt2);
                intRes._intPnt2.SetCnt(cnt2);
                if(elecal.IsElementEndpoint(elem_,intRes.P2(),endPre)){
                    isExiEnpt = true;
                    IntPnt intp = IntPnt(loop._id,i,lId,true,intRes.P2());
                    lointps.emplace_back(intp);
                }else{
                    IntPnt intp = IntPnt(loop._id,i,lId,false,intRes.P2());
                    lointps.emplace_back(intp);
                }
#if 0
                play.DisplayPoint(intRes.P1(),colors_[1]);
                play.DisplayPoint(intRes.P2(),colors_[1]);
#endif
            }
            if(isExiEnpt){
                linsol.ResetLineIntPoint(loop,sp,ep,agl,endPre,lointps);
            }
            for(auto& d : lointps){
                lintps.emplace_back(d);
            }
        }
        if(!isIntersect && num > 1)
            break;
        if(lintps.size() > 1){
            linsol.DeleteShortPoints(lintps,endPre);
        }
        if(lintps.size() % 2 != 0){
#if 0
            for(auto& d : lintps){
                play.DisplayPoint(d.P(),colors_[2]);
            }
#endif
            oftDist += dist;            num ++;
            continue;
        }
        if(isIntersect){lId ++;}
#if 0
        for (int j = 0; j < lintps.size(); ++j) {
            play.DisplayPoint(lintps[j].P(),colors_[2],true,j);
        }
#endif
        for(auto& d : lintps){
            intPts.emplace_back(d);
        }
        oftDist += dist;
        num ++;
    }
#if 0
    mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,colors_.back(),1);
    ///mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayPoint(intPts[i].P(),colors_[1],true,i);
    }
    int tem = 1;
#endif
}
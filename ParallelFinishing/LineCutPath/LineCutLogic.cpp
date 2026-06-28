#include "LineCutLogic.h"
#include "LineCutSolve.h"
#include "../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
#include "../../OperateView/DisplayGeom.h"
//#include "ToolTrajectoryAlg/GeometryCalculate/GeomIntersect/CurveInt2D.h"
#include"../../offset2D/ToolTrajectoryAlg/GeometryCalculate/GeomIntersect/CurveInt2D.h"
using namespace GeomUnit;
using namespace GridArea;
using namespace GeomStruct;
using namespace OffsetStruct;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
// void LineCutLogic::LoopToToolLocationLoop(const vector<vector<Rectangle>>& vRecs,
//                                           const DefLoop& loop,const double l,
//                                           DefLoop& resloop)
// {
//     LineCutSolve linsol;
//     for (int i = 0; i < loop.m_vElem.size(); ++i) {
//         ofts::Point intPt1,intPt2;
//         const ofts::DefSeg& seg = loop.m_vElem[i]._segment;
//         const ofts::Point& sp = seg._segSp,ep = seg._segEp;
//         const ofts::Point& sn = *sp._lable._normal,en = *ep._lable._normal;
//         if(!linsol.IsVertexNormalRecsInt(sp,sn,vRecs,l,intPt1) ||
//                 !linsol.IsVertexNormalRecsInt(ep,en,vRecs,l,intPt2))
//             continue;
//         resloop.m_vElem.push_back(DefElem(LINETYPE,DefSeg(intPt1,intPt2)));
//     }
// }
// void LineCutLogic::GenerateLoopLinePath(DefLoop loop,double dist,
//                                         double agl,DefLoop& res)
// {
//     if(loop.IsEmpty())
//         return;
// #define _GenDebug 0
//     GeomIntersect goint;LineCutSolve linsol;
//     EquidistantCurve equ;ElemCalculate elecal;
//     double endptPre = PreErr5_8,intPre = PreErr5_8,tanPre = PreErr5_8;
// #if _GenDebug
//     Quantity_Color yellow(0.7,0.7,0,Quantity_TOC_RGB);
//     Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
//     Quantity_Color green(0,0.25,0,Quantity_TOC_RGB);
//     Quantity_Color black(0,0,0,Quantity_TOC_RGB);
//     Quantity_Color orange(0.9,0.1,0,Quantity_TOC_RGB);
//     string str = OperateOftString().LoopElementString(loop);
//     DisplayGeom().DisplayLoop(loop,black,1,true);
//     _mainwind->myOccView->fitAll();
//     std::cout<<str<<endl;
// #endif
//     ofts::Point ext1,ext2;
//     linsol.LineCutStartLine(loop,dist,agl,ext1,ext2);
// #if _GenDebug
//     DisplayGeom().DisplayPoint(p0);
//     ofts::Point ext1_(p0.X() - dx * l, p0.Y() - dy * l);
//     ofts::Point ext2_(p0.X() + dx * l, p0.Y() + dy * l);
//     DisplayGeom().DisplayElement(
//                 DefElem(LINETYPE,DefSeg(ext1_,ext2_)),yellow,1);
//     DisplayGeom().DisplayElement(
//                 DefElem(LINETYPE,DefSeg(ext1,ext2)),blue,1.5);
// #endif
//     int num = 0;
//     PointInPolygon inpol;
//     double oftDist = dist;
//     while (true) {
//         bool isExiEndpt = false;
//         ofts::Point p1_,p2_;
//         vector<std::pair<ofts::Point,bool>>vInt;
//         equ.EquidistantSegment(ext1,ext2,oftDist,
//                                OUTSIDEPROFILE,p1_,p2_);
//         DefElem elem(LINETYPE,DefSeg(p1_,p2_));
// #if _GenDebug
//         DisplayGeom().DisplayElement(elem,blue,1.5);
// #endif
//         for (int i = 0; i < loop.Size(); ++i) {
//             GeomIntRes iRes;
//             bool isEndpt = false;
//             goint.CalculateCurveIntersect(elem,loop.m_vElem[i],
//                                           false,tanPre,endptPre,
//                                           intPre,iRes);
//             if(!iRes._isExistInt)
//                 continue;
//             if(elecal.IsElementEndpoint(loop.m_vElem[i],
//                                         iRes._intPnt1,PreErr5_6)){
//                 isEndpt = true;
//                 isExiEndpt = true;
//             }
//             if(!isEndpt && iRes._intNum > 1 &&
//                     elecal.IsElementEndpoint(loop.m_vElem[i],
//                                              iRes._intPnt2,PreErr5_6)){
//                 isExiEndpt = true;
//                 isEndpt = true;
//             }
//             vInt.emplace_back(std::make_pair(iRes._intPnt1,isEndpt));
//             if(iRes._intNum > 1){
//                 vInt.emplace_back(std::make_pair(iRes._intPnt2,isEndpt));
//             }
// #if _GenDebug
//             DisplayGeom().DisplayPoint(iRes._intPnt1);
//             DisplayGeom().DisplayPoint(iRes._intPnt2);
// #endif
//         }
//         if(vInt.size() < 2 && num > 1)
//             break;
//         if(!isExiEndpt && !vInt.empty()){
//             for (int i = 0; i < vInt.size()-1; ++i) {
//                 DefElem ele(LINETYPE,DefSeg(vInt[i].first,
//                                             vInt[i+1].first));
//                 res.PushBackElem(ele);
//                 i ++;
// #if _GenDebug
//                 DisplayGeom().DisplayElement(ele,green,1.5);
// #endif
//             }
//         }
//         else if(!vInt.empty()){
//             for (int i = 0; i < vInt.size()-1; ++i) {
//                 bool isOnBorder = false,isInPoly = false;
//                 Point mp = vInt[i].first.Midpoint(vInt[i+1].first);
//                 inpol.IsPointInsideLoop(loop,mp,PreErr5_10,
//                                         isOnBorder,isInPoly);
//                 if(isInPoly){
//                     DefElem ele(LINETYPE,DefSeg(vInt[i].first,
//                                                 vInt[i+1].first));
//                     res.PushBackElem(ele);
// #if _GenDebug
//                     DisplayGeom().DisplayElement(ele,green,1.5);
// #endif
//                 }
//             }
//         }
//         oftDist += dist;
//         num ++;
//     }
//     for(auto& e : res.m_vElem){
//         if(e._segment._segSp.X() > e._segment._segEp.X()){
//             ofts::Point p = e._segment._segSp;
//             e._segment._segSp = e._segment._segEp;
//             e._segment._segEp = p;
//         }
//     }
// #if _GenDebug
//     string str1 = OperateOftString().LoopElementString(res);
//     DisplayGeom().DisplayLoop(res,orange,1);
//     std::cout<<str1<<endl;
//     int tem = 0;
// #endif
// }
// void LineCutLogic::LoopsClipLinePath(const vector<DefLoop>& loops,
//                                      const DefLoop& loop,DefLoop& res)
// {
//     ElemCalculate elecal;
//     double zcoord = elecal.LoopZCoordinate(loop);
//     if(loops.empty()){
//         res = loop;
//         elecal.SetLoopElementZValue(zcoord,res);
//         return;
//     }
//     PointInPolygon inpol;GeomIntersect goint;
//     double endptPre = PreErr5_8,intPre = PreErr5_8,tanPre = PreErr5_8;
// #define _GenDebug 0
// #if _GenDebug
//     Quantity_Color blue(0,0,0.7,Quantity_TOC_RGB);
//     Quantity_Color green(0,0.5,0,Quantity_TOC_RGB);
//     Quantity_Color black(0,0,0,Quantity_TOC_RGB);
//     string str = OperateOftString().LoopElementString(loop);
//     DisplayGeom().DisplayLoop(loop,black,1,true);
//     DisplayGeom().DisplayLoops(loops,black,1.5);
//     _mainwind->myOccView->fitAll();
//     std::cout<<str<<endl;
// #endif
//     for (int e = 0; e < loop.Size(); ++e) {
//         bool isExiEndpt = false;
//         const DefElem& ele = loop.m_vElem[e];
//         vector<ofts::Point>vInt;
//         const ofts::Point& sp = ele._segment._segSp;
//         const ofts::Point& ep = ele._segment._segEp;
// #if _GenDebug
//         DisplayGeom().DisplayElement(ele,blue,1.5);
// #endif
//         for (int i = 0; i < loops.size(); ++i) {
//             vector<ofts::Point>vInt_;
//             for (int j = 0; j < loops[i].Size(); ++j) {
//                 GeomIntRes iRes;
//                 bool isEndpt = false;
//                 const DefElem& ele_ = loops[i].m_vElem[j];
//                 goint.CalculateCurveIntersect(ele,ele_,false,tanPre,
//                                               endptPre,intPre,iRes);
//                 if(!iRes._isExistInt)
//                     continue;
//                 if(elecal.IsElementEndpoint(ele_,iRes._intPnt1,endptPre)){
//                     isEndpt = true;
//                     isExiEndpt = true;
//                 }
//                 if(!isEndpt && iRes._intNum > 1 &&
//                         elecal.IsElementEndpoint(ele_,iRes._intPnt2,endptPre)){
//                     isEndpt = true;
//                     isExiEndpt = true;
//                 }
//                 vInt_.emplace_back(iRes._intPnt1);
//                 if(iRes._intNum > 1){
//                     vInt_.emplace_back(iRes._intPnt2);
//                 }
//             }
//             if(vInt_.size() > 1){
//                 for(auto& d : vInt_){
//                     vInt.emplace_back(d);
//                 }
//             }
//         }
//         if(vInt.size() < 2){
//             res.PushBackElem(ele);
// #if _GenDebug
//             DisplayGeom().DisplayElement(ele,green,1.5);
// #endif
//             continue;
//         }
//         std::sort(vInt.begin(), vInt.end(),
//                   [&sp](const ofts::Point& a,
//                   const ofts::Point& b) {
//             return a.DistSquare2D(sp) < b.DistSquare2D(sp);
//         });
// #if _GenDebug
//         for(auto& d : vInt){
//             DisplayGeom().DisplayPoint(d);
//         }
// #endif
//         for (int i = 0; i < vInt.size(); ++i) {
//             if(i == 0){
//                 res.PushBackElem(DefElem(LINETYPE,DefSeg(
//                                              sp,vInt[i])));
//             }else if(i == vInt.size()-1){
//                 res.PushBackElem(DefElem(LINETYPE,DefSeg(
//                                              vInt[i],ep)));
//             }else{
//                 if(!isExiEndpt){
//                     res.PushBackElem(DefElem(LINETYPE,DefSeg(
//                                                  vInt[i],
//                                                  vInt[i+1])));
//                     i ++;
//                 }else{
//                     bool isOnBorder = false,isInPoly = false;
//                     Point mp = vInt[i].Midpoint(vInt[i+1]);
//                     for(const auto& l : loops){
//                         inpol.IsPointInsideLoop(l,mp,PreErr5_10,
//                                                 isOnBorder,isInPoly);
//                         if(isInPoly)
//                             break;
//                     }
//                     if(isInPoly)
//                         continue;
//                     res.PushBackElem(DefElem(LINETYPE,DefSeg(
//                                                  vInt[i],vInt[i+1])));
//                 }
//             }
// #if _GenDebug
//             DisplayGeom().DisplayElement(res.m_vElem.back(),green,1.5);
// #endif
//         }
//     }
//     elecal.SetLoopElementZValue(zcoord,res);
// #if _GenDebug
//     DisplayGeom().DisplayLoop(loop,black,1,true);
//     DisplayGeom().DisplayLoops(loops,black,1.5);
//     DisplayGeom().DisplayLoop(res,green,1.5,true);
//     string str1 = OperateOftString().LoopElementString(res);
//     std::cout<<str1<<endl;
//     int tem = 0;
// #endif
// }
void LineCutLogic::AreaLineIntersectPoint(const GeomArea& area,
                                          double dist, double agl,
                                          vector<IntPnt>& intPts)
{
    vector<DefLoop>loops{area.BndLoop()};
    for (size_t i = 0; i < area.IslLoop().size(); ++i) {
        loops.emplace_back(area._vIslLoop[i]);
    }
    LineCutSolve linsol;
    EquidistantCurve equ;ElemCalculate elecal;
    double endPre = PreErr_8;
    numst::IntParam ipa;
    ipa._lcTanPre = endPre;ipa._cal_CL_Tan = false;
    ipa._lParalXPre = PreErr_10;ipa._lParalYPre = PreErr_10;
#if 0
    DisplayGeom play;
#endif
#if 0
    play.DisplayGeomArea(area,1,1);
#endif
    ofts::Point lsp,lep;
    linsol.LineCutStartLine(loops[0],dist,agl,lsp,lep);
#if 0
    play.DisplayElement(DefElem(LINETYPE,DefSeg(lsp,lep)),
                        _colors[0],1.5);
#endif
    int num = 0,lId = 0;
    double oftDist = dist;
    while (true) {
        ofts::Point sp,ep;
        equ.EquidistantSegment(lsp,lep,oftDist,false,sp,ep);
        DefElem curve(LINETYPE,DefSeg(sp,ep));
#if 0
        play.DisplayElement(curve,_colors[1],1.5);
#endif
        vector<IntPnt>lintps;
        bool isIntersect = false;
        for (int l = 0; l < loops.size(); ++l) {
            vector<IntPnt>lointps;
            bool isExiEnpt = false;
            const auto& loop = loops[l];
            for (int i = 0; i < loop.Size(); ++i) {
                GeomIntRes iRes;
                const auto& curve_ = loop[i];
                CurveInt2D::GetCurveIntersect(curve,curve_,ipa,iRes);
                if(!iRes.IsInt()){continue;}
                ///play.DisplayElement(curve_,_colors[2],1.5);
                isIntersect = true;
                ofts::Point cnt1;
                elecal.LoopPointBaseCurve(loop,iRes.P1(),i,true,PreErr5_2,cnt1);
                iRes._intPnt1.SetCnt(cnt1);
                bool isEp1 = curve_.IsEndpoint(iRes.P1(),endPre);
                auto intp = IntPnt(loop._id,i,lId,isEp1,iRes.P1());
                lointps.emplace_back(intp);
                if(!isExiEnpt && isEp1){isExiEnpt = true;}
                if(iRes.Num() < 2){continue;}
                ofts::Point cnt2;
                elecal.LoopPointBaseCurve(loop,iRes.P2(),i,true,PreErr5_2,cnt2);
                iRes._intPnt2.SetCnt(cnt2);
                bool isEp2 = curve_.IsEndpoint(iRes.P2(),endPre);
                auto intp1 = IntPnt(loop._id,i,lId,isEp2,iRes.P2());
                lointps.emplace_back(intp1);
                if(!isExiEnpt && isEp2){isExiEnpt = true;}
#if 0
                play.DisplayPoint(iRes.P1(),_colors[1]);
                play.DisplayPoint(iRes.P2(),_colors[1]);
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
                play.DisplayPoint(d.P(),_colors[2]);
            }
#endif
            oftDist += dist;
            num ++;
            continue;
        }
        if(isIntersect){lId ++;}
#if 0
        for (int j = 0; j < lintps.size(); ++j) {
            play.DisplayPoint(lintps[j].P(),_colors[2],true,j);
        }
#endif
        for(auto& d : lintps){
            intPts.emplace_back(d);
        }
        oftDist += dist;
        num ++;
    }
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,1,1);
    ///_mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayPoint(intPts[i].P(),_colors[1],true,i);
    }
    int tem = 1;
#endif
}
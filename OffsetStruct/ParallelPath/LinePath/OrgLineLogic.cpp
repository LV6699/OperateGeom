#include "OrgLineLogic.h"
#include "OrgLineSolve.h"
#include "LineCutSolve.h"
#include "../../../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
using namespace OftStr;
using namespace GridArea;
using namespace GeomStruct;
using namespace GeomUnit;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void OrgLineLogic::SingleDirectionLoop(const GeomArea& area,double agl,
                                       vector<GeomUnit::IntPnt>& intPts,
                                       vector<OftStr::DefLoop>& vLoop)
{
    vector<IntLine>intLines;
    LineCutSolve().IntPointToCutLine(area,agl,intPts,intLines);
    for (int l = 0; l < intLines.size(); ++l) {
        const IntLine& intl = intLines[l];
        for (int i = 0; i < intl._vIntPt.size(); ++i) {
            DefElem ele = IntPtToElem(intl._vIntPt[i],
                                      intl._vIntPt[i]->cnt);
            vLoop.push_back(DefLoop(vector<DefElem>{ele}));
            i++;
        }
    }
}
/**
跳刀规则:
goto:
(1)当前点cur下一点(nex)已经被访问.(2)下一点不存在.
back:
(1)当前点的_lId与nex的_lId相同.(2)下一点不存在.
 */
void OrgLineLogic::LineCutPointToLoop(const DefLoop& loop_,
                                      vector<IntPnt>& intPts,
                                      vector<IntLine>& intls,
                                      IntPnt* cur,
                                      vector<DefLoop>& vLoop)
{
    OrgLineSolve sol;
#if 0
    DisplayGeom play;
#endif
#if 0
    double w = 1;
    play.DisplayLoop(loop_,_colors[8],1.5);
    mainwind->myOccView->fitAll();
#endif
    DefLoop loop;
    bool isNex = true;
    IntPnt* pcur = nullptr;
    IntPnt* judge = nullptr;
    while (true) {
        cur->_hasVisit = true;
        cur->cnt->_hasVisit = true;
        loop.PushBackElem(IntPtToElem(cur,cur->cnt));
#if 0
        w = cur->P().X() < cur->cnt->P().X() ? 1.5 : 1;        play.DisplayPoint(cur->P(),_colors[1],true,cur->_id);
        play.DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
        play.DisplaySegment(cur->P(),cur->cnt->P(),_colors[vLoop.size()],
                w,true,cur->_lineId);
#endif
#if 0
        if(cur->_lineId == 9){
            int tem = 1;
        }
#endif
        cur = cur->cnt;
        judge = isNex ? cur->nex : cur->pre;
        if(judge->_hasVisit || judge->_lineId == cur->_lineId){
            cur = nullptr;
            for (int i = 0; i < intls.size(); ++i) {
                for (int j = 0; j < intls[i]._vIntPt.size(); ++j) {
                    if(intls[i]._vIntPt[j]->_hasVisit)
                        continue;
                    cur = intls[i]._vIntPt[j];
                    isNex = true;
#if 0
                    play.DisplayPoint(cur->P(),_colors[1],true,cur->_id);
                    play.DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt-
>_id);
                    play.DisplaySegment(cur->P(),cur->cnt->P(),_colors[
                                        vLoop.size()],1.5,true,cur->_lineId);
#endif
                    break;
                }
                if(cur != nullptr)
                    break;
            }
            AddLoop(vLoop,loop);
            if(cur == nullptr)
                break;
            continue;
        }
        pcur = cur;
        vector<DefElem> eles;
        cur = isNex ? cur->nex : cur ->pre;
        sol.PointsBoundary(loop_,pcur,cur,isNex,eles);
        for(auto& d : eles){
            loop.PushBackElem(d);
#if 0
            play.DisplayElement(d,isNex ? _colors[4] : _colors[5],1.5);
#endif
        }
        isNex = !isNex;
    }
#if 0
    mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayLoop(loop_,_colors[8],1,true);
    mainwind->myOccView->fitAll();
    for (int i = 0; i < vLoop.size(); ++i) {
        play.DisplayLoop(vLoop[i],_colors[i],1.5);
    }
    int tem = 1;
#endif
}
void OrgLineLogic::LineCutPointToLoop(const GeomArea& area,
                                      vector<IntPnt>& intPts,                                      vector<IntLine>& intls,
                                      IntPnt* cur,
                                      vector<DefLoop>& vLoop)
{
    OrgLineSolve sol;
    const DefLoop& bndLoop = area._bndLoop;
    vector<DefLoop> loops{bndLoop},loops1{area._oriBndLoop};
    for(auto& d : area._vIslLoop){
        loops.emplace_back(d);
    }
    for(auto& d : area._vOriIslLoop){
        loops1.emplace_back(d);
    }
    ///plan.LineCutPointNormalPlan(loops1,cur);
    ///plan.LineCutPointNormalPlan(loops1,cur->cnt);
#if 0
    DisplayGeom play;
#endif
#if 0
    double w = 1;
    play.DisplayGeomArea(area,_colors[8],1.5);
    mainwind->myOccView->fitAll();
#endif
    DefLoop loop;
    IntPnt* pcur = nullptr;
    IntPnt* judge = nullptr;
    bool isNex = true,isBndStart = true;
    while (true) {
#if 0
        w = cur->P().X() < cur->cnt->P().X() ? 1.5 : 1;
        play.DisplayPoint(cur->P(),_colors[1],true,cur->_id);
        play.DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
        play.DisplaySegment(cur->P(),cur->cnt->P(),_colors[vLoop.size()],
                w,true,cur->_lineId);
#endif
        cur->_hasVisit = true;
        cur->cnt->_hasVisit = true;
        loop.PushBackElem(IntPtToElem(cur,cur->cnt));
#if 0
        if(cur->_lineId == 9){
            int tem = 1;
        }
#endif
        pcur = cur;
        cur = cur->cnt;
        isNex = cur->nex->_lineId > cur->pre->_lineId ? true : false;
        judge = isNex ? cur->nex : cur->pre;
        if(judge->_hasVisit || judge->_lineId == cur->_lineId){
            ///plan.LineCutPointNormalPlan(loops1,cur);
            ///plan.LineCutPointNormalPlan(loops1,cur->cnt);
            ///loop.SetLastElem(IntPtToElem(cur,cur->cnt));
            cur = nullptr;
            isBndStart = false;
            for (int i = 0; i < intls.size(); ++i){
                for (int j = 0; j < intls[i]._vIntPt.size(); ++j){
                    if(intls[i]._vIntPt[j]->_lId < 0 &&
                            !intls[i]._vIntPt[j]->_hasVisit){
                        cur = intls[i]._vIntPt[j];
                        isBndStart = true;
                        break;
                    }
                }                if(isBndStart)
                    break;
            }
            if(!isBndStart){
                for (int i = 0; i < intls.size(); ++i) {
                    for (int j = 1; j < intls[i]._vIntPt.size()-1; ++j) {
                        if(intls[i]._vIntPt[j]->_hasVisit)
                            continue;
                        cur = intls[i]._vIntPt[j];
                        break;
                    }
                    if(cur != nullptr)
                        break;
                }
            }
            AddLoop(vLoop,loop);
            if(cur == nullptr)
                break;
            ///plan.LineCutPointNormalPlan(loops1,cur);
            ///plan.LineCutPointNormalPlan(loops1,cur->cnt);
#if 0
            play.DisplayPoint(cur->P(),_colors[1],true,cur->_id);
            play.DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
            play.DisplaySegment(cur->P(),cur->cnt->P(),_colors[
                                vLoop.size()],1.5,true,cur->_lineId);
#endif
            continue;
        }
#if 0
        pcur = cur;
        vector<DefElem>eles;
        cur = isNex ? cur->nex : cur ->pre;
        if(area._vIslLoop.empty()){
            sol.PointsBoundary(bndLoop,pcur,cur,isNex,eles);
        }else{
            for(const auto& d : loops){
                if(d._id == cur->_lId){
                    sol.PointsBoundary(d,pcur,cur,isNex,eles);
                    break;
                }
            }
        }
        for(auto& d : eles){
            loop.PushBackElem(d);
#if 1
            play.DisplayElement(d,isNex ? _colors[4] : _colors[5],1.5);
#endif
        }
#else
        cur = isNex ? cur->nex : cur->pre;
        AddLoop(vLoop,loop);
#endif
    }
    for(auto& d : vLoop){
        ElemCalculate().SetLoopElementZValue(0,d);
    }
#if 0
    mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,_colors[8],1);
    mainwind->myOccView->fitAll();
    for (int i = 0; i < vLoop.size(); ++i) {
        play.DisplayLoop(vLoop[i],_colors[i],1.5);
    }
    int tem = 1;#endif
}
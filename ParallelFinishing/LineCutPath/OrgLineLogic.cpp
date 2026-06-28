#include "OrgLineLogic.h"
#include "OrgLineSolve.h"
#include "LineCutSolve.h"
#include "../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
#include "../../OperateView/DisplayGeom.h"
using namespace ofts;
using namespace GridArea;
using namespace GeomStruct;
using namespace GeomUnit;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void OrgLineLogic::SingleDirectionLoop(const GeomArea& area,double agl,
                                       vector<GeomUnit::IntPnt>& intPts,
                                       vector<ofts::DefLoop>& vLoop)
{
    vector<IntLine>intLines;
    LineCutSolve().IntPointToCutLine(area,agl,intPts,intLines);
    for (size_t l = 0; l < intLines.size(); ++l) {
        const IntLine& intl = intLines[l];
        for (size_t i = 0; i < intl._vIntPt.size(); ++i) {
            DefElem ele = IntPtToElem(intl._vIntPt[i],
                                      intl._vIntPt[i]->cnt);
            vLoop.push_back(DefLoop(vector<DefElem>{ele}));
            i++;
        }
    }
}
/**
璺冲垁瑙勫垯:
goto:
(1)褰撳墠鐐筩ur 涓嬩竴鐐(nex)宸茬粡琚闂.(2)涓嬩竴鐐逛笉瀛樺湪.
back:
(1)褰撳墠鐐圭殑_lId 涓巒ex 鐨刜lId 鐩稿悓.(2)涓嬩竴鐐逛笉瀛樺湪.
*/
void OrgLineLogic::LineCutPointToLoop(const DefLoop& loop_,
                                      vector<IntPnt>& intPts,
                                      vector<IntLine>& intls,
                                      IntPnt* cur,
                                      vector<DefLoop>& vLoop)
{
    OrgLineSolve sol;
#if 0
    double w = 1;
    DisplayGeom().DisplayLoop(loop_,_colors[8],1.5);
    _mainwind->myOccView->fitAll();
#endif
    DefLoop loop;
    bool isNex = true;
    IntPnt* pcur = nullptr;
    IntPnt* judge = nullptr;
    while (true) {
        cur->_hasVisit = true;
        cur->cnt->_hasVisit = true;
        loop.AddElem(IntPtToElem(cur,cur->cnt));
#if 0
        w = cur->P().X() < cur->cnt->P().X() ? 1.5 : 1;
        DisplayGeom().DisplayPoint(cur->P(),_colors[1],true,cur->_id);
        DisplayGeom().DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
        DisplayGeom().DisplaySegment(cur->P(),cur->cnt->P(),_colors[vLoop.size()],
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
                    DisplayGeom().DisplayPoint(cur->P(),_colors[1],true,cur->_id);
                    DisplayGeom().DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
                    DisplayGeom().DisplaySegment(cur->P(),cur->cnt->P(),_colors[
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
            loop.AddElem(d);
#if 0
            DisplayGeom().DisplayElement(d,isNex ? _colors[4] :
                                         _colors[5],1.5);
#endif
        }
        isNex = !isNex;
    }
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    DisplayGeom().DisplayLoop(loop_,_colors[8],1,true);
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < vLoop.size(); ++i) {
        DisplayGeom().DisplayLoop(vLoop[i],_colors[i],1.5);
    }
    int tem = 1;
#endif
}
void OrgLineLogic::LineCutPointToLoop(const GeomArea& area,
                                      ofts::ParalParam param,
                                      vector<IntPnt>& intPts,
                                      vector<IntLine>& intls,
                                      IntPnt* cur,
                                      vector<DefLoop>& rloops)
{
    OrgLineSolve sol;
    vector<DefLoop>loops{area._bndLoop};
    for(auto& d : area._vIslLoop){
        loops.emplace_back(d);
    }
#if 0
    double w = 1;
    DisplayGeom().DisplayGeomArea(area,1,1);
#endif
    DefLoop loop;
    IntPnt* pcur = nullptr;
    IntPnt* judge = nullptr;
    bool isNex = true,isBndStart = true;
    while (true) {
#if 0
        double w = cur->P().X() < cur->cnt->P().X() ? 1.5 : 1;
        DisplayGeom().DisplayPoint(cur->P(),_colors[1],true,cur->_id);
        DisplayGeom().DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
        DisplayGeom().DisplaySegment(cur->P(),cur->cnt->P(),_colors[rloops.size()],1.5,true,cur->_lineId);
#endif
        cur->_hasVisit = true;
        cur->cnt->_hasVisit = true;
        loop.AddElem(IntPtToElem(cur,cur->cnt));/**
        if(cur->_lineId == 11){
            int tem = 1;
        }*/
        pcur = cur;
        cur = cur->cnt;
        isNex = cur->nex->_lineId > cur->pre->_lineId ? true : false;
        if(isNex && cur->nex->_hasVisit){
            isNex = false;
        }
        if(!isNex && cur->pre->_hasVisit){
            isNex = true;
        }
        judge = isNex ? cur->nex : cur->pre;
        if(judge->_hasVisit || judge->_lineId == cur->_lineId){
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
                }
                if(isBndStart)
                    break;
            }
            if(!isBndStart){
                for (int i = 0; i < intls.size(); ++i) {
                    for (int j = 1; j < intls[i]._vIntPt.size()-1; ++j) {
                        if(intls[i]._vIntPt[j]->_hasVisit){continue;}
                        cur = intls[i]._vIntPt[j];
                        break;
                    }
                    if(cur != nullptr)
                        break;
                }
            }
            AddLoop(rloops,loop);
            if(cur == nullptr)
                break;
#if 0
            DisplayGeom().DisplayPoint(cur->P(),_colors[1],true,cur->_id);
            DisplayGeom().DisplayPoint(cur->cnt->P(),_colors[0],true,cur->cnt->_id);
            DisplayGeom().DisplaySegment(cur->P(),cur->cnt->P(),_colors[
                                         rloops.size()],1,true,cur->_lineId);
#endif
            continue;
        }
        if(param.PType() != Ofte::AloneContour){
            cur = isNex ? cur->nex : cur->pre;
            AddLoop(rloops,loop);
            continue;
        }
        pcur = cur;
        cur = isNex ? cur->nex : cur->pre;
        vector<DefElem>eles;
        if(area._vIslLoop.empty()){
            sol.PointsBoundary(area._bndLoop,pcur,cur,isNex,eles);
        }else{
            for(const auto& d : loops){
                if(d._id != cur->_lId){continue;}/**
                    if(d._id == 0){
                        int tem = 1;
                    }*/
                sol.PointsBoundary(d,pcur,cur,isNex,eles);
                break;
            }
        }
        for(auto& d : eles){
            loop.AddElem(d);
            //DisplayGeom().DisplayElement(d,isNex ? _colors[4] : _colors[5],1.5);
        }
    }
    for(auto& d : rloops){
        d.SetEndptZ(0);
    }
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    DisplayGeom().DisplayLoop(area.BLoop(),_colors[8],1,true);
    for (size_t i = 0; i < area.ILoops().size(); i++){
      DisplayGeom().DisplayLoop(area._vIslLoop[i],_colors[8],1.5,true);
    }
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < rloops.size(); ++i) {
        DisplayGeom().DisplayLoop(rloops[i],_colors[i],1.5);
    }
    int tem = 1;
#endif
}
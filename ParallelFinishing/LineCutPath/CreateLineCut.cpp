#include <iostream>
///#include <tbb/tbb.h>
#include "CreateLineCut.h"
#include "LineCutLogic.h"
#include "OrgLineLogic.h"
#include "LineCutSolve.h"
#include "../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
#include "../../offset2D/ToolTrajectoryAlg/OperCurve2D/OperCurve2D/OperateLoop.h"
#include "../../OperateView/DisplayGeom.h"
using std::string;
using std::vector;
using namespace GridArea;
using namespace GeomStruct;
using namespace OffsetStruct;
using namespace GeomUnitStruct;
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void CreateLineCut::BoundaryLineCutPath(GeomArea& area,
                                        ofts::ParalParam param,
                                        vector<DefLoop>& loops)
{
#if 0
    DisplayGeom play;
    _mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,_colors.back(),1.5);
    _mainwind->myOccView->myView->SetProj(V3d_Zpos);
    string str = OperateOftString().BndIslLoopString(area);
    std::cout<<str<<endl;
#endif
    vector<IntPnt>intPts;
    vector<IntLine>intLines;
    LineCutLogic cutlg;LineCutSolve linsol;OrgLineLogic orglg;
    linsol.ResetAngle(param._agl);
    cutlg.AreaLineIntersectPoint(area,param.Dist(),param.Agl(),intPts);
#if 0
    play.DisplayLoop(loop,_colors.back(),1,true);
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayPoint(intPts[i].P());
    }
#endif
    if(param.PType() == Ofte::SingleDirection){
        orglg.SingleDirectionLoop(area,param.Agl(),intPts,loops);
        return;
    }
    if(!intPts.empty()){
        linsol.OrgnizeIntPointBaseElem(area.BndLoop(),intPts);
    }
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayLoop(loop,_colors.back(),1,true);
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayLoop(loop,_colors.back(),1,true);
        play.DisplayPoint(intPts[i].P(),true,i);
        play.DisplayPoint(intPts[i].pre->P());
        play.DisplayPoint(intPts[i].nex->P());
        _mainwind->myOccView->getContext()->RemoveAll(true);
    }
#endif
    linsol.IntPointToCutLine(area,param.Agl(),intPts,intLines);
#if 0
    double w = 1;
    for (int i = 0; i < intLines.size(); ++i) {
        for (int j = 0; j < intLines[i]._vIntPt.size(); ++j) {
            w = intLines[i]._vIntPt[j]->P().X() <
                    intLines[i]._vIntPt[j]->cnt->P().X() ? 1.5 : 1;
            play.DisplayPoint(intLines[i]._vIntPt[j]->P(),
                              true,intLines[i]._vIntPt[j]->_id);
            play.DisplayPoint(intLines[i]._vIntPt[j]->cnt->P(),
                              true,intLines[i]._vIntPt[j]->cnt->_id);
            play.DisplaySegment(intLines[i]._vIntPt[j]->P(),
                                intLines[i]._vIntPt[j]->cnt->P(),
                                green,w,true,intLines[i]._id);
        }
    }
    int tem = 1;
#endif
    if(!intLines.empty()){
        orglg.LineCutPointToLoop(area,param,intPts,intLines,
                                 intLines[0]._vIntPt[0],loops);
    }
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    DisplayGeom().DisplayLoop(area.BndLoop(),_colors.back(),1,true);
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < loops.size(); ++i) {
        play.DisplayLoop(loops[i],_colors[i],1.5);
    }
    int tem = 1;
#endif
}
void CreateLineCut::GeomAreaLineCutPath(GeomArea& area,
                                        ofts::ParalParam param,
                                        vector<DefLoop>& loops)
{
#if 1
    DisplayGeom play;
    string str = OperateOftString().BndIslLoopString(area);
    std::cout<<str<<endl;
#endif
    for(int i = 0;i < area._vIslLoop.size();i++){
        OperateLoop().CircleArcLoopTrans(true,area._vIslLoop[i]);
        area._vIslLoop[i].SetId(i);
    }
    vector<IntPnt>intPts;
    vector<IntLine>intLines;
    LineCutLogic cutlg;LineCutSolve linsol;OrgLineLogic orglg;
    linsol.ResetAngle(param._agl);
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,_colors.back(),1.5);
    _mainwind->myOccView->myView->SetProj(V3d_Zpos);
    _mainwind->myOccView->fitAll();
#endif
    cutlg.AreaLineIntersectPoint(area,param.Dist(),param.Agl(),intPts);
#if 0
    for (int i = 0; i < intPts.size(); ++i) {
        play.DisplayPoint(intPts[i].P(),_colors[1]);
    }
#endif
    if(param.PType() == Ofte::SingleDirection){
        orglg.SingleDirectionLoop(area,param.Agl(),intPts,loops);
        return;
    }
    linsol.OrgnizeIntPointBaseLoop(area,intPts);
#if 0
    _mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,_colors.back(),1.5);
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < intPts.size(); ++i) {
        const IntPnt& pt = intPts[i];
        play.DisplayPoint(pt.P(),_colors[0],true,i);
        play.DisplayPoint(pt.pre->P(),_colors[1]);
        play.DisplayPoint(pt.nex->P(),_colors[2]);
        int tem = 1;
    }
#endif
    linsol.IntPointToCutLine(area,param.Agl(),intPts,intLines);
#if 0
    //_mainwind->myOccView->getContext()->RemoveAll(true);
    //play.DisplayGeomArea(area,_colors.back(),1.5);
    //_mainwind->myOccView->fitAll();
    for (int i = 0; i < intLines.size(); ++i) {
        std::vector<IntPnt*>& line = intLines[i]._vIntPt;
        for (int j = 0; j < line.size(); ++j) {
            ofts::Point sp = line[j]->P(),ep = line[j]->cnt->P();
            play.DisplayPoint(sp,_colors[1],true,line[j]->_id);
            play.DisplayPoint(ep,_colors[1],true,line[j]->cnt->_id);
            play.DisplaySegment(sp,ep,_colors[i],1.5,true,intLines[i]._id);
            j ++;
        }
    }
#endif
    if(!intLines.empty()){
        orglg.LineCutPointToLoop(area,param,intPts,intLines,
                                 intLines[0]._vIntPt[0],loops);
    }
#if 0
    ///_mainwind->myOccView->getContext()->RemoveAll(true);
    play.DisplayGeomArea(area,_colors.back(),1.5);
    _mainwind->myOccView->fitAll();
    for (int i = 0; i < loops.size(); ++i) {
        play.DisplayLoop(loops[i],_colors[i],1.5);
    }
    int tem = 1;
#endif
}
void CreateLineCut::CreateLineCutPath(const GeomArea& area,
                                      ofts::ParalParam param,
                                      vector<DefLoop>& loops)
{
    if(area._bndLoop.IsEmpty()){return;}
    GeometryLogic golgl;
    GeomArea area_ = area;
    if(param.Dist() < PreErr_2){return;}
    loops.clear();
  
    area_._bndLoop.SetEndptZ(0);
    golgl.CircleArcLoopTransform(true,area_._bndLoop);
    for(auto& d : area_._vIslLoop){
        d.SetEndptZ(0);
        golgl.CircleArcLoopTransform(true,d);
    }
    if(area._vIslLoop.empty()){
        BoundaryLineCutPath(area_,param,loops);
    }else{
        for (int i = 0; i < area_.IslLoop().size(); ++i) {
            area_._vIslLoop[i]._id = i;
        }
        for (int i = 0; i < area_.OriIslLoop().size(); ++i) {
            area_._vOriIslLoop[i]._id = i;
        }
        GeomAreaLineCutPath(area_,param,loops);
    }
#if 0
    for (int i = 0; i < loops.size(); ++i) {
        play.DisplayLoop(loops[i],_colors[i],1.5);
    }
    int tem = 1;
#endif
}
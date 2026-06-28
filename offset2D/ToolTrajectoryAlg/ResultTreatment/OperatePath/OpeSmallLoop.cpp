#include "OpeSmallLoop.h"
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
void OpeSmallLoop::ExpandPathSmallLoop(double R,std::shared_ptr<OffsetNode>& pathNode)
{
    if(pathNode == nullptr)
        return;
    if(!pathNode->_loops.empty()){
        ElemCalculate elecal;
        ExicuteProgram exipro;RootNodeSolve nodsol;
        DefCircle circle;
        DefLoop loop = pathNode->_loops.back();
        loop.SetLeftward(true);
        elecal.LoopMinCircumcircle(loop,true,5,circle);
#if _ViewDebug
        DisplayGeom().DisplayLoop(loop,_colors.back(),1.5);
        DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,circle),_colors[1],1);
        _mainwind->myOccView->fitAll();
#endif
        if(circle._cirR + PreErr_3 < R && circle._cirR > PreErr_3){
            if(!loop.IsEmpty()){
                OffsetParam oftPara;
                double dist = R - circle._cirR;
                if (dist > R / 2) {
                    dist = R / 2;
                }
                OffsetUnit oftUnit(true,1,R,dist,dist,false,PreErr5_2);
                oftPara._offsetUnit = oftUnit;
                vector<DefLoop>loops;
                shared_ptr<OffsetNode> node = std::make_shared<OffsetNode>();
                exipro.LoopOffsetNodeResult(loop,oftPara,node);
                nodsol.PathRootNodeLoop(node,loops);
                if(!loops.empty()){
                    pathNode->_loops.insert(pathNode->_loops.end() - 1,loops[0]);
#if _ViewDebug
                    DefCircle c;
                    elecal.LoopMinCircumcircle(loops[0],c);
                    DisplayGeom().DisplayLoop(loops[0],_colors[0],1.5);
                    DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,c),_colors[1],1);
                    _mainwind->myOccView->fitAll();
#endif
                }
            }
        }
    }
    for(auto& d : pathNode->_nodes){
        ExpandPathSmallLoop(R,d);
    }
}

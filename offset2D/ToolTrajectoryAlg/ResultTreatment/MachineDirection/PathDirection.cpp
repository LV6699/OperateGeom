#include "PathDirection.h"
void PathDirection::SpecifyPathMillDirection(OffsetParam offsetPara,
                                             shared_ptr<OffsetNode>&rootNode)
{
#if DebugModel
    return;
#endif
    bool isToolClock = offsetPara._millDirection.IsToolClock(),
            isProfClock = offsetPara._millDirection.IsProfClockMill(),
            isAreaClock = offsetPara._millDirection.IsAreaClockMill();
    SetToolPathDirection(isToolClock,true,isProfClock,rootNode);
    SetToolPathDirection(isToolClock,false,isAreaClock,rootNode);
}
void PathDirection::SetToolPathDirection(bool isToolClock,bool isProfile,
                                         bool isClockMill,
                                         shared_ptr<OffsetNode>&rootNode)
{
    ElemCalculate numcal;
    for (int lo = 0; lo < rootNode->_loops.size(); ++lo) {
        ToolPathType pathType = rootNode->_geneType;
        if((isProfile && rootNode->_loops[lo].PastOftNum() > 1) ||
                (!isProfile && rootNode->_loops[lo].PastOftNum() <= 1)){
            continue;
        }
        if(!IsChangePathDirection(pathType,isToolClock,isClockMill)){
            continue;
        }
        numcal.ReverseLoopDirection(rootNode->_loops[lo]);
        rootNode->_loops[lo]._label._isCW = false;
    }
    for (int n = 0; n < rootNode->_nodes.size(); ++n) {
        SetToolPathDirection(isToolClock,isProfile,isClockMill,
                             rootNode->_nodes[n]);
    }
}
bool PathDirection::IsChangePathDirection(ToolPathType pathType,
                                          bool isToolClock,
                                          bool isClockMill)
{
    if(isToolClock){
        if(isClockMill){
            return (pathType == BndOffsetPath) ||
                    (pathType == BndOftLocalPath) ||
                    (pathType == BndIslMerPath) ||
                    (pathType == IslIslMerPath) ||
                    (pathType == IslOftLocalPath);
        }
        else{
            return pathType == IslOffsetPath;
        }
    }
    if(isClockMill){
        return pathType == IslOffsetPath;
    }
    else {
        return (pathType == BndOffsetPath) ||
                (pathType == BndOftLocalPath) ||
                (pathType == BndIslMerPath) ||
                (pathType == IslIslMerPath) ||
                (pathType == IslOftLocalPath);
    }
}

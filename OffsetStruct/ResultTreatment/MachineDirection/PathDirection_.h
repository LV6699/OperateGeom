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
    for (int lo = 0; lo < rootNode->_vLoopData.size(); ++lo) {
        ToolPathType pathType = rootNode->_pathType;
        if((isProfile && rootNode->_vLoopData[lo].PastOffsetNum() > 1) ||
                (!isProfile && rootNode->_vLoopData[lo].PastOffsetNum() <= 1)){
            continue;
        }
        if(!IsChangePathDirection(pathType,isToolClock,isClockMill)){
            continue;
        }
        numcal.ReverseLoopDirection(rootNode->_vLoopData[lo]);
        rootNode->_vLoopData[lo]._loopLabel._loRotDir = ANTICLOCKWISE;
    }
    for (int n = 0; n < rootNode->_vOffsetNode.size(); ++n) {
        SetToolPathDirection(isToolClock,isProfile,isClockMill,
                             rootNode->_vOffsetNode[n]);
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
    }}
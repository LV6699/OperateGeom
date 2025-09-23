#include "SpecifyCutEntry.h"
#include "CutEntrySolve.h"
void SpecifyCutEntry::RedesignPathBaseOnEntry(shared_ptr<OffsetNode>& rootNode,
                                              bool isToOutside)
{
    if(rootNode == nullptr){
        return;
    }
    SpecifyCutEntryPoint(rootNode->_vLoopData,isToOutside);
    for (int i = 0; i < rootNode->_vOffsetNode.size(); ++i) {
        RedesignPathBaseOnEntry(rootNode->_vOffsetNode[i],isToOutside);
    }
}
void SpecifyCutEntry::SpecifyCutEntryPoint(vector<DefLoop>& vLoop,
                                           bool isToOutside)
{
#define sym 0
#if sym == 0
    ///ClosetPointSplitNextLoop(isToOutside,vLoop);
    CutEntryPointSplitLoop(isToOutside,vLoop);
#elif sym == 1
    if(isToOutside){
        FromInsideToOutsideMaching(vLoop);
    }
    else{
        FromOutsideToInsideMaching(vLoop);
    }
#else
    ClosetPointSplitNextLoop(isToOutside,vLoop);
#endif
}
///需要保证vLoop中的环是从外向内.
void SpecifyCutEntry::CutEntryPointSplitLoop(const bool isToOutside,
                                             vector<DefLoop>&vLoop)
{
    if(vLoop.empty()){
        return;
    }
    ///使vLoop从内向外排序.
    std::reverse(vLoop.begin(),vLoop.end());
    double zCoord;
    DefLoop firLoop = vLoop[0];
    zCoord = ElemCalculate().LoopZCoordinate(firLoop);
    ///重置最里面的环元素
    CutEntrySolve entsol;
    entsol.ReorederSplitElement(firLoop);
    vLoop[0] = firLoop;
    if(vLoop.size() == 1){
        return;
    }
    ElemCalculate numcal;
    Point preLoSp = numcal.LoopEndpoint(firLoop,false);
    for (int i = 0; i < vLoop.size() - 1; ++i) {
        Point closePt;
        int nex = numcal.NextIndex(vLoop.size(),i),index;
        DefLoop nexLoop = vLoop[nex];
        entsol.PointToLoopClosestPoint(preLoSp,nexLoop,index,closePt);
        closePt.SetZ(zCoord);
        entsol.PointSplitReorederLoop(closePt,index,vLoop[nex]);        preLoSp = closePt;
    }
    if(!isToOutside){
        std::reverse(vLoop.begin(),vLoop.end());
    }
}
///从内向外加工,需要保证vLoop中的环是从外向内.
void SpecifyCutEntry::FromInsideToOutsideMaching(vector<DefLoop>&vLoop)
{
    if(vLoop.empty()){
        return;
    }
    CutEntrySolve entsol;
    DefLoop firLoop = vLoop[0],lasLoop = vLoop.back();
    ///重置最里面的环元素
    entsol.ReorederSplitElement(lasLoop);
    vLoop[vLoop.size() - 1] = lasLoop;
    if(vLoop.size() == 1){
        return;
    }
    int index;
    Point startPt,cloPt;
    ElemCalculate numcal;
    numcal.ElementEndpoint(lasLoop.m_vElem[0],false,startPt);
    ///获取startPt至firLoop最近的点
    entsol.PointToLoopClosestPoint(startPt,firLoop,index,cloPt);
    DefElem ele = firLoop.m_vElem[index],preEle,nexEle;
    ///重置最外面的环元素
    numcal.ReorederLoopElement(index,true,false,firLoop);
    numcal.PointSplitElement(ele,cloPt,true,preEle);
    numcal.PointSplitElement(ele,cloPt,false,nexEle);
    firLoop.m_vElem.insert(firLoop.m_vElem.begin(),nexEle);
    firLoop.m_vElem.push_back(preEle);
    vLoop[0] = firLoop;
    for (int i = 1; i < vLoop.size() - 1; ++i) {
        int index;
        Point intPnt;
        entsol.SegLoopSpecifyIntersect(vLoop[i],startPt,
                                       cloPt,index,intPnt);
        entsol.PointSplitReorederLoop(intPnt,index,vLoop[i]);
    }
}
///从内向外加工,需要保证vLoop中的环是从外向内.
void SpecifyCutEntry::ClosetPointSplitNextLoop(const bool isToOutside,
                                               vector<DefLoop>&vLoop)
{
    if(vLoop.empty()){
        return;
    }
    if(isToOutside){
        ///反转vLoop便于处理.
        std::reverse(vLoop.begin(),vLoop.end());
    }
    double zCoord;
    DefLoop firLoop = vLoop[0];
    zCoord = ElemCalculate().LoopZCoordinate(firLoop);
    ///重置最里面的环元素
    CutEntrySolve entsol;
    entsol.ReorederSplitElement(firLoop);    vLoop[0] = firLoop;
    if(vLoop.size() == 1){
        return;
    }
    ElemCalculate numcal;
    Point preLoSp = numcal.LoopEndpoint(firLoop,false);
    for (int i = 0; i < vLoop.size() - 1; ++i) {
        Point closePt;
        int nex = numcal.NextIndex(vLoop.size(),i),index;
        DefLoop nexLoop = vLoop[nex];
        entsol.PointToLoopClosestPoint(preLoSp,nexLoop,index,closePt);
        closePt.SetZ(zCoord);
        entsol.PointSplitReorederLoop(closePt,index,vLoop[nex]);
        preLoSp = closePt;
    }
    if(isToOutside){
        std::reverse(vLoop.begin(),vLoop.end());
    }
}
///从外向内加工,需要保证vLoop中的环是从外向内.
void SpecifyCutEntry::FromOutsideToInsideMaching(vector<DefLoop>&vLoop)
{
    if(vLoop.empty()){
        return;
    }
    CutEntrySolve entsol;
    DefLoop firLoop = vLoop[0],lasLoop = vLoop.back();
    entsol.ReorederSplitElement(firLoop);
    vLoop[0] = firLoop;
    if(vLoop.size() == 1){
        return;
    }
    int index;
    Point startPt,cloPt;
    ElemCalculate numcal;
    numcal.ElementEndpoint(firLoop.m_vElem[0],false,startPt);
    entsol.PointToLoopClosestPoint(startPt,lasLoop,index,cloPt);
    DefElem ele = lasLoop.m_vElem[index],preEle,nexEle;
    numcal.ReorederLoopElement(index,true,false,lasLoop);
    numcal.PointSplitElement(ele,cloPt,true,preEle);
    numcal.PointSplitElement(ele,cloPt,false,nexEle);
    lasLoop.m_vElem.insert(lasLoop.m_vElem.begin(),nexEle);
    lasLoop.m_vElem.push_back(preEle);
    vLoop[vLoop.size() - 1] = lasLoop;
    for (int i = 1; i < vLoop.size() - 1; ++i) {
        int index;
        Point intPnt;
        entsol.SegLoopSpecifyIntersect(vLoop[i],startPt,cloPt,
                                       index,intPnt);
        entsol.PointSplitReorederLoop(intPnt,index,vLoop[i]);
    }
}
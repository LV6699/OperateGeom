#include "EquidIslandAux.h"
#include"../../CommonFile/Header.h"
using namespace std;
using namespace oft;
void EquidIslandAux::AddDeleteIslNode(vector<LoopNode>&iNodes,int i,
                                      oft::NodeForm& nodeForm)
{
    if(!iNodes[i]._offsetNode->_vLoopData.empty() ||
            !iNodes[i]._offsetNode->_vOffsetNode.empty()){
        iNodes[i]._offsetNode->_geneType = IslOffsetPath;
        nodeForm._iOftNodes.push_back(iNodes[i]._offsetNode);
    }
    iNodes.erase(iNodes.begin() + i);
}
///1.对于ioLoops中的外环,如果其包含在ioLoops中其他InOutLoop的_inLoops
///或iNodes中,则其应该被删除.
///即删除ioLoops中在其他环(iNodes或InOutLoop的内环中)的外环.
void EquidIslandAux::DeleteInsidedOutLoop(vector<LoopNode>& iNodes,
                                          vector<InOutLoop>& ioLoops)
{
    GeomContain gocon;
    for (size_t i = 0; i < ioLoops.size(); ++i) {
        size_t l = 0;
        InOutLoop& ioLoop = ioLoops[i];
        while (l < ioLoop._outLoops.size()) {
            const DefLoop& loop = ioLoop._outLoops[l];
            vector<DefLoop>loops;
            for (size_t k = 0; k < ioLoops.size(); ++k) {
                if(i == k)
                    continue;
                for(auto& d : ioLoops[k]._inLoops){
                    loops.push_back(d);
                }
            }
            for(auto& d : iNodes){
                loops.push_back(d.m_loop);
            }
            bool isIn = false;
            gocon.IsLoopsContainLoop(loops,loop,PreErr5_6,PreErr5_8,isIn);
            if(isIn){
                ioLoop._outLoops.erase(ioLoop._outLoops.begin() + l);
                continue;
            }
            l ++;
        }
    }
}
void EquidIslandAux::OperateEquidOutLoop(const vector<DefLoop>& inloops,
                                         vector<DefLoop>& outloops,
                                         vector<LoopNode>& iNodes,
                                         oft::NodeForm& nodeForm,
                                         vector<GeomArea>& regions)
{
    int i = 0;
    CheckGeomInt chint;GeomContain gocon;
    ///outloops 中的环可能是新的边界环 ,判断其是否包含iNodes中的岛屿环.
    while (i < outloops.size()){
        int j = 0;
        bool isOffset = true;
        vector<DefLoop>relLoops;        ///判断过程中,outloops和iNodes中的元素均可能删除
        while (j < iNodes.size()){
            const DefLoop& iloop = iNodes[j].m_loop;
            /// 判断outloops[i]与iloop的关系,包括不相关、包含或相交
            bool isExiInt = false;
            chint.IsLoopLoopIntersect(outloops[i],iloop,PreErr_10,isExiInt);
            if(isExiInt){
                relLoops.push_back(iloop);
                isOffset = false;/**
                还需要判断iloop与iNodes中的环是否存在相交,只有存在相交时,iloop
                才应该继续保留在其所在的 iNodes中,以供与iNodes中的环进行裁剪.*/
                vector<DefLoop>vOtherLoop;
                for (int k = 0; k < iNodes.size(); ++k) {
                    if(k != j){
                        vOtherLoop.push_back(iNodes[k].m_loop);
                    }
                }
                bool isExiInt_ = false;
                for (int k = 0; k < vOtherLoop.size(); ++k) {
                    chint.IsLoopLoopIntersect(iloop,vOtherLoop[k],
                                              PreErr_10,isExiInt_);
                    if(isExiInt_)
                        break;
                }
                if(!isExiInt_){
                    AddDeleteIslNode(iNodes,j,nodeForm);
                    continue;
                }
            }else{
                bool isInclude = false;
                gocon.IsLoopContainLoop(outloops[i],iloop,
                                        PreErr5_6,PreErr5_6,
                                        PreErr5_10,isInclude);
                /// 如果outloops[i]包含iloop
                if(isInclude){
                    relLoops.push_back(iloop);
                    AddDeleteIslNode(iNodes,j,nodeForm);
                    continue;
                }
            }
            j ++;
        }
        if(!relLoops.empty()){
            for(auto& l : relLoops){
                l._loopLabel._isLoopOffset = isOffset;
            }
            GeomArea data(outloops[i],relLoops,false,false);
            data._bndLoop._loopLabel._isLoopOffset = isOffset;
            regions.push_back(data);
            outloops.erase(outloops.begin() + j);
            continue;
        }
        i ++;
    }
    DeleteInsideIslNode(inloops,iNodes,nodeForm);
}
void EquidIslandAux::DeleteInsideIslNode(const vector<DefLoop>& inloops,
                                         vector<LoopNode>& iNodes,
                                         oft::NodeForm& nodeForm)
{
    ///对于iNodes中的环,如果其被包含在vOftPair中的m_inloops环中,则其应该被
    ///删除,除此以外，当其被包含在 iNodes中的其他环内,也应该被删除.    int i = 0;GeomContain gocon;
    while (i < iNodes.size()) {
        bool isContain = false;
        gocon.IsLoopsContainLoop(inloops,iNodes[i].m_loop,
                                 PreErr5_6,PreErr5_10,isContain);
        if(isContain){
            AddDeleteIslNode(iNodes,i,nodeForm);
            continue;
        }
        vector<DefLoop>vOtherIsl;
        ///ExceptionIslNode(iNodes,i,vOtherIsl);
        for (int j = 0; j < iNodes.size(); ++j) {
            if(j != i){
                vOtherIsl.push_back(iNodes[j].m_loop);
            }
        }
        bool isContain_ = false;
        gocon.IsLoopsContainLoop(vOtherIsl,iNodes[i].m_loop,
                                 PreErr5_6,PreErr5_10,isContain_);
        if(isContain_){
            AddDeleteIslNode(iNodes,i,nodeForm);
            continue;
        }
        i ++;
    }
}
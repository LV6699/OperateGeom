#include "EquidIslandAux.h"
#include"../../CommonFile/Header.h"
#include"../../GeometryCalculate/RelativeContain/LoopContain.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
using namespace std;
using namespace OffsetStruct;
void EquidIslandAux::AddDeleteIslNode(vector<LoopNode>&iNodes,int i,
                                      OffsetStruct::NodeForm& nodeForm)
{
    if(!iNodes[i]._offsetNode->_loops.empty() ||
            !iNodes[i]._offsetNode->_nodes.empty()){
        iNodes[i]._offsetNode->_geneType = IslOffsetPath;
        nodeForm._iOftNodes.emplace_back(iNodes[i]._offsetNode);
    }
    iNodes.erase(iNodes.begin() + i);
}
///1.对于ioLoops 中的外环,如果其包含在ioLoops 中其他InOutLoop 的_inLoops
///或iNodes 中,则其应该被删除.
///即删除ioLoops 中在其他环(iNodes 或InOutLoop 的内环中)的外环.
void EquidIslandAux::DeleteInsidedOutLoop(vector<LoopNode>& iNodes,
                                          vector<InOutLoop>& ioLoops)
{
    LoopContain lc;
    for (size_t i = 0; i < ioLoops.size(); ++i) {
        size_t l = 0;
        InOutLoop& ioLoop = ioLoops[i];
        while (l < ioLoop._outLoops.size()) {
            auto& loop = ioLoop._outLoops[l];
            loop.SetLimCoord(LimitCoord2D::LoopLimitCoord(loop));
            vector<DefLoop>loops;
            for (size_t k = 0; k < ioLoops.size(); ++k) {
                if(i == k)
                    continue;
                for(auto& d : ioLoops[k]._inLoops){
                    loops.emplace_back(d);
                }
            }
            for(auto& d : iNodes){
                loops.emplace_back(d.m_loop);
            }
            bool isIn = false;
            ///gocon.IsLoopsContainLoop(loops,loop,PreErr5_6,PreErr5_8,isIn);
            for(auto& l : loops){l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));}
            numst::ContParam cpa(PreErr5_6,PreErr5_8,PreErr5_8);
            isIn = LoopContain::IsLoopsContLoop(loops,loop,cpa);
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
                                         OffsetStruct::NodeForm& nodeForm,
                                         vector<GeomArea>& regions)
{
    int i = 0;
    CheckGeomInt chint;
    numst::ContParam cpa(PreErr5_6,PreErr5_6,PreErr5_10);
    ///outloops 中的环可能是新的边界环,判断其是否包含iNodes 中的岛屿环.
    while (i < outloops.size()){
        int j = 0;
        bool isOffset = true;
        vector<DefLoop>relLoops;
        auto& loop = outloops[i];
        LimitCoord2D::SetLoopLimCoord(loop);
        ///判断过程中,outloops 和iNodes 中的元素均可能删除
        while (j < iNodes.size()){
            auto& iloop = iNodes[j].m_loop;
            iloop.SetLimCoord(LimitCoord2D::LoopLimitCoord(iloop));
            ///判断outloops[i]与iloop 的关系,包括不相关、包含或相交
            bool isExiInt = false;
            chint.IsLoopLoopIntersect(loop,iloop,PreErr_10,isExiInt);
            if(isExiInt){
                relLoops.emplace_back(iloop);
                isOffset = false;/**
                还需要判断iloop 与iNodes 中的环是否存在相交,只有存在相交时,iloop
                才应该继续保留在其所在的iNodes 中,以供与iNodes 中的环进行裁剪.*/
                vector<DefLoop>vOtherLoop;
                for (int k = 0; k < iNodes.size(); ++k) {
                    if(k != j){vOtherLoop.emplace_back(iNodes[k].m_loop);}
                }
                bool isExiInt1 = false;
                for (int k = 0; k < vOtherLoop.size(); ++k) {
                    chint.IsLoopLoopIntersect(iloop,vOtherLoop[k],PreErr_10,isExiInt1);
                    if(isExiInt1){
                        break;
                    }
                }
                if(!isExiInt1){
                    AddDeleteIslNode(iNodes,j,nodeForm);continue;
                }
            }else{
                bool isInclude = LoopContain::IsLoopContainLoop(loop,iloop,cpa);
                ///如果outloops[i]包含iloop
                if(isInclude){
                    relLoops.emplace_back(iloop);
                    AddDeleteIslNode(iNodes,j,nodeForm);continue;
                }
            }
            j ++;
        }
        if(!relLoops.empty()){
            for(auto& l : relLoops){l._label._isOffset = isOffset;}
            GeomArea data(loop,relLoops,false,false);
            data._bndLoop._label._isOffset = isOffset;
            regions.emplace_back(data);
            outloops.erase(outloops.begin() + i);
            continue;
        }
        i ++;
    }
    DeleteInsideIslNode(inloops,iNodes,nodeForm);
}
void EquidIslandAux::DeleteInsideIslNode(vector<DefLoop> inloops,
                                         vector<LoopNode>& iNodes,
                                         OffsetStruct::NodeForm& nodeForm)
{
    ///对于iNodes 中的环,如果其被包含在vOftPair 中的m_inloops 环中,则其应该被
    ///删除,除此以外，当其被包含在iNodes 中的其他环内,也应该被删除.
    int i = 0;
    numst::ContParam cpa(PreErr5_6,PreErr5_10,PreErr5_10);
    for(auto& l : inloops){l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));}
    while (i < iNodes.size()) {
        bool isContain = false;
        auto& loop = iNodes[i].m_loop;
        loop.SetLimCoord(LimitCoord2D::LoopLimitCoord(loop));
        isContain = LoopContain::IsLoopsContLoop(inloops,loop,cpa);
        if(isContain){
            AddDeleteIslNode(iNodes,i,nodeForm);
            continue;
        }
        vector<DefLoop>vOtherIsl;
        ///ExceptionIslNode(iNodes,i,vOtherIsl);
        for (int j = 0; j < iNodes.size(); ++j) {
            if(j != i){
                vOtherIsl.emplace_back(iNodes[j].m_loop);
            }
        }
        bool isContain_ = false;
        for(auto& l : vOtherIsl){l.SetLimCoord(LimitCoord2D::LoopLimitCoord(l));}
        isContain_ = LoopContain::IsLoopsContLoop(vOtherIsl,loop,cpa);
        if(isContain_){
            AddDeleteIslNode(iNodes,i,nodeForm);
            continue;
        }
        i ++;
    }
}

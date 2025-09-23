#include <tbb/tbb.h>
#include "EquIslLoopSolve.h"
#if DebugModel
#pragma optimize("", off)
#endif
void EquIslLoopSolve::AddDeleteIslNode(vector<LoopNode>&vIslNode,int i,
                                       
vector<shared_ptr<OffsetNode>>&vIslOftNode)
{
    if(!vIslNode[i]._offsetNode->_vLoopData.empty() ||
            !vIslNode[i]._offsetNode->_vOffsetNode.empty()){
        vIslNode[i]._offsetNode->_geneType = IslOffsetPath;
        vIslOftNode.push_back(vIslNode[i]._offsetNode);
    }
    vIslNode.erase(vIslNode.begin() + i);
}
///1.对于vOftPair中的外环,如果其包含在vOftPair中其他EquWitOutLoop的m_vWitLoop
///或vIslNode中,则其应该被删除.
///即删除vWitOutLoop中在其他环(vIslNode或vWitOutLoop的内环中)的外环.
void EquIslLoopSolve::HandleOutLoopIncludedLoop(vector<LoopNode> &vIslNode,
                                                
vector<EquWitOutLoop>&vWitOutLoop)
{
    GeomContain gocon;
    for (int i = 0; i < vWitOutLoop.size(); ++i) {
        int lo = 0;
        while (lo < vWitOutLoop[i].m_vOutLoop.size()) {
            DefLoop curLoop = vWitOutLoop[i].m_vOutLoop[lo];
            vector<DefLoop>vOtherLoop;
            for (int k = 0; k < vWitOutLoop.size(); ++k) {
                if(i == k)
                    continue;
                for (int w = 0; w < vWitOutLoop[k].m_vWitLoop.size(); ++w)
                    vOtherLoop.push_back(vWitOutLoop[k].m_vWitLoop[w]);
            }
            for (int t = 0; t < vIslNode.size(); ++t)
                vOtherLoop.push_back(vIslNode[t].m_loop);
            bool isInOtherLoop = false;
            gocon.IsLoopsContainLoop(vOtherLoop,curLoop,PreErr5_6,
                                     PreErr5_8,isInOtherLoop);
            if(isInOtherLoop){
                
vWitOutLoop[i].m_vOutLoop.erase(vWitOutLoop[i].m_vOutLoop.begin() + lo);
                continue;
            }
            lo ++;
        }
    }
}
void EquIslLoopSolve::SummarizeIslEquOutLoop(vector<DefLoop>& vOutLoop,
                                             vector<LoopNode>& vIslNode,
                                             const vector<DefLoop>& vWitLoop,
                                             
vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                             vector<GeomArea>& vMatchLoop)
{
    int i = 0;
    bool isOffset = true;
    CheckGeomInt chint;    GeomContain gocon;
    ///vOutLoop 中的环可能是新的边界环 ,判断其是否包含vIslNode中的岛屿环.
    while (i < vOutLoop.size()) {
        int j = 0;
        vector<DefLoop>vRelaIslLoop;
        ///判断过程中,vOutLoop和vIslNode中的元素均可能删除
        while (j < vIslNode.size()) {
            DefLoop islLoop = vIslNode[j].m_loop;
            /// 判断vOutLoop[i]与curLoop的关系,包括不相关、包含或相交
            bool isExiInt = false;
            chint.IsLoopLoopIntersect(vOutLoop[i],islLoop,
                                      PreErr_10,isExiInt);
            /// 如果存在相交
            if(isExiInt){
                vRelaIslLoop.push_back(islLoop);
                isOffset = false;/**
                还需要判断curLoop与vIslNode中的环是否存在相交,只有存在相交时,
                curLoop 才应该继续保留在其所在的 vIslNode中,以供与vIslNode中
                的环进行裁剪.*/
                vector<DefLoop>vOtherLoop;
                for (int k = 0; k < vIslNode.size(); ++k) {
                    if(k != j)
                        vOtherLoop.push_back(vIslNode[k].m_loop);
                }
                bool isExiInt = false;
                for (int k = 0; k < vOtherLoop.size(); ++k) {
                    chint.IsLoopLoopIntersect(islLoop,vOtherLoop[k],
                                              PreErr_10,isExiInt);
                    if(isExiInt)
                        break;
                }
                if(!isExiInt){
                    AddDeleteIslNode(vIslNode,j,vIslOftNode);
                    continue;
                }
            }
            else{
                bool isInclude = false;
                gocon.IsLoopContainLoop(vOutLoop[i],islLoop,
                                        PreErr5_6,PreErr5_6,
                                        PreErr5_10,isInclude);
                /// 如果vOutLoop[i]包含curLoop
                if(isInclude){
                    vRelaIslLoop.push_back(islLoop);
                    AddDeleteIslNode(vIslNode,j,vIslOftNode);
                    continue;
                }
            }
            j ++;
        }
        if(!vRelaIslLoop.empty()){
            for(auto& lo : vRelaIslLoop)
                lo._loopLabel._isLoopOffset = isOffset;
            GeomArea data(vOutLoop[i],vRelaIslLoop,false,false);
            data._bndLoop._loopLabel._isLoopOffset = isOffset;
            vMatchLoop.push_back(data);
            vOutLoop.erase(vOutLoop.begin() + j);
            continue;
        }
        i ++;
    }
    ///对于vIslNode中的环,如果其被包含在vOftPair中的m_vWitLoop环中,则其应该被
    ///删除,除此以外，当其被包含在 vIslNode中的其他环内,也应该被删除.
    HandleIslNodeIncludedLoop(vWitLoop,vIslNode,vIslOftNode);}
void ExceptionIslNode(const vector<LoopNode>& vIslNode,
                      int index,vector<DefLoop>& vOtherIsl)
{
    for (int i = 0; i < vIslNode.size(); ++i) {
        if(i != index)
            vOtherIsl.push_back(vIslNode[i].m_loop);
    }
}
void EquIslLoopSolve::HandleIslNodeIncludedLoop(const vector<DefLoop>& vWitLoop,
                                                vector<LoopNode>& vIslNode,
                                                
vector<shared_ptr<OffsetNode>>&vIslOftNode)
{
    GeomContain gocon;
#if 1
    for(int i = 0;i < vIslNode.size();++i){
        bool matchResult = false;
        gocon.IsLoopsContainLoop(vWitLoop,vIslNode[i].m_loop,
                                 PreErr5_6,PreErr5_10,matchResult);
        if(matchResult){
            AddDeleteIslNode(vIslNode,i,vIslOftNode);
            continue;
        }
        vector<DefLoop>vOtherIsl;
        ExceptionIslNode(vIslNode,i,vOtherIsl);/**
        for (int lo = 0; lo < vIslNode.size(); ++lo) {
            if(lo == i)
                continue;
            vOtherIsl.push_back(vIslNode[lo].m_loop);
        }*/
        bool matchResult_ = false;
        gocon.IsLoopsContainLoop(vOtherIsl,vIslNode[i].m_loop,
                                 PreErr5_6,PreErr5_10,matchResult_);
        if(matchResult_){
            AddDeleteIslNode(vIslNode,i,vIslOftNode);
            continue;
        }
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0,vIslNode.size()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            bool matchResult = false;
            gocon.IsLoopsContainLoop(vWitLoop,vIslNode[i].m_loop,
                                     PreErr5_6,PreErr5_10,matchResult);
            if(matchResult){
                AddDeleteIslNode(vIslNode,i,vIslOftNode);
                continue;
            }
            vector<DefLoop>vOtherIsl;
            ExceptionIslNode(vIslNode,i,vOtherIsl);/**
            for (int lo = 0; lo < vIslNode.size(); ++lo) {
                if(lo == i)
                    continue;
                vOtherIsl.push_back(vIslNode[lo].m_loop);
            }*/
            bool matchResult_ = false;
            gocon.IsLoopsContainLoop(vOtherIsl,vIslNode[i].m_loop,
                                     PreErr5_6,PreErr5_10,matchResult_);
            if(matchResult_){                AddDeleteIslNode(vIslNode,i,vIslOftNode);
                continue;
            }
        }
    });
#endif
}
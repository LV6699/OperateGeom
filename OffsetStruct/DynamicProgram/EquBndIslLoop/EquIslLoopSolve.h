#ifndef EQUISLLOOPSOLVE_H
#define EQUISLLOOPSOLVE_H
#include"../../CommonFile/Header.h"
class EquIslLoopSolve
{
public:
    EquIslLoopSolve(){}
    void AddDeleteIslNode(vector<LoopNode>&vIslNode,int i,
                          vector<shared_ptr<OffsetNode>>&vIslOftNode);
    void HandleOutLoopIncludedLoop(vector<LoopNode> &vIslNode,
                                   vector<EquWitOutLoop>&vWitOutLoop);
    void SummarizeIslEquOutLoop(vector<DefLoop>& vOutLoop,
                                vector<LoopNode>& vIslNode,
                                const vector<DefLoop>& vWitLoop,
                                vector<shared_ptr<OffsetNode>>&vIslOftNode,
                                vector<GeomArea>& vMatchLoop);
    void HandleIslNodeIncludedLoop(const vector<DefLoop>& vWitLoop,
                                   vector<LoopNode>& vIslNode,
                                   vector<shared_ptr<OffsetNode> > 
&vIslOftNode);
};
#endif // EQUISLLOOPSOLVE_H
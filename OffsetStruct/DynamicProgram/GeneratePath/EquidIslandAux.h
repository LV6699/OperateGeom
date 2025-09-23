#ifndef EQUIDISLANDAUX_H
#define EQUIDISLANDAUX_H

#include<vector>
#include"../../CommonFile/DataStructure.h"

class EquidIslandAux{

public:

    void AddDeleteIslNode(vector<LoopNode>&iNodes,int i,
                          oft::NodeForm& nodeForm);

    void DeleteInsidedOutLoop(vector<LoopNode>& iNodes,
                              vector<InOutLoop>& ioLoops);


    void OperateEquidOutLoop(const vector<DefLoop>& inloops,
                             vector<DefLoop>& outloops,
                             vector<LoopNode>& iNodes,
                             oft::NodeForm& nodeForm,
                             vector<GeomArea>& regions);

    void DeleteInsideIslNode(const vector<DefLoop>& inloops,
                             vector<LoopNode>& iNodes,
                             oft::NodeForm& nodeForm);



}

#endif // EQUIDISLANDAUX_H

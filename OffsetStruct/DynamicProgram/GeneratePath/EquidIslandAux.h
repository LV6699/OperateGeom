#ifndef EQUIDISLANDAUX_H
#define EQUIDISLANDAUX_H

#include<vector>
#include"../../CommonFile/DataStructure.h"

class EquidIslandAux{

public:

    void AddDeleteIslNode(vector<LoopNode>&iNodes,int i,
                          ofts::NodeForm& nodeForm);

    void DeleteInsidedOutLoop(vector<LoopNode>& iNodes,
                              vector<InOutLoop>& ioLoops);


    void OperateEquidOutLoop(const vector<DefLoop>& inloops,
                             vector<DefLoop>& outloops,
                             vector<LoopNode>& iNodes,
                             ofts::NodeForm& nodeForm,
                             vector<GeomArea>& regions);

    void DeleteInsideIslNode(const vector<DefLoop>& inloops,
                             vector<LoopNode>& iNodes,
                             ofts::NodeForm& nodeForm);



}

#endif // EQUIDISLANDAUX_H

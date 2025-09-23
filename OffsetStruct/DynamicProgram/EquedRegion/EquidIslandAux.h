#ifndef EQUIDISLANDAUX_H
#define EQUIDISLANDAUX_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class EquidIslandAux
{
public:
    EquidIslandAux(){}
    void AddDeleteIslNode(std::vector<OffsetStruct::LoopNode>&iNodes, int i,
                          OffsetStruct::NodeForm& nodeForm);
    void DeleteInsidedOutLoop(std::vector<OffsetStruct::LoopNode>& iNodes,
                              std::vector<OffsetStruct::InOutLoop>& ioLoops);
    void OperateEquidOutLoop(const std::vector<OffsetStruct::DefLoop>& inloops,
                             std::vector<OffsetStruct::DefLoop>& outloops,
                             std::vector<OffsetStruct::LoopNode>& iNodes,
                             OffsetStruct::NodeForm& nodeForm,
                             std::vector<OffsetStruct::GeomArea>& regions);
    void DeleteInsideIslNode(const std::vector<OffsetStruct::DefLoop>& inloops,
                             std::vector<OffsetStruct::LoopNode>& iNodes,
                             OffsetStruct::NodeForm& nodeForm);
};
#endif // EQUIDISLANDAUX_H
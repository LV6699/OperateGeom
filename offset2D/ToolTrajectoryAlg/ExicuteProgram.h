#ifndef PROGEXICUTE_H
#define PROGEXICUTE_H
#include"CommonFile/Header.h"
class CAMALGORITHM_EXPORT ExicuteProgram
{
public:
    ExicuteProgram(){}
    void CheckOffsetParam(ofts::OffsetParam &param);
    void LoopOffsetNodeResult(const ofts::DefLoop &loop,
                              ofts::OffsetParam param,
                              std::shared_ptr<ofts::OffsetNode> &oftNode,
                              bool isPrecocess = true);
    void GetLoopOffsetNode(ofts::OffsetParam param,
                           const ofts::DefLoop& loop,
                           std::shared_ptr<ofts::OffsetNode> &rootNode);
    void GetMultipleLoopOffset(const std::vector<ofts::DefLoop>&loops,
                               Ofte::ToolPathType pathType,
                               ofts::OffsetParam param,
                               bool isAddOriLoop,
                               std::shared_ptr<ofts::OffsetNode>& rootNode);
    void FirstOffsetSmooth(ofts::OffsetParam& param,ofts::DefLoop& loop);
private:
    void ExicuteLoopOffset(const ofts::DefLoop &loop,
                           ofts::OffsetParam param,
                           std::vector<ofts::DefLoop>& locLoops,
                           std::vector<ofts::DefLoop>& resLoops,
                           bool isPrecocess = true);
};
#endif // PROGEXICUTE_H

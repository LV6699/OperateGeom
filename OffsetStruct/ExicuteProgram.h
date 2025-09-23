#ifndef PROGEXICUTE_H
#define PROGEXICUTE_H
#include"CommonFile/Header.h"
class CAMALGORITHM_EXPORT ExicuteProgram
{
public:
    ExicuteProgram(){}
    void CheckoutOffsetParameter(OffsetParam &param);
    void LoopOffsetNodeResult(const DefLoop &loop,
                              OffsetParam param,
                              shared_ptr<OffsetNode> &oftNode,
                              bool isStaticTime = true);
    void GetLoopOffsetNode(OffsetParam param,
                           const DefLoop& loop,
                           shared_ptr<OffsetNode> &rootNode);
    void GetMultipleLoopOffset(const vector<DefLoop>&loops,
                               ToolPathType pathType,
                               OffsetParam param,
                               bool isAddOriLoop,
                               shared_ptr<OffsetNode>& rootNode);
    void FirstOffsetSmooth(OffsetParam& oftPara,DefLoop& loop);
private:
    void ExicuteOffsetCalculate(const DefLoop &loop,
                                OffsetParam param,
                                vector<DefLoop>& locLoops,
                                vector<DefLoop>& resLoops);
};
#endif // PROGEXICUTE_H
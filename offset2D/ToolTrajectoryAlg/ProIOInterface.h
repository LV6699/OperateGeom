#ifndef PROIOINTERFACE_H
#define PROIOINTERFACE_H
#include"CommonFile/Header.h"
class CAMALGORITHM_EXPORT ProIOInterface
{
public:
    ProIOInterface(){}
    bool IsValidExicute(ofts::GeomArea& region,
                        ofts::OffsetParam param,
                        std::shared_ptr<ofts::OffsetNode>& pathNode);
    void PreprocesseInput(ofts::GeomArea& region,
                          ofts::OffsetParam& param);
    void BorderLoopOffset(ofts::GeomArea& region,
                          ofts::OffsetParam& param,
                          std::shared_ptr<ofts::OffsetNode>& pathNode);
    void InPutOutPutIO(ofts::GeomArea region, ofts::OffsetParam param,
                       std::shared_ptr<ofts::OffsetNode>& pathNode);
    void NodePathAfterprocess(ofts::OffsetParam param,
                              double zCoord,
                              std::shared_ptr<ofts::OffsetNode>& pathNode);
};
#endif // PROIOINTERFACE_H
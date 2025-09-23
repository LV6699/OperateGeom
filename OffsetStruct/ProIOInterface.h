#ifndef PROIOINTERFACE_H
#define PROIOINTERFACE_H
#include"CommonFile/Header.h"
class CAMALGORITHM_EXPORT ProIOInterface
{
public:
    ProIOInterface(){}
    bool IsValidExicuteCalculate(GeomArea& region,
                                 OffsetParam param,
                                 shared_ptr<OffsetNode>& pathNode);
    void PreprocesseInputData(GeomArea& region,
                              OffsetParam& param);
    void BorderLoopOffsetInOutIO(GeomArea& region,
                                 OffsetParam& param,
                                 shared_ptr<OffsetNode>& pathNode);
    void InPutOutPutIO(GeomArea region, OffsetParam param,
                       shared_ptr<OffsetNode>& pathNode);
    void NodePathResultTreatment(OffsetParam param,
                                 double zCoord,
                                 shared_ptr<OffsetNode>& pathNode);
};
#endif // PROIOINTERFACE_H
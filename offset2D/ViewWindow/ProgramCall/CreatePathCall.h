#ifndef CREATEPATHCALL_H
#define CREATEPATHCALL_H
#include"../HearderFile.h"
class CreatePathCall
{
public:
    CreatePathCall(){}
    void CallCreateToolPath(BndIslPair &pairLoop,
                            OffsetParam offsetPar,
                            shared_ptr<OffsetNode>&rootNode);
    void ExicuteToolPathCalculate(BndIslPair pairLoop,
                                  OffsetParam offsetPar,
                                  shared_ptr<OffsetNode>&rootNode);
};
#endif // CREATEPATHCALL_H
#ifndef PREPROCLOOP_H
#define PREPROCLOOP_H
#include"../../CommonFile/DataStructure.h"
class CAMALGORITHM_EXPORT PreprocLoop
{
public:
    PreprocLoop(){}
    void SetLineHorVerQual(ofts::Point& sp, ofts::Point& ep,
                           bool isChEp, double pre);
    void ResetArcCenter(ofts::DefArc& arc);
    void ResetArcEndpoint(bool isLast,ofts::DefArc& arc);
    void OptInputPreprocess(const ofts::OffsetParam& param,
                            ofts::DefLoop& loop,
                            std::vector<ofts::DefLoop>&loops);
    void OptPreprocessLoop(const ofts::OffsetParam& param,
                           ofts::DefLoop& loop);
    void PreprocessLoop(const ofts::OffsetParam& param,ofts::DefLoop& loop);
    void InputPreprocess(const ofts::OffsetParam& param,ofts::DefLoop& loop,
                         std::vector<ofts::DefLoop>& loops);
    void DeleteInvalidCurve(double segPre,double arcPre,
                            std::vector<ofts::DefElem>& cs);
    void DeleteInvalidCurve(double segPre, double arcPre,
                            ofts::DefLoop& loop);
};
#endif // PREPROCLOOP_H

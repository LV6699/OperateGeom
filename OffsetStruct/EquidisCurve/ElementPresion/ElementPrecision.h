#ifndef HANDLEELEPRE_H
#define HANDLEELEPRE_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT ElementPrecision
{
public:
    ElementPrecision(){}
    void SpecifyInputPretreatment(bool isFixedOft,
                                  bool isAblePreProc,
                                  double oftVaue,
                                  bool isIniRadi,
                                  bool isIniSur,
                                  bool isDelInval,
                                  bool isEndtoend,
                                  bool isHandleClose,
                                  DefLoop& loop,
                                  vector<DefLoop>&vLoop);
    void SpecifyPretreatmentLoop(bool isFixedOft,
                                 bool isAblePreProc,
                                 double oftVaue,
                                 bool isIniRad,
                                 bool isIniSur,
                                 bool isDelInval,
                                 bool isEndtoend,
                                 bool isHandleClose,
                                 DefLoop& loop);
    void PretreatmentLoop(DefLoop& loop, bool isIniRad,
                          bool isIniSur, bool isDelInval,
                          bool isEndtoend, bool isHandleClose);
    void InputPretreatment(DefLoop& loop,
                           vector<DefLoop>&vLoop,
                           bool isIniRadi,
                           bool isIniSur,
                           bool isIniInvalEle,
                           bool isEndtoend,
                           bool isHandleClose);
    void LoopElementEndToEnd(DefLoop &loop);
    void HandleLoopSurplusElem(DefLoop& loop);
    void DeleteInvalidElement(double segPre,double arcPre,
                              vector<DefElem>& vElem);
    void DeleteInvalidElement(double segPre, double arcPre,
                              DefLoop& loop);
};
#endif // HANDLEELEPRE_H
#ifndef OPENLOOPSELINT_H
#define OPENLOOPSELINT_H
#include"../../../CommonFile/Header.h"
class OpenLoopSelInt
{
public:
    OpenLoopSelInt(){}
    void SpecifySelIntEleValidCutPart(const DefLoop& oriLoop,
                                      const DefElem& ele,
                                      double offsetVal,
                                      const Point &P,
                                      bool& isCurEleVal,
                                      bool& isNexEleVal,
                                      DefElem &valCutEle);
    void HandleOpenLoopInvalidEle(const DefLoop& oriLoop,
                                  const DefElem& ele,
                                  double offsetVal,
                                  bool& isLocVal,
                                  DefElem& valPart);
    bool IsIntEleExistValCutPart(const DefLoop &oriLoop,
                                 const DefLoop &loop,
                                 const DefElem &ele,
                                 double offsetVal,
                                 const Point &P,
                                 bool &isCurCutVal,
                                 DefElem &valCutEle);
};
#endif // OPENLOOPSELINT_H
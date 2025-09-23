#ifndef LOOPBOOLSUBTRACT_H
#define LOOPBOOLSUBTRACT_H
#include"../../CommonFile/Header.h"
class LoopBoolSubtract
{
public:
    LoopBoolSubtract(){}
    void InputPreprocesse(DefLoop& tempLoop,
                          vector<DefLoop>& vLoop,
                          vector<DefLoop> &vResLoop);
    void LoopGeomBoolSubtract(DefLoop tempLoop,
                                  vector<DefLoop> vLoop,
                                  vector<DefLoop>& vResLoop);
    void IteratorTemplateLoop(DefLoop& tempLoop,
                              vector<DefLoop>& vLoop,
                              vector<DefLoop>& vResLoop);
};
#endif // LOOPBOOLSUBTRACT_H
#ifndef MULTIPLEINCLUDE_H
#define MULTIPLEINCLUDE_H
#include"../../CommonFile/Header.h"
class MultipleInclude
{
public:
    MultipleInclude(){}
    void OrganizeLoopToArea(const vector<DefLoop> &vLoop,
                            double intPre, double coinPre, double incPre,
                            vector<GeomArea>& vBILoop);
    void LoopMultipleInclude(const vector<DefLoop> &vLoop, double intPre,
                             double coinPre, double incPre,
                             vector<GeomArea>& vBILoop);
    void LoopIncludeLoops(const DefLoop& loop,
                          vector<DefLoop>& vLoop, double intPre,
                          double coinPre, double incPre, bool isDel,
                          vector<DefLoop>& vIncLoop);
};
#endif // MULTIPLEINCLUDE_H
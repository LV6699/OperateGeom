#ifndef GEOMCONTAIN_H
#define GEOMCONTAIN_H
#include"../../CommonFile/Header.h"
class GeomContain
{
public:
    GeomContain(){}
    void IsLoopOutsideLoopCoord(const DefLoop& outLoop,
                                const DefLoop& loop,
                                double pre,
                                bool& isCoorOut);
    void IsLoopContainLoopCoord(const DefLoop& outLoop,
                                const DefLoop& loop,
                                double pre,
                                bool& isCoorInc);
    void IsLoopContainIntersectLoop(const DefLoop& outLoop,
                                    const DefLoop&loop,
                                    double incErrVal,
                                    double coinErrVal,
                                    double intErrVal,
                                    bool isMatchBord,
                                    bool& resValue);
    void IsLoopContainLoop(const DefLoop& outLoop,
                           const DefLoop&loop,
                           double intPre,
                           double coinPre,
                           double incPre,
                           bool& isContain);
    void IsLoopsContainIntersectLoop(const vector<DefLoop>& loops,
                                     const DefLoop& loop,
                                     double incErrVal,
                                     double coinErrVal,
                                     double intErrVal,
                                     bool isMatchBord,
                                     bool& resValue);
    void IsLoopsContainLoop(const vector<DefLoop>& loops,
                            const DefLoop& loop, double intPre,
                            double pre,
                            bool& isContain);
    void IsLoopsContainLoopCoord(const vector<DefLoop> &loops,
                                 const DefLoop& loop,
                                 double pre,
                                 bool& isInclude);
};
#endif // GEOMCONTAIN_H
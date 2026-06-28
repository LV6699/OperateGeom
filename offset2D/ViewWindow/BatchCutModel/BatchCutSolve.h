#ifndef BATCHCUTSOLVE_H
#define BATCHCUTSOLVE_H
#include"../HearderFile.h"
class BatchCutSolve
{
public:
    BatchCutSolve(){}
    void CreateSpecifyDirectory(string dir, bool isRemove);
    void CreateCutModelDirectory();
    void GetBndIslContourWeight(BndIslPair& bndIslLoop);
    void SetBndIslMatchOffsetVal(BndIslPair& biLoop);
    void GetOffsetValBaseScale(double minX, double maxX, double minY,
                               double maxY, double &toolRadius,
                               bool &isValScale,
                               vector<double>& vOffsetVal);
    bool IsMatchLoopSimilarProperty(const BndIslPair& biLoop1,
                                    const BndIslPair& biLoop2);
};
#endif // BATCHCUTSOLVE_H
#ifndef BATCHCUTMODEL_H
#define BATCHCUTMODEL_H
#include"../HearderFile.h"
class BatchCutModel
{
public:
    BatchCutModel(){}
    void GetContourMaxWeightLoop(const vector<vector<BndIslPair> > &vAreLayLoop,
                                 BndIslPair &bndIslLoop);
    void GetFixedNumMaxWeightLoop(const vector<vector<BndIslPair> > &vAreLayLoop,
                                  int num, vector<BndIslPair>&vBndIslLoop);
    void GetSpecifyBndIslLoop(vector<vector<BndIslPair> > &vAreLayLoop,
                              vector<BndIslPair> &vBILoop);
    bool IsValidContourMatchLoop(const vector<BndIslPair>&vBndIslLoop,
                                 const BndIslPair& bndIslLoop);
};
#endif // BATCHCUTMODEL_H
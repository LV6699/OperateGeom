#ifndef LOOPBOOLSUBT_H
#define LOOPBOOLSUBT_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class LoopBoolSubt
{
public:
    LoopBoolSubt(){}
    void InputPreprocesse(OffsetStruct::DefLoop &tloop,
                          std::vector<ofts::DefLoop>& loops);
    void DeleteInvalidCurves(ofts::DefLoop &tloop,
                             ofts::DefLoop& tcloop,
                             std::vector<ofts::DefLoop> &loops,
                             std::vector<ofts::DefLoop>& cloops);
    void LoopBoolSubtract(ofts::DefLoop tloop,
                          std::vector<ofts::DefLoop> loops,
                          std::vector<ofts::DefLoop>& rloops);
    bool HasClip()const{return _hasClip;}
public:
    bool _hasClip = false;
};
#endif // LOOPBOOLSUBT_H

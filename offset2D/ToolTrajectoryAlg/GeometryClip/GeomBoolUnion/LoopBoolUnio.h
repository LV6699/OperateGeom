#ifndef LOOPBOOLUNIO_H
#define LOOPBOOLUNIO_H
#include<vector>
#include"../../CommonFile/DataStructure.h"
class LoopBoolUnio
{
public:
    LoopBoolUnio(){}
    void InputPreprocesse(std::vector<ofts::DefLoop>& loops);
    void DeleteInvalidCurves(const std::vector<ofts::DefLoop>& orils,
                             std::vector<OffsetStruct::DefLoop> &cloops);
    void LoopsBoolUnion(std::vector<OffsetStruct::DefLoop> loops,
                        std::vector<ofts::DefLoop>& rloops);
    bool HasClip()const{return _hasClip;}
public:
    bool _hasClip = false;
};
#endif // LOOPBOOLUNIO_H

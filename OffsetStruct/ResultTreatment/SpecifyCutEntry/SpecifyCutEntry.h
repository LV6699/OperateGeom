#ifndef SPECIFYCUTENTRY_H
#define SPECIFYCUTENTRY_H
#include"../../CommonFile/Header.h"
class SpecifyCutEntry
{
public:
    SpecifyCutEntry(){}
    void RedesignPathBaseOnEntry(shared_ptr<OffsetNode>& rootNode,
                                 bool isToOutside);
    ///vLoop是加工区域刀路,isToOutside指定是否从内向外加工 .
    void SpecifyCutEntryPoint(vector<DefLoop>&vLoop,bool isToOutside);
    ///需要保证vLoop中的环是从外向内.
    void CutEntryPointSplitLoop(const bool isToOutside,
                                vector<DefLoop>&vLoop);
    ///需要保证vLoop中的环是从外向内.
    void FromInsideToOutsideMaching(vector<DefLoop>&vLoop);
    ///需要保证vLoop中的环是从外向内.
    void ClosetPointSplitNextLoop(const bool isToOutside,
                                  vector<DefLoop>&vLoop);
    ///需要保证vLoop中的环是从外向内.
    void FromOutsideToInsideMaching(vector<DefLoop>&vLoop);
};
#endif // SPECIFYCUTENTRY_H
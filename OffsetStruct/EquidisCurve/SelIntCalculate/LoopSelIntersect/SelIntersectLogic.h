#ifndef SELINTERSECTLOGIC_H
#define SELINTERSECTLOGIC_H
#include "../../../CommonFile/Header.h"
class SelIntersectLogic
{
public:
    SelIntersectLogic() {}
    void AddIndexElement(const DefLoop& loop,
                         vector<vector<int>>vArrIndex,
                         vector<DefLoop>&vResLoop);
    void ClipSelfIntersectLoop(const DefLoop &oriLoop,
                               const DefLoop &loop,
                               double offsetVal,
                               bool &isIntersect,
                               DefLoop &resLoop);
    void SplitSelfIntersectLoop(const DefLoop &oriLoop,
                                DefLoop loop,
                                double offsetVal,
                                bool &isIntersect,
                                DefLoop &resLoop);
    void DeleteInvalidClipElement(const DefLoop &oriLoop,
                                  double offsetVal,
                                  bool isOpenLoop,
                                  bool &isExiDelete,
                                  vector<DefElem>&elems);
    void SplitElementLocalLoop(DefLoop loop,
                               vector<DefLoop>&vResLoop);
    void OrgnizeElementToLoop(DefLoop loop, bool isOpen,
                              vector<DefLoop>&vResLoop);
};
#endif // SELINTERSECTLOGIC_H
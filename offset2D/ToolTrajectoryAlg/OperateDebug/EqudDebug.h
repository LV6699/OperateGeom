#ifndef EQUDDEBUG_H
#define EQUDDEBUG_H
#include"../CommonFile/Header.h"
class EqudDebug
{
public:
    EqudDebug(){}
    void EquedElementDistance(const vector<DefElem>& vOriElem,
                              const vector<DefElem>& vElem,
                              double offsetVal, vector<int> &vIndex);
    void SameEndpointIndex(const vector<DefElem> &vElem,
                           double pre,
                           vector<int>&vIndex);
    void NotConnectElemIndex(const vector<DefElem>& vElem,
                             double pre,
                             vector<int>&vIndex);
    void ArcGeomInfo(const DefArc& arc,double &d1,double &d2);
    void NotMeetArcIndex(const DefLoop &loop,double e,vector<int>&vIndex);
    void InformationCase(GeomArea biLoop,
                         OffsetParam offsetPara,
                         shared_ptr<OffsetNode>rootNode);
    void LoopDataDebug(const DefLoop &loop);
    void PathNodeDebug(const shared_ptr<OffsetNode>&rootNode);
    bool IsLoopExistPoint(const DefLoop& loop, const OffsetStruct::Point& P,
                          double err = PreErr5_3);
};
#endif // EQUDDEBUG_H
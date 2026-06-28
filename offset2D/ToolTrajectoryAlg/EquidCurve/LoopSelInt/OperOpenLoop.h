#ifndef OPEROPENLOOP_H
#define OPEROPENLOOP_H
#include "../../CommonFile/DataStructure.h"
class OperOpenLoop
{
public:
    OperOpenLoop(){}

    void CreateEndClipArc(const ofts::DefLoop& oloop,
                          const ofts::DefElem& sc,
                          const ofts::DefElem& ec,double R,
                          ofts::DefArc& arc1, ofts::DefArc& arc2);
    void MergeOpenLoops(ofts::DefLoop& loop,double e);
    void MergeOpenLoops(std::vector<ofts::DefLoop>& loops,double e);





};
#endif // OPEROPENLOOP_H

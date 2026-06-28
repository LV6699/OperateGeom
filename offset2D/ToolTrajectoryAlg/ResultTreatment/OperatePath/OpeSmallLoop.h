#ifndef OPESMALLLOOP_H
#define OPESMALLLOOP_H
#include"../../CommonFile/Header.h"
class OpeSmallLoop
{
public:
    OpeSmallLoop(){}
    void ExpandPathSmallLoop(double R,
                             std::shared_ptr<OffsetNode>& pathNode);
};
#endif // OPESMALLLOOP_H

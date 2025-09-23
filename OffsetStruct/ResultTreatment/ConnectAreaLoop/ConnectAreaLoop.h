#ifndef CONNECTAREALOOP_H
#define CONNECTAREALOOP_H
#include"../../CommonFile/Header.h"
class ConnectAreaLoop
{
public:
    ConnectAreaLoop(){}
    void AreaLoopStraightConnect(vector<DefLoop>vLoop,
                                 vector<DefLoop> &vResLoop);
    ///需要保证vLoop中的环是从外向内.
    void ConnectAreaLoopToLoop(vector<DefLoop> vLoop,
                               bool isSmoothConnect,
                               bool isToOutside,
                               vector<DefLoop> &vResLoop);
    void AreaLoopArcConnect(vector<DefLoop> vLoop,
                            bool isToOutside,
                            vector<DefLoop> &vResLoop);
    void CreateConnectAreaLoopArc(const Point& preEp,
                                  const Point& nexSp,
                                  bool isToOutside,
                                  bool isPreCW, bool isNexCW,
                                  DefArc& arc1, DefArc& arc2);
};
#endif // CONNECTAREALOOP_H
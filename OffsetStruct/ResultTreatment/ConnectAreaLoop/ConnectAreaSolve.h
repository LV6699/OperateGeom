#ifndef CONNECTAREASOLVE_H
#define CONNECTAREASOLVE_H
#include "ConnectAreaLoop.h"
class ConnectAreaSolve
{
public:
    ConnectAreaSolve(){}
    void CreateConnectAreaLoopArc(const Point& preEp,
                                  const Point& nexSp,
                                  bool isToOutside,
                                  bool isPreCW, bool isNexCW,
                                  DefArc& arc1, DefArc& arc2);
    void ConnectTwoLoopEndpoint(const DefLoop& preLoop,
                                const DefLoop&nexLoop,
                                bool isSmooth,
                                DefLoop &resLoop);
};
#endif // CONNECTAREASOLVE_H
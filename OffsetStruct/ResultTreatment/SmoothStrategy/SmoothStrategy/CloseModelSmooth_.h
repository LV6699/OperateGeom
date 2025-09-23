FuLNRX4J6wCnaNe72doeTI7TNnjP
Page 1#ifndef CLOSEMODELSMOOTH_H
#define CLOSEMODELSMOOTH_H
#include"../../../CommonFile/Header.h"
class CloseModelSmooth
{
public:
    CloseModelSmooth(){}
    void ObjectSmoothLoop(const DefLoop& oriLoop
                          OffsetParam offsetPara
                          DefLoop& loop);
    void CloseModelPathSmooth(OffsetParam offsetPara
                              GeomArea& biLoop);
};
#endif // CLOSEMODELSMOOTH_H
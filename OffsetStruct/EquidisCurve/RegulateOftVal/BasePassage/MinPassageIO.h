FrY47gDnrfTgJQPhTA6aLcBYiNf6
Page 1#ifndef MINPASSAGEIO_H
#define MINPASSAGEIO_H
#include"../../../CommonFile/Header.h"
class MinPassageIO
{
public:
    MinPassageIO(){}
    void OffsetValueBasePassage(const DefLoop& loop
                                double R double &offsetVal);
    void BasePassagePreprocess(const DefLoop& loop  double R
                               DefLoop& loop_);
};
#endif // MINPASSAGEIO_H
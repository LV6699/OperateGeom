FokbhptWw79ycxYLklWb_nXSD6RU
Page 1#ifndef SELINTERSECTAUX_H
#define SELINTERSECTAUX_H
#include "../../../CommonFile/Header.h"
class SelIntersectAux
{
public:
    SelIntersectAux(){}
    void ElemToLoopDistanceType(const DefElem &ele
                              const DefLoop &loop
                              double offsetVal
                              EleLoopDisType &disType);
    void SetCurLoopClose(bool isOpen DefLoop& loop);
};
#endif // SELINTERSECTAUX_H
#ifndef NUMSTRUCT_H
#define NUMSTRUCT_H

#include "NumPreErr.h"

namespace NumStruct {
class ReLoopErr{
public:
    ReLoopErr(){}

private:
    double _lAglErr = 0;
    double _aRadErr = 0;
    double _aCpErr = 0;
    double _closeErr = 1;
};
}

#endif // NUMSTRUCT_H

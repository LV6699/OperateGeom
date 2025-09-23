FjqylTVmCabb8Xfkc3Bdclji8bJa
Page 1#ifndef NUMSTRUCT_H
#define NUMSTRUCT_H
#include "NumPreErr.h"
namespace NumStruct {
class ReLoopErr{
public:
    ReLoopErr(){}
    ReLoopErr(double l double ardouble ac
        _lAglErr(l) _aRadErr(ar)_aCpErr(ac)
    void SetAglErr(double v){_lAglErr = v;}
    void SetRadErr(double v){_aRadErr = v;}
    void SetCpErr(double v){_aCpErr = v;}
    void SetCloseErr(double v){_aCpErr = v;}
    double RelAglErr() const{return _lAglErr;}
    double RelRadErr() const{return _aRadErr;}
    double RelCpErr() const{return _aCpErr;}
    double RelCloseErr() const{return _closeErr;}
private:
    double _lAglErr = 0;
    double _aRadErr = 0;
    double _aCpErr = 0;
    double _closeErr = 1;
};
}
#endif // NUMSTRUCT_HFjqylTVmCabb8Xfkc3Bdclji8bJa
Page 2double close = PreErr5_10) :
_closeErr(close){}
#ifndef ORGANSUBTRLOOP_H
#define ORGANSUBTRLOOP_H
#include"../../CommonFile/Header.h"
class OrganSubtrLoop
{
public:
    OrganSubtrLoop(){}
    void TemplateLoopForwardIterate(DefLoop& tempLoop,
                                    vector<DefLoop> &vLoop,
                                    Point endpoint,
                                    int eleIndex,
                                    vector<DefElem>& vElement);
    void ArrayLoopReverseIterate(DefLoop& tempLoop,
                                 vector<DefLoop> &vLoop,
                                 Point endpoint, int loIndex,
                                 int eleIndex,
                                 vector<DefElem>& vElement);
};
#endif // ORGANSUBTRLOOP_H
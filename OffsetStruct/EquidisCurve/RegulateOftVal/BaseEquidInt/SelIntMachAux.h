#ifndef SELINTMACHAUX_H
#define SELINTMACHAUX_H
#include"../../../CommonFile/Header.h"
class SelIntMachAux
{
public:
    SelIntMachAux(){}
    void IsRequireReset(double offsetVal,
                        double R,
                        double distance,
                        bool &isReset);
    void IsEleEquExiSelInt(const DefElem& ele1,
                           const DefElem& ele2,
                           bool isAdjoin,
                           ProfType profType,
                           double offsetVal,
                           bool &isResetOftVal);
    void IsResetOftValBaseSelInt(const DefLoop &loop,
                                 const DefElem& ele,
                                 int index, double offsetVal,
                                 bool &isReset);
};
#endif // SELINTMACHAUX_H
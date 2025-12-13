#ifndef EXICUTECALC_H
#define EXICUTECALC_H

#include"../Solution/CutLocateZ.h"

class ExicuteCalc
{
public:
    ExicuteCalc(){}

    void LineTriangleClInt(const ofts::Point& p);

    void GetRelaTris(double x,double y,std::vector<grm::Triangle>& ts);





};

#endif // EXICUTECALC_H

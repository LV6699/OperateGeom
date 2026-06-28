#ifndef FINDINFORMATION_H
#define FINDINFORMATION_H
#include"../CommonFile/Header.h"
class FindInformation
{
public:
    FindInformation(){}
    bool IsPointInLoop(const DefLoop& loop, double err,
                       bool isBorder,int index, const Point& p);
    bool IsPointInArrayLoop(const std::vector<DefLoop>& vLoop,
                            double err,const Point& p);
    bool IsPointInPathNode(const std::shared_ptr<OffsetNode>& pathNode,
                           double err,const Point& p);
    bool IsPointInGeomArea(const GeomArea& area,double err,const Point& p);
};
#endif // FINDINFORMATION_H
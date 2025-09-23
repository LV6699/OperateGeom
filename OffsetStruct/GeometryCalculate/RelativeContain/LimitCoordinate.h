#ifndef LimitCoordinate_H
#define LimitCoordinate_H
#include"../../CommonFile/Header.h"
class LimitCoordinate
{
public:
    LimitCoordinate(){}
    void SpecifyHorVerCoordValue(double minX,double maxX,
                                 double minY,double maxY,
                                 bool isHor,bool isMax,
                                 double& specifyVal);
    void SegLimitHorVerCoord(const Point& lSp,
                             const Point& lEp,
                             bool isHor, bool isMax,
                             double& specifyVal);
    void SegLimitCoordPoint(const Point& lSp,const Point& lEp,
                            bool isHor, bool isMax,Point& P);
    void ArcLimitHorVerCoord(DefArc arc,
                             bool isHor, bool isMax,
                             Point& P);
    void CircleLimitHorVerCoord(const Point& cp,
                                double R,
                                bool isHor, bool isMax,
                                double& specifyVal);
    void CircleLimitCoordPoint(const Point& cp,
                                double R,bool isHor,
                                bool isMax,Point& P);
    void ElementLimitCoord(const DefElem& ele,
                           bool isHor, bool isMax,
                           double &value);
    void ElemLimitCoordPoint(const DefElem& ele,
                             bool isHor, bool isMax,
                             Point& P);
    void LoopLimitCoordPoint(const DefLoop& loop,
                             bool isHor, bool isMax, Point &p);
    void LoopLimitCoordinate(const DefLoop& loop,
                             bool isHor, bool isMax,
                             double& limVal);
    void IsPointInsideLoopCoord(const DefLoop& loop,
                                const Point& P, double pre,
                                bool& isIncCoor);
    void IsEleInsideLoopCoord(const DefLoop& loop,
                              const DefElem& ele,
                              double pre,
                              bool& isIncCoor);
    void IsElemOutsideLoopCoord(const DefLoop& loop,
                                const DefElem& ele,
                                double pre,
                                bool& isOutCoord);    void LoopLimitCoordinate(const DefLoop& loop,
                             double& minX,double& maxX,
                             double& minY,double& maxY);
    bool IsPointInLoopCoord(const DefLoop &loop,
                            const Point &P,
                            double errVal);
    void LoopMinCircumcircle(const DefLoop &loop,DefCircle& circle);
    void LoopMinimumBox(const DefLoop&loop,double &xSpan,double &ySpan);
    bool IsLoopMinBoxMeetLenth(const DefLoop&loop, double len, bool isGreater);
};
#endif // LimitCoordinate_H
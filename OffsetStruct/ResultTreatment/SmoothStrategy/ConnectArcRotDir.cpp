#ifndef ConnectArcRotDir_H
#define ConnectArcRotDir_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT ConnectArcRotDir
{
public:
    ConnectArcRotDir(){}
    void ArcRotDirBaseOriginLine(Point lSp, Point lEp, bool isNex,
                                 DefArc& arc,RotDir& rotDir);
    void ArcRotDirBaseOriginArc(const DefArc& baseArc,bool isNex,
                                DefArc& arc,RotDir& rotDir);
    void ArcDirectionBaseOriginElem(const DefElem& ele,bool isNex,
                                    DefArc& arc,RotDir& rotDir);
    void ArcRotBaseOriginElement(const DefElem& ele,
                                 ProfType loopType,
                                 RotDir& rotDir);
    void ArcRotBasePrevoiusElement(const DefElem& preEle,
                                   ProfType loopType,
                                   RotDir& rotDir);
    void ArcRotBasePrevoiusSeg(const Point& lSp,
                               const Point& lEp,
                               ProfType loopType,
                               RotDir& rotDir);
    void ArcRotBasePrevoiusArc(const DefArc& arc,
                               ProfType loopType,
                               RotDir& rotDir);
};
#endif // ConnectArcRotDir_H
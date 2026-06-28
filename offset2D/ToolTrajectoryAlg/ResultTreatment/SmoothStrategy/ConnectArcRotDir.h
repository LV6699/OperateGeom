#ifndef ConnectArcRotDir_H
#define ConnectArcRotDir_H
#include"../../CommonFile/Header.h"
class CAMALGORITHM_EXPORT ConnectArcRotDir
{
public:
    ConnectArcRotDir(){}
    void ArcRotDirBaseOriginLine(Point lSp, Point lEp, bool isNex,
                                 DefArc& arc,bool& iscw);
    void ArcRotDirBaseOriginArc(const DefArc& baseArc,bool isNex,
                                DefArc& arc,bool& iscw);
    void ArcDirectionBaseOriginElem(const DefElem& ele,bool isNex,
                                    DefArc& arc,bool& iscw);
    void ArcRotBaseOriginElement(const DefElem& ele,
                                 bool leftward,
                                 bool& iscw);
    void ArcRotBasePrevoiusElement(const DefElem& preEle,
                                   bool leftward,
                                   bool& iscw);
    void ArcRotBasePrevoiusSeg(const Point& lSp,
                               const Point& lEp,
                               bool leftward,
                               bool& iscw);
    void ArcRotBasePrevoiusArc(const DefArc& arc,
                               bool leftward,
                               bool& iscw);
};
#endif // ConnectArcRotDir_H

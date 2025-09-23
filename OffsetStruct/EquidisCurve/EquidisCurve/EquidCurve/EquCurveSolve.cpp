#include "EquCurveSolve.h"
#if DebugModel
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
void EquCurveSolve::CreateDoubleArc(const DefElem& preOriEle,
                                    ProfType loopType,double R,
                                    vector<DefElem>&vEquElem)
{
    ElemCalculate numcal;
    ConnectArcRotDir arcrot;
    RotDir rotDir = CLOCKWISE;
    Point cp = numcal.ElementEndpoint(preOriEle,true);
    Point sp(cp.X() - R,cp.Y(),cp.Z()),ep(cp.X() + R,cp.Y(),cp.Z());
    DefArc arc1(sp,ep,cp,R,rotDir),arc2(ep,sp,cp,R,rotDir);
    arcrot.ArcRotBasePrevoiusElement(preOriEle,loopType,rotDir);
    arc1.SetArcRot(rotDir);
    arc2.SetArcRot(rotDir);
    vEquElem.push_back(DefElem(ARCTYPE,arc1));
    vEquElem.push_back(DefElem(ARCTYPE,arc2));
}
void EquCurveSolve::NotExistEquidCurve(const DefElem& preOriEle,
                                       const DefArc& oriArc,
                                       ProfType loopType,double R,
                                       vector<DefElem>&vEquElem)
{
    ConnectArcRotDir arcrot;
    Point cp1 = oriArc.ArcStartPt();
    Point cp2 = oriArc.ArcEndPt();
    Point sp1(cp1.X() - R,cp1.Y()),ep1(cp1.X() + R,cp1.Y(),cp1.Z()),
            sp2(cp2.X() - R,cp2.Y()),ep2(cp2.X() + R,cp2.Y(),cp2.Z());
    RotDir rotDir = CLOCKWISE;
    DefArc arc11(sp1,ep1,cp1,R,rotDir),arc12(ep1,sp1,cp1,R,rotDir);
    DefArc arc21(sp2,ep2,cp2,R,rotDir),arc22(ep2,sp2,cp2,R,rotDir);
    ///arcrot.ArcDirectionBaseOriginElem(preOriEle,true,arc11,rotDir);
    arcrot.ArcRotBasePrevoiusElement(preOriEle,loopType,rotDir);
    arc11.SetArcRot(rotDir);
    arc12.SetArcRot(rotDir);
    arc21.SetArcRot(rotDir);
    arc22.SetArcRot(rotDir);
    vEquElem.push_back(DefElem(ARCTYPE,arc11));
    vEquElem.push_back(DefElem(ARCTYPE,arc12));
    vEquElem.push_back(DefElem(ARCTYPE,arc21));
    vEquElem.push_back(DefElem(ARCTYPE,arc22));
}
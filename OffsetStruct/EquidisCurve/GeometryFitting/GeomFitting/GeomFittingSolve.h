#ifndef GEOMFITTINGSOLVE_H
#define GEOMFITTINGSOLVE_H
#include"../../../CommonFile/Header.h"
class GeomCalculate;
class GeomFittingSolve
{
public:
    GeomFittingSolve(){}
    bool IsMeetAdjoinAngle(const DefLoop &loop,int i,double aErr){
        int pre = loop.PreviousIndex(i);
        const auto& sp = loop[pre]._segment._segSp;
        const auto& ep = loop[pre]._segment._segEp;
        const auto& ep1 = loop[i]._segment._segEp;
        GeomCalculate gocal;
        double a = 0,a1 = 0;
        gocal.SegmentAngle(sp,ep,a);
        gocal.SegmentAngle(sp,ep1,a1);
        RotDir rotDir = CLOCKWISE;
        gocal.TriangleRotDirection(sp,ep,ep1,rotDir);
        double aglAbs = 0;
        gocal.TwoAngleIncludedAngle(a,a1,rotDir,aglAbs);
        return aglAbs <= aErr ? true : false;
    }
    bool IsMeetFittingCase(const DefElem& ele){
        if(ele.ElemeType() != LINETYPE)
            return false;
        const auto& lSp = ele._segment._segSp;
        const auto& lEp = ele._segment._segEp;
        double lenLim = 3;
        double len = lSp.Distance2D(lEp);
        if(len > lenLim)
            return false;
        return true;
    }
    bool ReferenceArc(const DefLoop &loop,
                      OffsetStruct::GeomFitting::FittArc &fitData)
    {
        if(fitData._indexs.size() < 2)
            return false;
        fitData._preArc = fitData._arc;
        DefArc arc = fitData._arc;
        vector<Point>points = fitData._nodes;
        vector<int>ids = fitData._indexs;
        if(ids.size() < 3){
            for (int i = 0; i < ids.size(); ++i) {
                int id = ids[i];
                points.push_back(loop.IndexElem(id).ElemSeg().SegStartPt());
            }
            fitData._nodes = points;
        }
        points.push_back(loop.IndexElem(ids.back()).ElemSeg().SegEndPt());
        
fitData._nodes.push_back(loop.IndexElem(ids.back()).ElemSeg().SegEndPt());
        Point intPnt;
        GeomCalculate gocal;GeomIntersect goint;
        int m = static_cast<int>(static_cast<int>(points.size()) / 2);
        const auto& sp = points[0];
        const auto& mp = points[m];
        const auto& ep = points.back();        double A,B,C,A_,B_,C_;
        gocal.SegmentBisectorVertical(sp,mp,A,B,C);
        gocal.SegmentBisectorVertical(mp,ep,A_,B_,C_);
        bool isExiInt = false;
        goint.CalculateLineParameterInt(A,B,C,A_,B_,C_,isExiInt,intPnt);
        if(!isExiInt){
            if(fitData._indexs.size() < 3)
                return false;
            Point mp0 = sp.Midpoint(mp),mp1 = mp.Midpoint(ep),
                    cp = mp0.Midpoint(mp1);
            double d = cp.Distance2D(fitData._arc._arcCp);
            if(d > PreErr5_2)
                return false;
            intPnt = cp;
        }
        RotDir rotDir = CLOCKWISE;
        if(ids.size() < 3){
            gocal.TriangleRotDirection(intPnt,sp,ep,rotDir);
            arc.SetArcRot(rotDir);
        }
        else{
            if(!intPnt.IsSamePoint2D(fitData._arc._arcCp,2.5))
                return false;
            const auto& seg = loop[ids.back()]._segment;
            const auto& lsp = seg._segSp;
            const auto& lep = seg._segEp;
            gocal.TriangleRotDirection(intPnt,lsp,lep,rotDir);
            if(rotDir != fitData._arc.ArcRot())
                return false;
        }
        arc.SetArcStartPt(sp);
        arc.SetArcEndPt(ep);
        arc.SetArcCenterPt(intPnt);
        double d1 = sp.Distance2D(intPnt),d2 = ep.Distance2D(intPnt);
        arc.SetArcRadius((d1 + d2) / 2);
        fitData._arcCps.push_back(intPnt);
        fitData._arcRadius.push_back(arc.ArcRadius());
        fitData._arc = arc;
        return true;
    }
    bool IsMeetTangentAngle(const DefArc& arc,const Point &sp,
                            const Point &ep,const Point& P,
                            double &aErr);
    void TangentAngle(const DefArc& arc,const Point &sp,
                      const Point &ep,const Point& P,
                      double &angle){
        double la = 0;
        GeomCalculate gocal;
        gocal.SegmentAngle(sp,ep,la);
        double tanAg = gocal.ArcPointTangentAngle(arc,P);
        gocal.TwoAngleIncludedAngle(tanAg,la,arc.ArcRot(),angle);
    }
    bool IsMeetBowHeight(const DefLoop &loop,
                         const OffsetStruct::GeomFitting::FittArc &fitData,
                         double hErr,double &h){
        if(fitData._indexs.size() < 2)
            return false;
        GeomCalculate gocal;
        vector<Point>points;
        vector<int>ids = fitData._indexs;        for (int i = 0; i < ids.size(); ++i) {
            Point mp;
            const auto& ele = loop[ids[i]];
            points.push_back(ele._segment._segSp);
            mp = ele._segment._segSp.Midpoint(ele._segment._segEp);
            points.push_back(mp);
            if(i == ids.size() - 1){
                points.push_back(ele._segment._segEp);
            }
        }
        h = 0;
        const auto& cp = fitData._arc._arcCp;
        double R = fitData._arc.ArcRadius();
        for (int i = 0; i < points.size(); ++i) {
            double d = cp.Distance2D(points[i]);
            double h_ = std::abs(R - d);
            if(h_ > h){
                h = h_;
                if(h > hErr){
                    return false;
                }
            }
        }
        return true;
    }
    bool CurrentIteratorValidity(const DefLoop &loop,
                                 const OffsetStruct::GeomFitting::FittArc 
&fitData,
                                 double aErr, double hErr){
        DefArc arc = fitData._arc;
        for (int i = 0; i < fitData._indexs.size(); ++i) {/**
            if(i == 50){
                int tem = 0;
            }*/
            int id = fitData._indexs[i];
            if(!loop.IsValIndex(id))
                return false;
            const auto& lSp = loop[id]._segment._segSp;
            const auto& lEp = loop[id]._segment._segEp;
            double angle = 0;
            TangentAngle(arc,lSp,lEp,lSp,angle);
            if(angle > aErr){
                if(std::abs(360 - angle) <= aErr)
                    continue;
                return false;
            }
        }
        double h = 0;
        if(!IsMeetBowHeight(loop,fitData,hErr,h) || h > hErr)
            return false;
        return true;
    }
};
#endif // GEOMFITTINGSOLVE_H
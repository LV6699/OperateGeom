#ifndef LINECUTSOLVE_H
#define LINECUTSOLVE_H
#include <cmath>
#include <algorithm>
#include"../UnitStruct/GeomUnitStruct.h"
class LineCutSolve
{
public:
    LineCutSolve(){}
    void ResetAngle(double& agl){
        if(std::abs(agl) <= PreErr_1 ||
                std::abs(std::abs(agl) - 180) < PreErr_1 ||
                std::abs(std::abs(agl) - 360) < PreErr_1){
            agl = 0;
        }else if(std::abs(std::abs(agl) - 90) < PreErr_1||
                std::abs(std::abs(agl) - 270) < PreErr_1){
            agl = 90;
        }else if(agl > 0 && agl < 180)
            return;
        else if(agl > 180 && agl < 360){
            agl -= 180;
        }else if(agl < 0 && agl > -180){
            agl += 180;
        }else if(agl < -180 && agl > -360){
            agl += 360;
        }else{///agl > 360 || agl < -360
            agl = std::fmod(agl,360);
            ResetAngle(agl);
        }
    }
    bool IsValidInt(double agl,
                    const ofts::Point& p1,const ofts::Point& p2,
                    const ofts::Point& p1_,const ofts::Point& p2_){
        double max1 = 0,min1 = 0,max1_ = 0,min1_ = 0;
        if(std::abs(agl) <= PreErr_1 ||
                std::abs(std::abs(agl) - 180) < PreErr_1 ||
                std::abs(std::abs(agl) - 360) < PreErr_1){
            max1 = p1.Y();min1 = p2.Y();
            max1_ = p1_.Y();min1_ = p2_.Y();
        }else{
            max1 = p1.X();min1 = p2.X();
            max1_ = p1_.X();min1_ = p2_.X();
        }
        if(max1 < min1){
            std::swap(max1,min1);
        }
        if(max1_ < min1_){
            std::swap(max1_,min1_);
        }
        if(((max1 > max1_ && min1 > min1_) ||
            (max1 < max1_ && min1 < min1_))){
            return true;
        }
        return false;
    }
    void SetLoopClose(ofts::DefLoop& loop);
    double LoopMaxZCoord(const ofts::DefLoop& loop);
    void ClassfyLoopsBaseZ(const std::vector<ofts::DefLoop>& vLoop,
                           std::vector<ofts::GeomArea>& areas);
    bool IsInsideRectRange(const GeomUnit::Rectangle& rec,
                           double v){
        double minx = rec._vertex._p0._x;
        if(minx > rec._vertex._p1._x){minx = rec._vertex._p1._x;}
        if(minx > rec._vertex._p2._x){minx = rec._vertex._p2._x;}
        if(minx > rec._vertex._p3._x){minx = rec._vertex._p3._x;}
        if(v < minx){return false;}
        double maxx = rec._vertex._p0._x;
        if(maxx < rec._vertex._p1._x){maxx = rec._vertex._p1._x;}
        if(maxx < rec._vertex._p2._x){maxx = rec._vertex._p2._x;}
        if(maxx < rec._vertex._p3._x){maxx = rec._vertex._p3._x;}
        if(v > maxx){return false;}
        double miny = rec._vertex._p0._y;
        if(miny > rec._vertex._p1._y){miny = rec._vertex._p1._y;}
        if(miny > rec._vertex._p2._y){miny = rec._vertex._p2._y;}
        if(miny > rec._vertex._p3._y){miny = rec._vertex._p3._y;}
        if(v < miny){return false;}
        double maxy = rec._vertex._p0._y;
        if(maxy < rec._vertex._p1._y){maxy = rec._vertex._p1._y;}
        if(maxy < rec._vertex._p2._y){maxy = rec._vertex._p2._y;}
        if(maxy < rec._vertex._p3._y){maxy = rec._vertex._p3._y;}
        if(v > maxy){return false;}
        double minz = rec._vertex._p0._z;
        if(minz > rec._vertex._p1._z){minz = rec._vertex._p1._z;}
        if(minz > rec._vertex._p2._z){minz = rec._vertex._p2._z;}
        if(minz > rec._vertex._p3._z){minz = rec._vertex._p3._z;}
        if(v < minz){return false;}
        double maxz = rec._vertex._p0._z;
        if(maxz < rec._vertex._p1._z){maxz = rec._vertex._p1._z;}
        if(maxz < rec._vertex._p2._z){maxz = rec._vertex._p2._z;}
        if(maxz < rec._vertex._p3._z){maxz = rec._vertex._p3._z;}
        if(v > maxz){return false;}
        return true;
    }
    bool IsVertexNormalRecInt(const ofts::Point& p,
                              const ofts::Point& dir,
                              const GeomUnit::Rectangle& rec,
                              double l,
                              ofts::Point& res)
    {
        const ofts::Point&p0 = rec._vertex._p0,
                p1 = rec._vertex._p1,p2 = rec._vertex._p2;
        ofts::Point recNor = (p1 - p0).Cross(p2 - p0).Normalized2D();
        ///璁＄畻鍒嗘瘝
        double deno = dir.Dot(recNor);
        ///妫€鏌ュ钩琛屾儏鍐
        if (std::fabs(deno) < 1e-6f)
            return false;
        ///璁＄畻浜ょ偣鍙傛暟t
        double t = (p0 - p).Dot(recNor) / deno;
        ///妫€鏌ヤ氦鐐规槸鍚﹀湪绾挎涓
        if (t < 0 || t > l)
            return false;
        ///璁＄畻浜ょ偣鍧愭爣
        ofts::Point intersection = p + dir * t;
        ///璁＄畻鐭╁舰杈瑰悜閲
        ofts::Point u = p1 - p0;
        ofts::Point v = p0 - p0;
        ofts::Point w = intersection - p0;
        /// 璁＄畻鎶曞奖鍙傛暟
        double uu = u.Dot(u);
        double uv = u.Dot(v);
        double vv = v.Dot(v);
        double wu = w.Dot(u);
        double wv = w.Dot(v);
        double deno1 = uv*uv - uu*vv;
        /// 璁＄畻s 鍜宼鍙傛暟
        double s = (uv*wv - vv*wu) / deno1;
        double t_param = (uv*wu - uu*wv) / deno1;
        /// 妫€鏌ユ槸鍚﹀湪鐭╁舰鍐
        if (s >= 0 && s <= 1 && t_param >= 0 && t_param <= 1) {
            res = intersection;
            return true;
        }
        return false;
    }
    bool IsVertexNormalRecsInt(const ofts::Point& p,
                               const ofts::Point& dir,
                               const std::vector<std::vector<GeomUnit::Rectangle>>& vRecs,
                               double l,
                               ofts::Point& res){
        for(const auto& d : vRecs){
            for(const auto& r : d ){
                if(IsVertexNormalRecInt(p,dir,r,l,res))
                    return true;
            }
        }
        return false;
    }
    void ResetLoopOrderBaseX(ofts::DefLoop& loop);
    void LineOffsetStartPoint(const ofts::Point& sp,
                              const ofts::Point& ep,
                              double agl,ofts::Point& p);
    void LineCutStartLine(const ofts::DefLoop& loop,
                          double dist, double agl,
                          ofts::Point& p1,
                          ofts::Point& p2);
    void ResetIntPntOrder(double agl,std::vector<GeomUnit::IntPnt>& intPts)
    {
        bool isx = true;
        if(std::abs(agl - 90) < 10 || std::abs(agl - 270) < 10){
            isx = false;
        }
        if(isx){
            std::sort(intPts.begin(),intPts.end(),[](const GeomUnit::IntPnt& a,
                      const GeomUnit::IntPnt& b) {return a._p.X() < b._p.X();});
            return;
        }
        std::sort(intPts.begin(),intPts.end(),[](const GeomUnit::IntPnt& a,
                  const GeomUnit::IntPnt& b) {return a.P().Y() < b.P().Y();});
    }
    void ResetIntPntOrder(double agl,GeomUnit::IntLine& intl)
    {
        bool isx = true;
        if(std::abs(agl - 90) < 10 || std::abs(agl - 270) < 10){
            isx = false;
        }
        if(isx){
            std::sort(intl._vIntPt.begin(),intl._vIntPt.end(),[](const GeomUnit::IntPnt* a,
                      const GeomUnit::IntPnt* b) {return a->_p.X() < b->_p.X();});
            return;
        }
        std::sort(intl._vIntPt.begin(),intl._vIntPt.end(),[](const GeomUnit::IntPnt* a,
                  const GeomUnit::IntPnt* b) {return a->P().Y() < b->P().Y();});
    }
    void DeleteShortPoints(std::vector<GeomUnit::IntPnt>&pts,double e)
    {
        std::vector<int>vIndex;
        for (int i = 0; i < pts.size()-1; ++i) {
            if(pts[i]._p.Distance2D(pts[i+1]._p) < e){
                vIndex.emplace_back(i);
                vIndex.emplace_back(i+1);
                i ++;
            }
        }
        if(vIndex.empty())
            return;
        for (auto it = vIndex.rbegin();it != vIndex.rend(); ++it) {
            pts.erase(pts.begin() + (*it));
        }
    }
    void ResetLineIntPoint(const ofts::DefLoop &loop,
                           const ofts::Point &sp, const ofts::Point &ep,
                           double agl, double err,
                           std::vector<GeomUnit::IntPnt>& intPts);
    void OrgnizeIntPointBaseElem(const ofts::DefLoop& loop,
                                 std::vector<GeomUnit::IntPnt>& intPts);
    void OrgnizeIntPointBaseLoop(const ofts::GeomArea &area,
                                 std::vector<GeomUnit::IntPnt>& intPts);
    void IntPointToCutLine(const ofts::GeomArea &area, double agl,
                           std::vector<GeomUnit::IntPnt>& intPts,
                           std::vector<GeomUnit::IntLine>& intLines);
};
#endif // LINECUTSOLVE_H
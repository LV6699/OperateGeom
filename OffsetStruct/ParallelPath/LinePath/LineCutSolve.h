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
        }
        else if(std::abs(std::abs(agl) - 90) < PreErr_1||
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
                    const Point& p1,const Point& p2,
                    const Point& p1_,const Point& p2_){
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
    void SetLoopClose(OftStr::DefLoop& loop);
    double LoopMaxZCoord(const OftStr::DefLoop& loop);
    void ClassfyLoopsBaseZ(const std::vector<OftStr::DefLoop>& vLoop,
                           std::vector<OftStr::GeomArea>& areas);    bool IsInsideRectRange(const GeomUnit::Rectangle& rec,
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
    bool IsVertexNormalRecInt(const OftStr::Point& p,
                              const OftStr::Point& dir,
                              const GeomUnit::Rectangle& rec,
                              double l,
                              OftStr::Point& res)
    {
        const OftStr::Point&p0 = rec._vertex._p0,
                p1 = rec._vertex._p1,p2 = rec._vertex._p2;
        OftStr::Point recNor = (p1 - p0).Cross(p2 - p0).Normalize();
        ///计算分母
        double deno = dir.Dot(recNor);
        ///检查平行情况
        if (std::fabs(deno) < 1e-6f)
            return false;
        ///计算交点参数t
        double t = (p0 - p).Dot(recNor) / deno;
        ///检查交点是否在线段上
        if (t < 0 || t > l)
            return false;
        ///计算交点坐标
        OftStr::Point intersection = p + dir * t;
        ///计算矩形边向量
        OftStr::Point u = p1 - p0;
        OftStr::Point v = p0 - p0;
        OftStr::Point w = intersection - p0;        /// 计算投影参数
        double uu = u.Dot(u);
        double uv = u.Dot(v);
        double vv = v.Dot(v);
        double wu = w.Dot(u);
        double wv = w.Dot(v);
        double deno1 = uv*uv - uu*vv;
        /// 计算s和t参数
        double s = (uv*wv - vv*wu) / deno1;
        double t_param = (uv*wu - uu*wv) / deno1;
        /// 检查是否在矩形内
        if (s >= 0 && s <= 1 && t_param >= 0 && t_param <= 1) {
            res = intersection;
            return true;
        }
        return false;
    }
    bool IsVertexNormalRecsInt(const OftStr::Point& p,
                               const OftStr::Point& dir,
                               const 
std::vector<std::vector<GeomUnit::Rectangle>>& vRecs,
                               double l,
                               OftStr::Point& res){
        for(const auto& d : vRecs){
            for(const auto& r : d ){
                if(IsVertexNormalRecInt(p,dir,r,l,res))
                    return true;
            }
        }
        return false;
    }
    void ResetLoopOrderBaseX(OftStr::DefLoop& loop);
    void LineOffsetStartPoint(const OftStr::Point& sp,
                              const OftStr::Point& ep,
                              double agl,OftStr::Point& p);
    void LineCutStartLine(const OftStr::DefLoop& loop,
                          double dist, double agl,
                          OftStr::Point& p1,
                          OftStr::Point& p2);
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
        }        if(isx){
            std::sort(intl._vIntPt.begin(),intl._vIntPt.end(),[](const 
GeomUnit::IntPnt* a,
                      const GeomUnit::IntPnt* b) {return a->_p.X() < b-
>_p.X();});
            return;
        }
        std::sort(intl._vIntPt.begin(),intl._vIntPt.end(),[](const 
GeomUnit::IntPnt* a,
                  const GeomUnit::IntPnt* b) {return a->P().Y() < b->P().Y();});
    }
    void DeleteShortPoints(std::vector<GeomUnit::IntPnt>&pts,double e)
    {
        std::vector<int>vIndex;
        for (int i = 0; i < pts.size(); ++i) {
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
    void ResetLineIntPoint(const OftStr::DefLoop &loop,
                           const OftStr::Point &sp, const OftStr::Point &ep,
                           double agl, double err,
                           std::vector<GeomUnit::IntPnt>& intPts);
    void OrgnizeIntPointBaseElem(const OftStr::DefLoop& loop,
                                 std::vector<GeomUnit::IntPnt>& intPts);
    void OrgnizeIntPointBaseLoop(const OftStr::GeomArea &area,
                                 std::vector<GeomUnit::IntPnt>& intPts);
    void IntPointToCutLine(const OftStr::GeomArea &area, double agl,
                           std::vector<GeomUnit::IntPnt>& intPts,
                           std::vector<GeomUnit::IntLine>& intLines);
};
#endif // LINECUTSOLVE_H
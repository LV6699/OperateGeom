///#include <tbb/tbb.h>
#include "ParallelLogic.h"
#include "../../../offset2D/ToolTrajectoryAlg/CommonFile/Header.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize("O0")
#endif
using namespace std;
void ParallelLogic::SetPointsZ(const std::shared_ptr<cls::model> &clsMod,
                               std::vector<oft::Point> &points)
{
#if 1
    double z;
    //vector<double> v;
    for (size_t i = 0; i < points.size(); ++i) {
        rmath::vec2 p(points[i].X(), points[i].Y());
        z = cls::get_cutter_location(p, *clsMod);
        points[i].SetZ(z);
        //v.emplace_back(z);
    }
#else
    tbb::parallel_for(
                tbb::blocked_range<size_t>(0, points.size()),
                [&](const tbb::blocked_range<std::size_t> &range) {
        for (size_t i = range.begin(); i != range.end(); i++) {
            rmath::vec2 p(points[i].X(), points[i].Y());
            points[i].SetZ(cls::get_cutter_location(p, *clsMod));
        }
    });
#endif
}
void ParallelLogic::SetLoopZCoord(const shared_ptr<cls::model>&clsMod,
                                  DefLoop& loop)
{
    for (int i = 0; i < loop.ElemNum(); ++i) {
        int nex = loop.NextIndex(i);
        Point &ep = loop.m_vElem[i]._segment._segEp;
        double z = cls::get_cutter_location(
                    rmath::vec2(ep.X(),ep.Y()),*clsMod);
        ep.SetZ(z);
        loop.m_vElem[nex]._segment._segSp.SetZ(z);
    }
}
void ParallelLogic::PointsPrecisionBaseCLS(const 
                                           std::shared_ptr<cls::model>&clsMod,
                                           std::vector<Point>& points)
{
    /*cls::contours clsPts;
    for(auto& d : points){
        clsPts.push_back(rmath::vec2(d.X(),d.Y()));
    }
    cls::path out;
    cls::set_line_on_cls(clsPts,clsMod,PreErr5_5,out);*/
}
void ParallelLogic::SetLoopsZCoord(const std::shared_ptr<cls::model>&clsMod,
                                   std::vector<DefLoop>& loops)
{#if 0
    string str = OperateOftString().ArrayLoopString(loops);
    std::cout<<str<<endl;
#endif
    GeometryLogic golg;GeomCalculate gocal;
    for (int l = 0; l < loops.size(); ++l) {
        DefLoop& loop = loops[l];
        vector<vector<Point>>looppts;
        for (int i = 0; i < loop.ElemNum(); ++i) {/**
            if(i == 10 || i == 30 || i == 50 || i == 80){
                int tem = 1;
            }*/
            vector<Point>pts;
#if 0
            const Point &sp = loop.m_vElem[i]._segment._segSp;
            const Point &ep = loop.m_vElem[i]._segment._segEp;
            if (sp.IsSamePoint2D(Point(0,88.1041069030761,0), PreErr5_4) &&
                    sp.IsSamePoint2D(Point(0,88.1041069030761,0),
                                     PreErr5_4)) {
                double d = sp.Distance2D(ep);
                std::cout << d << endl;
            }
#endif
            golg.DiscreteCurveToPoint(loop.m_vElem[i],PreErr_1,
                                      PreErr5_3,false,pts);
            SetPointsZ(clsMod,pts);
            gocal.SimplePointsBaseLine(PreErr5_3,pts);
#if 0
            Point pt(0,88.1041069030761,0);
            for (auto &p : pts) {
                if (!ElemCalculate().IsValidPoint(p) ||
                        p.IsSamePoint3D(pt, PreErr5_4)) {
                    double d = p.Distance2D(Point(0, 0));
                    std::cout << d << endl;
                }
            }
#endif
            PointsPrecision(clsMod,PreErr5_3,PreErr5_3,pts);
#if 0
            Point pt_(0,88.1041069030761,0);
            for (auto &p : pts) {
                if (!ElemCalculate().IsValidPoint(p) ||
                        p.IsSamePoint3D(pt_, PreErr5_4)) {
                    double d = p.Distance2D(Point(0, 0));
                    std::cout << d << endl;
                }
            }
#endif
            looppts.push_back(pts);
        }
        vector<DefElem>elems;
        for (const auto &v : looppts) {
            if (v.size() < 2)
                continue;
            for (size_t j = 0; j < v.size() - 1; j++) {
                elems.push_back(DefElem(LINETYPE,DefSeg(v[j],v[j+1])));
            }
        }
#if 0
        string str = OperateOftString().LoopElementString(loop);
        string str_ = OperateOftString().LoopElementString(DefLoop(elems));
        std::cout<<str<<str_<<endl;
#endif
        loop.SetElem(elems);    }
}

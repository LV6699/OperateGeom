#include "CreateGrid.h"

using namespace std;
using namespace oft;

#pragma optimize("", off)

namespace grm{

void CreateGrid::CreateModelGrid(grm::ClModel& m,
                                 double step,
                                 vector<vector<Point>>&clPts)
{
    m.BoundaryPoint();
    double x = m.MinXPt().X(),x1 = m.MaxXPt().X(),
            y = m.MinYPt().Y(),y1 = m.MaxYPt().Y();
    double cx = x,cy = y;
    while (cy < y1) {
        vector<Point>xs;
        while (cx < x1) {
            xs.emplace_back(Point(cx,cy));
            cx += step;
        }
        clPts.emplace_back(xs);
        cy += step;
    }
}





}
















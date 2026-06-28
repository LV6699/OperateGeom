#include "LoopClipAux.h"
#include "../../GeometryCalculate/RelativeContain/PointInLoop.h"
using std::vector;
using namespace OffsetStruct;
bool LoopClipAux::IsPointInLoops(vector<DefLoop>& loops,
                                 const Point& P, bool isBoundary, double bErr)
{
#if 0
    string s = OperateOftString().ArrayLoopString(loops);
    std::cout<<s<<endl;
#endif
    for(auto& l : loops){LimitCoord2D::SetLoopLimCoord(l);}
    ///PointInPolygon pip;
    for (size_t i = 0; i < loops.size(); ++i) {
        bool isOnBorder = false,isInPoly = false;///PreErr5_10
        ///pip.IsPointInsideLoop(loops[i],P,bErr,isOnBorder,isInPoly);
        PointInLoop().GetPointInsideLoop(loops[i],P,bErr,isOnBorder,isInPoly);
        if(isOnBorder){
            if(isBoundary){
                return false;
            }
            return true;
        }
        else if(isInPoly){
            return true;
        }
    }
    return false;
}

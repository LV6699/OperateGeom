#include "MinPassageSolve.h"
using namespace oft::ProgOftValue;
#if DebugModel
#pragma optimize("", off)
#endif
void MinPassageSolve::IndexDistanceSort(vector<ValidDist>&vIdDis)
{
    if(vIdDis.empty())
        return;
    for (int i = 0; i < vIdDis.size(); ++i) {
        for (int j = 0; j < vIdDis.size() - i - 1; ++j) {
            if (vIdDis[j]._indexDist.second > vIdDis[j + 1]._indexDist.second) {
                ValidDist t = vIdDis[j];
                ///std::pair<int,double> t = vIdDis[j];
                vIdDis[j] = vIdDis[j + 1];
                vIdDis[j + 1] = t;
            }
        }
    }
}
void IsMeetDistance(const DefElem& e1,const DefElem&e2,
                    double up,double dow);
bool MinPassageSolve::IsValidExploreUnprocess(const DefLoop &loop,
                                              const int index,
                                              const double segStep,
                                              const double arcStep,
                                              const int id,
                                              const double nodeDis,
                                              const double Distance)
{
    vector<Point>vPoint;
    GeomCalculate gocal;GeomDistance godis;
    DefElem ele = loop.IndexElem(index),ele_ = loop.IndexElem(id);
    gocal.TranslateElemToPoint(ele_,true,segStep,arcStep,vPoint);
    if(vPoint.empty())
        return false;
    int minId = 0;
    double dis = 1000000;
    ///godis.ElementToElementDistance(ele,ele_,nodeDis);
    for (int i = 0; i < vPoint.size(); ++i) {
        double dis_ = 0;
        godis.PointToElementDistance(vPoint[i],ele,dis_);
        if(dis_ < dis){
            dis = dis_;
            minId = i;
        }
    }
    Point closePt;
    ClosestPoint clo;
    clo.PointToElementClosestPoint(vPoint[minId],ele,closePt);
    auto ValidDistance = [&gocal,&godis](const Point &closePt,
            const DefElem &ele_,double segStep,double arcStep,
            double Distance,double nodeDis)->bool{
        vector<Point>vPoint;
        gocal.TranslateElemToPoint(ele_,true,segStep,arcStep,vPoint);
        for (int i = 0; i < vPoint.size(); ++i) {
            double d = 0;
            ///godis.PointToElementDistance(vPoint[i],ele,d);            d = vPoint[i].Distance2D(closePt);
            if(d - nodeDis >= Distance)
                return true;
        }
        return false;
    };
    bool isFontValue = false,isBackValue = false;
    for (int i = minId; i >= 0; --i) {
        double dis_ = 0;
        ///需要保证dis_>nodeDis.
        ///godis.PointToElementDistance(vPoint[i],closePt,dis_);
        dis_ = vPoint[i].Distance2D(closePt);
        if(dis_ - nodeDis >= Distance){
            isFontValue = true;
            break;
        }
    }
    if(!isFontValue){
        int i = loop.PreviousIndex(id);
        while (i != id && i != index) {
            if(ValidDistance(closePt,loop.IndexElem(i),segStep,
                             arcStep,Distance,nodeDis)){
                isFontValue = true;
                break;
            }
            i = loop.PreviousIndex(i);
        }
    }
    if(!isFontValue)
        return false;
    for (int i = minId; i < vPoint.size(); ++i) {
        double dis_ = 0;
        ///需要保证dis_>nodeDis.
        ///godis.PointToElementDistance(vPoint[i],closePt,dis_);
        dis_ = vPoint[i].Distance2D(closePt);
        if(dis_ - nodeDis >= Distance){
            isBackValue = true;
            break;
        }
    }
    if(!isBackValue){
        int i = loop.NextIndex(id);
        while (i != id && i != index) {
            if(ValidDistance(closePt,loop.IndexElem(i),segStep,
                             arcStep,Distance,nodeDis)){
                isBackValue = true;
                break;
            }
            i = loop.NextIndex(i);
        }
    }
    if(!isBackValue)
        return false;
    return true;
}
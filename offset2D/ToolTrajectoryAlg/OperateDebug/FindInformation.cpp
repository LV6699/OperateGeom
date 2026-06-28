using namespace std;
#include<iostream>
#include "FindInformation.h"
bool FindInformation::IsPointInLoop(const DefLoop& loop,double err,
                                    bool isBorder,int index,const Point& p)
{
    ElemCalculate elecal;OperateOftString oftstr;
    for (int i = 0; i < loop.Size(); ++i) {
        Point sp = elecal.ElementEndpoint(loop.IndexElem(i),false),
                ep = elecal.ElementEndpoint(loop.IndexElem(i),true);
        if(p.IsSamePoint3D(sp,err) ||
                p.IsSamePoint3D(ep,err)){
            std::string str = oftstr.LoopElementString(loop,isBorder,index);
            std::cout << str << std::endl;
            return true;
        }
    }
    return false;
}
bool FindInformation::IsPointInArrayLoop(const std::vector<DefLoop>& vLoop,
                                         double err,const Point& p)
{
    for (int i = 0; i < vLoop.size(); ++i) {
        if(IsPointInLoop(vLoop[i],err,false,i,p)){
            std::string str = OperateOftString().ArrayLoopString(vLoop);
            std::cout << str << std::endl;
            return true;
        }
    }
    return false;
}
bool FindInformation::IsPointInPathNode(const std::shared_ptr<OffsetNode>& pathNode,
                                        double err,const Point& p)
{
    if(IsPointInArrayLoop(pathNode->Loops(),err,p)){
        int n = pathNode->Loops().size();
        std::cout << n << std::endl;
        return true;
    }
    for (int i = 0; i < pathNode->Nodes().size(); ++i) {
        if(IsPointInPathNode(pathNode->IndexAreaNode(i),err,p)){
            int n = pathNode->Nodes().size();
            std::cout << n << std::endl;
            return true;
        }
    }
    return false;
}
bool FindInformation::IsPointInGeomArea(const GeomArea& area,
                                        double err,const Point& p)
{
    if(IsPointInLoop(area.BndLoop(),err,true,0,p)){
        int n = area.BndLoop().Size();
        std::cout << n << std::endl;
        return true;
    }
    if(IsPointInArrayLoop(area.IslLoop(),err,p)){
        int n = area.IslLoop().size();
        std::cout << n << std::endl;
        return true;
    }
    return false;
}
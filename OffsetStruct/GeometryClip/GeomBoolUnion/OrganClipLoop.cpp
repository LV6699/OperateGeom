#include "OrganClipLoop.h"
bool OrganClipLoop::IsTerminateSearch(const Point& firstPt,const Point&curSp,
                                      vector<vector<ClipIndex>>&vArrIndex,
                                      vector<ClipIndex>&vIndex)
{
    if(GeomCalculate().IsIdenticalPoint(firstPt,curSp,PreErr5_8)){
        if(!vIndex.empty()){
            vArrIndex.push_back(vIndex);
            vIndex.clear();
        }
        return true;
    }
    return false;
}
void OrganClipLoop::SpecifyConnectElement(const vector<DefLoop>& vLoop,
                                          const Point& preEp,const int 
curLoIndex,
                                          bool& isFound,int& loopIndex,
                                          int & eleIndex)
{
    isFound = false;
    GeomCalculate gocal;
    ElemCalculate numcal;
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        if(lo == curLoIndex || !vLoop[lo]._loopLabel._loopIntersect){
            continue;
        }
        for (int i = 0; i < vLoop[lo].ElemNum(); ++i) {
            DefElem ele = vLoop[lo].IndexElem(i);
            if(ele._elemLabel._eleHasMer){
                continue;
            }
            Point curSp = numcal.ElementEndpoint(ele,false);
            if(!gocal.IsIdenticalPoint2D(preEp,curSp,PreErr5_8)){
                continue;
            }
            eleIndex = i;
            loopIndex = lo;
            isFound = true;
            return;
        }
    }
}
void OrganClipLoop::LoopClipIndexElement(const vector<DefLoop>& vLoop,
                                         const 
vector<vector<ClipIndex>>&vArrIndex,
                                         vector<DefLoop>& vResLoop)
{
    if(vLoop.empty()){
        return;
    }
    ElemCalculate numcal;
    int hasOftNum = vLoop[0].PastOffsetNum();
    for (int i = 0; i < vArrIndex.size(); ++i) {
        if(vArrIndex[i].empty())
            continue;
        DefLoop loop;
        bool isExistClip = false;        for(int lo = 0;lo < vArrIndex[i].size();lo ++){
            int nex = numcal.NextIndex(vArrIndex[i].size(),lo);
            if(vArrIndex[i][lo].loopIndex != vArrIndex[i][nex].loopIndex)
                isExistClip = true;
        }
        for (int j = 0; j < vArrIndex[i].size(); ++j) {
            int i_ = vArrIndex[i][j].loopIndex;
            int j_ = vArrIndex[i][j].eleIndex;
            if(!numcal.IsInArrayRange(vLoop.size(),i_) ||
                    !numcal.IsInArrayRange(vLoop[i_].m_vElem.size(),j_)){
                continue;
            }
            DefElem ele = vLoop[i_].IndexElem(j_);
            loop.PushBackElem(ele);
        }
        loop._loopLabel._loopIntersect = isExistClip;
        loop.SetPastOftNum(hasOftNum);
        vResLoop.push_back(loop);
    }
}
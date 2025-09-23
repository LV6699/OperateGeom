#include "BndIslMatch.h"
#include<iostream>

#if DebugModel
#pragma optimize("", off)
#endif

void BndIslMatch::LoopsMatchLoopsToAreas(const vector<DefLoop>&vBndLoop,
                                          const vector<DefLoop>&vIslLoop,
                                          vector<DefLoop>& vNotIncLoop,
                                          vector<GeomArea>& vBILoop)
{
#if 0
    vector<DefLoop>vLoop = vBndLoop;
    for(auto& d : vIslLoop){vLoop.emplace_back(d);}
    string str = OperateOftString().ArrayLoopString(vLoop);
    std::cout <<str<<endl;
#endif
    GeomContain gocon;
    for(int i = 0;i < vBndLoop.size();i++){
        vector<DefLoop>vResLoop;
        for (int lo = 0; lo < vIslLoop.size(); ++lo) {
            if(vIslLoop[lo].IsEmpty())
                continue;
            bool resValue = false;
            gocon.IsLoopContainIntersectLoop(vBndLoop[i],vIslLoop[lo],
                                             PreErr_5,PreErr_5,PreErr_5,
                                             false,resValue);
            ///resValue = true;
            if(resValue){
                vResLoop.push_back(vIslLoop[lo]);
            }
        }
        if(vResLoop.size()){
            GeomArea data(vBndLoop[i],vResLoop);
            vBILoop.push_back(data);
        }
        else{
            vNotIncLoop.push_back(vBndLoop[i]);
        }
    }
}

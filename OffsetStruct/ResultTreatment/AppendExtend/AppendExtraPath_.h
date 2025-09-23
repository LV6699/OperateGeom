#include "AppendExtraPath.h"
void AppendExtraPath::CreateAppendExtraPath(double radius,double offsetVal,
                                            std::shared_ptr<OffsetNode> 
&pathNode)
{
    if(offsetVal - PreErr_2 <= radius)
        return;
    ExicuteAppendExtraPath(radius,offsetVal,pathNode);
}
void AppendExtraPath::ExicuteAppendExtraPath(double radius, double offsetVal,
                                             std::shared_ptr<OffsetNode> 
&pathNode)
{
    if(pathNode == nullptr)
        return;
    for (int i = 0; i < pathNode->_vLoopData.size(); ++i) {
        LoopAppendExtraPath(radius,offsetVal,pathNode->_vLoopData[i]);
    }
    for (int i = 0; i < pathNode->_vOffsetNode.size(); ++i) {
        ExicuteAppendExtraPath(radius,offsetVal,pathNode->_vOffsetNode[i]);
    }
}
void AppendExtraPath::LoopAppendExtraPath(double radius,double offsetVal,
                                          DefLoop &loop)
{
    if(loop.IsCloseMode())
        return;
    AppExtraPathSolve sol;
    vector<DefElem>vElem;
    DefLoop loop_ = loop;
    loop_.ClearElem();
    for (int i = 0; i < loop.ElemNum(); ++i) {
        if(i == 19){
            int tem = 1;
        }
        sol.AdjoinElementAppendPath(loop,i,radius,offsetVal,vElem);
        loop_.PushBackElem(loop.IndexElem(i));
        for(auto &d : vElem){
            loop_.PushBackElem(d);
        }
    }
    loop = loop_;
}
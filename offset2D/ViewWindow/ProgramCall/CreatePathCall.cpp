#include "CreatePathCall.h"
shared_ptr<ProIOInterface> calltp_io = make_shared<ProIOInterface>();
shared_ptr<GeomCalculate>calltp_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate> calltp_num = make_shared<ElemCalculate>();
void CreatePathCall::ExicuteToolPathCalculate(BndIslPair pairLoop,
                                              OffsetParam offsetPar,
                                              shared_ptr<OffsetNode>&rootNode)
{
    if(offsetPar._offsetUnit.FirstOftVal() < PreErr5_2){
        offsetPar._offsetUnit.SetFirstOffsetValue(2);
    }
    if(offsetPar._offsetUnit.OffsetValue() < PreErr5_2){
        offsetPar._offsetUnit.SetOffsetValue(2);
    }
    for(auto& it : pairLoop.m_vIslLoop){
        it.SetLeftward(true);
    }
    ///offsetPar._offsetUnit.IsPathSmooth() = false;
    ///offsetPar._offsetUnit.PathSmoothValue() = offsetPar._offsetUnit.OffsetValue() * 0.05;
    bool isPrintInfo = false;
    if(isPrintInfo){
        OperateOftString().PrintLoopData(pairLoop.m_bndLoop,true,0);
        for(auto& it : pairLoop.m_vIslLoop){
            OperateOftString().PrintLoopData(it,true,0);
        }
    }
    ///double toolRadius = (offsetPar._offsetUnit.OffsetValue() / OftValDiamMultiplier) / 2;
    ///offsetPar.SetToolRadius(toolRadius);
    ///offsetPar._offsetUnit.FirstOffsetValue() = toolRadius;
    ///offsetPar._offsetUnit.IsPathSmooth() = false;
    GeomArea bndIslLoop(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
    try{
        calltp_io->InPutOutPutIO(bndIslLoop,offsetPar,rootNode);
    }
    catch (const std::exception& e) {
        std::cout <<  e.what() << std::endl;
    }
    catch(string e1){
        std::cout << "Exception" << std::endl;
    }
    catch (const char* e2){
        std::cout << "Exception" << std::endl;
    }
    catch (...){
       std::cout << "Exception" << std::endl;
    }
  
}
void CreatePathCall::CallCreateToolPath(BndIslPair &pairLoop,
                                        OffsetParam offsetPar,
                                        shared_ptr<OffsetNode>&rootNode)
{
    if(rootNode == nullptr){
        rootNode = make_shared<OffsetNode>();
    }
    if(offsetPar._pathTech.McStrategy() == ModelAreaClearing){
        ///oriRootNode->_loops = pairLoop.m_vIslLoop;
        ///oriRootNode->_loops.push_back(pairLoop.m_bndLoop);
        ExicuteToolPathCalculate(pairLoop,offsetPar,rootNode);
    }
    else if(offsetPar._pathTech.McStrategy() == ModelContour){
        ///oriRootNode->_loops = pairLoop.m_vIslLoop;
        ///oriRootNode->_loops.push_back(pairLoop.m_bndLoop);
        ExicuteToolPathCalculate(pairLoop,offsetPar,rootNode);
    }
    else{
        DefLoop offsetLoop = pairLoop.m_bndLoop;
        if(!pairLoop.m_vIslLoop.empty()){
            offsetLoop = pairLoop.m_vIslLoop[0];
            for(auto& lo : pairLoop.m_vIslLoop){
                if(int(lo.m_vElem.size()) > int(offsetLoop.m_vElem.size())){
                    offsetLoop = lo;
                }
            }
            if(int(pairLoop.m_bndLoop.m_vElem.size()) >
                    int(offsetLoop.m_vElem.size())){
                offsetLoop = pairLoop.m_bndLoop;
            }
        }
        pairLoop.m_vIslLoop.clear();
        pairLoop.m_bndLoop = offsetLoop;
        ///oriRootNode->_loops.push_back(offsetLoop);
        BndIslPair pairLoop_(pairLoop.m_bndLoop,pairLoop.m_vIslLoop);
        shared_ptr<OffsetNode>rootNode_ = make_shared<OffsetNode>();
        pairLoop.m_bndLoop.SetLeftward(false);
        ExicuteToolPathCalculate(pairLoop,offsetPar,rootNode_);
        if(!rootNode_->_loops.empty() ||
                !rootNode_->_nodes.empty()){
            rootNode->_nodes.push_back(rootNode_);
        }
        shared_ptr<OffsetNode>rootNode_1 = make_shared<OffsetNode>();
        pairLoop_.m_bndLoop.SetLeftward(true);
        ExicuteToolPathCalculate(pairLoop_,offsetPar,rootNode_1);
        if(!rootNode_1->_loops.empty() ||
                !rootNode_1->_nodes.empty()){
            rootNode->_nodes.push_back(rootNode_1);
        }
    }
}
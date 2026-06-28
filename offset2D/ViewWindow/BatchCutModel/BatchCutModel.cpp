#include "BatchCutModel.h"
shared_ptr<GeomCalculate>batcut_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>batcut_num = make_shared<ElemCalculate>();
shared_ptr<BatchCutSolve>batcut_batsol = make_shared<BatchCutSolve>();
void BatchCutModel::GetContourMaxWeightLoop(const vector<vector<BndIslPair>>&vAreLayLoop,
                                            BndIslPair& bndIslLoop)
{
    if(vAreLayLoop.empty()){
        return;
    }
    bndIslLoop = vAreLayLoop[0][0];
    for (int i = 0; i < vAreLayLoop.size(); ++i) {
        for (int j = 0; j < vAreLayLoop[i].size(); ++j) {
            if(bndIslLoop.m_weight < vAreLayLoop[i][j].m_weight){
                bndIslLoop = vAreLayLoop[i][j];
            }
        }
    }
}
void BatchCutModel::GetFixedNumMaxWeightLoop(const vector<vector<BndIslPair>>&vAreLayLoop,
                                             int num,vector<BndIslPair>&vBndIslLoop)
{
    vector<double>vWeight;
    vector<BndIslPair>vBILoop;
    for (int i = 0; i < vAreLayLoop.size(); ++i) {
        for (int j = 0; j < vAreLayLoop[i].size(); ++j) {
            vBILoop.push_back(vAreLayLoop[i][j]);
            vWeight.push_back(vAreLayLoop[i][j].m_weight);
        }
    }
    int num_ = 0;
    std::sort(vWeight.begin(),vWeight.end());
    vWeight.erase(unique(vWeight.begin(),vWeight.end()),vWeight.end());
    for (int i = 0; i < vWeight.size(); ++i) {
        for (int lo = 0; lo < vBILoop.size(); ++lo) {
            batcut_batsol->SetBndIslMatchOffsetVal(vBILoop[lo]);
            if(!IsValidContourMatchLoop(vBndIslLoop,vBILoop[lo])){
                continue;
            }
            num_ ++;
            vBndIslLoop.push_back(vBILoop[lo]);
            if(num_ >= num){
                return;
            }
        }
    }
}
void BatchCutModel::GetSpecifyBndIslLoop(vector<vector<BndIslPair>>&vAreLayLoop,
                                         vector<BndIslPair>&vBILoop)
{
    if(vAreLayLoop.empty()){
        return;
    }
    for (int i = 0; i < vAreLayLoop.size(); ++i) {
        for (int j = 0; j < vAreLayLoop[i].size(); ++j) {
            DefLoop bndLoop = vAreLayLoop[i][j].m_bndLoop;
            vector<DefLoop>vIslLoop = vAreLayLoop[i][j].m_vIslLoop;/**
            batcut_elepre->InputPretreatment(bndLoop,vIslLoop,false,true,
                                             true,true,true);*/
            BndIslPair bndIslLoop(bndLoop,vIslLoop);
            batcut_batsol->GetBndIslContourWeight(bndIslLoop);
            vAreLayLoop[i][j] = bndIslLoop;
        }
    }
    int matchNum = 3;
    GetFixedNumMaxWeightLoop(vAreLayLoop,matchNum,vBILoop);
}
bool BatchCutModel::IsValidContourMatchLoop(const vector<BndIslPair>&vBndIslLoop,
                                            const BndIslPair& bndIslLoop)
{
    if(!bndIslLoop.m_isValScale){
        return false;
    }
    for(const auto& it : vBndIslLoop){
        if(batcut_batsol->IsMatchLoopSimilarProperty(it,bndIslLoop)){
            return false;
        }
        if(!it.m_isValScale){
            return false;
        }
    }
    return true;
}
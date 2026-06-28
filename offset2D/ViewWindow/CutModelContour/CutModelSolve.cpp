#include "CutModelSolve.h"
shared_ptr<GeomCalculate>cutsol_go = make_shared<GeomCalculate>();
shared_ptr<ElemCalculate>cutsol_num = make_shared<ElemCalculate>();
shared_ptr<DataEdit>cutsol_dat = make_shared<DataEdit>();
void CutModelSolve::SaveBatchCutContour(string saveDir,
                                        string modelName,
                                        vector<BndIslPair>&vBILoop,
                                        bool& isSuccess)
{
    isSuccess = true;
    for (int i = 0; i < vBILoop.size(); ++i) {
        bool isSuccess_ = false;
        if(i == 0)
            vBILoop[i].m_partName = modelName;
        else
            vBILoop[i].m_partName = modelName + "_" +
                    std::to_string(i);
        cutsol_dat->SaveCutPairLoopData(saveDir,vBILoop[i],isSuccess_);
        if(!isSuccess_){
            isSuccess = false;
        }
    }
}
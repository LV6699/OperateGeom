#ifndef CUTPARTS
#define CUTPARTS
#include"../HearderFile.h"
class CutParts
{
public:
    CutParts(){}
    void TwoArcConvertCircle(const Point& arcSp1,
                             const Point& arcEp1,
                             const Point& cirCdt1,
                             const Point& arcSp2,
                             const Point& arcEp2,
                             const Point& cirCdt2,
                             bool& isCircle,
                             DefCircle &cirData);
    void HandleCircleSameCdt(vector<DefLoop>&vIslLoop);
    void CutPartIntoLoopData(const string &file,
                             double xExp, double yExp,
                             const int &layTotal,
                             bool& isCutSuccess,
                             vector<vector<BndIslPair> > &vAreaLayData);
    void CutPartNameIntoData(double xExp, double yExp, CutPartPara cutPara,
                             CutPartResult& cutResultData);
    void CutSingleModelFile(CutModelInfo modelInfo, bool &isCutSuccess,
                            vector<vector<BndIslPair> > &vAreLayData);
    void BatchCutModelFile(const vector<CutModelInfo> &vModelFile,
                           vector<BndIslPair>& vModelLoop);
    void CreateBlanck(string path, double lenExp, double widExp,
                      TopoDS_Shape &blankShape);
};
#endif // CUTPARTS
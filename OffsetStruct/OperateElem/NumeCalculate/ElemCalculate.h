#ifndef NUMERICALCAL_H
#define NUMERICALCAL_H
#include"../../CommonFile/Header.h"
#include <string>
class CAMALGORITHM_EXPORT ElemCalculate
{
public:
    ElemCalculate(){}
    ///len是总长度
    int NextIndex(int len,int i)
    {
        if(len <= 0)
            return 0;
        if(i >= len - 1)
            return 0;
        return i + 1;
    }
    ///len是总长度
    int PreviousIndex(int len,int i)
    {
        if(len <= 0)
            return 0;
        if(i <= 0)
            return len - 1;
        return i - 1;
    }
    bool IsInArrayRange(int len,int i)
    {
        if(len < 0 || i < 0 || i > len - 1)
            return false;
        return true;
    }
    bool IsLoopExistPreEle(const DefLoop&loop,int i)
    {
        if(loop.m_vElem.empty())
            return false;
        if(!loop.IsOpenLoop() || i != 0)
            return true;
        return false;
    }
    bool IsLoopExistNexEle(const DefLoop&loop,int i)
    {
        if(loop.m_vElem.empty())
            return false;
        if(!loop.IsOpenLoop() || i != loop.m_vElem.size() - 1)
            return true;
        return false;
    }
    void DeduplicationPoint(vector<Point>& vPoint,double pre);
    void DeduplicationLoop(vector<DefLoop>& vLoop);
    void ResetLoopParameters(const DefLoop &oriLoop,
                             DefLoop &resLoop);
    void ResetLoopEleParameters(const DefLoop &oriLoop,
                                bool isResetLoop,                                DefLoop &loop);
    void ResetLoopEleParameters(DefLoop &loop,
                                bool isSetLoop);
    double LoopZCoordinate(const DefLoop& loop);
    void ResetMultiLoopEleParam(vector<DefLoop>&vLoop,
                                bool isSetLoop);
    void DeleteArrayEmptyLoop(vector<DefLoop>&vLoop);
    void DeleteIllegalEleNumLoop(vector<DefLoop>&vLoop);
    bool IsElementIndexValid(const DefLoop&loop,
                             int i);
    bool IsLoopInArrayLoop(const vector<DefLoop>& vLoop,
                           const DefLoop& loop);
    bool IsEleExistValInt(const DefElem& ele1,
                          const DefElem& ele2,
                          const GeomIntRes &intRes);
    void LoopGeometryLenth(const DefLoop& loop, double &lenth);
    void ArrayLoopMaxLenthLoop(const vector<DefLoop>&vLoop,
                               bool isMaxLenth,DefLoop& loop);
    void GetNodeLoopNum(const shared_ptr<OffsetNode>&rootNode,int& num);
    void GetNodeEleNum(const shared_ptr<OffsetNode>&rootNode,int& num);
    void GetNodeEleLenth(const shared_ptr<OffsetNode>&rootNode,double& len);
    bool IsValidPoint(const Point &P,bool is2D = true);
    void GetArcRadius(const DefArc &arc,double& radius);
    void IsLegalCurveNumLoop(const DefLoop &loop, bool& isLegNum);
    bool IsLegalCurveNumber(const DefLoop& loop);
    void ExceptIndexLoop(const vector<DefLoop>&vOriLoop,
                         int index,DefLoop &loop,
                         vector<DefLoop>&vLoop);
    void ExceptIndexLoop(const vector<DefLoop>&vOriLoop,
                         int index,vector<DefLoop>&vLoop);
    void InitialArcRadius(DefArc &arc);
    void InitialLoopArcRadius(DefLoop &loop);
    bool IsTwoElemIdentical(const DefElem &ele1,
                            const DefElem &ele2,double pre);
    bool IsIdenticalElement(const DefElem &ele1,
                            const DefElem &ele2,
                            double pre);
    void ProfTypeSwhichString(ElemType type,std::string& str);
    void DeleteIndexLoop(const vector<int>& vIndex, vector<DefLoop>& vLoop);    void DeleteIndexElement(const vector<int>& vIndex,vector<DefElem>& vElem);
    void SetLoopElementZValue(const double& zValue,DefLoop& loop);
    void SetArrayLoopElementZValue(const double &zValue,
                                   vector<DefLoop> &vloop);
    void ElementEndpoint(const DefElem &ele,bool isEp,Point& P);
    Point ElementEndpoint(const DefElem &ele,bool isEp);
    void SetElemEndpointZCoord(bool isEp,double Z,
                               DefElem &ele );
    void PointIsElemEndpoint(const DefElem &ele,
                             const Point& p,
                             double e,
                             bool& isEndpoint,
                             bool& isSp);
    void PointIsEleExtremity(const DefElem &ele,
                             const Point& p,
                             double pre,
                             bool& isExtr);
    void IsElementEndpoint(const DefElem &ele,
                           const Point& p,
                           double pre,
                           bool isJudgeSp,
                           bool& is);
    bool IsElementEndpoint(const DefElem &ele,
                           const Point& p,
                           double pre,
                           bool isJudgeSp);
    bool IsElementEndpoint(const DefElem &ele,
                           const Point& p,
                           double pre);
    void SetElementEndpoint(const Point& P,
                            bool isChEp,
                            DefElem& ele);
    void ReverseSegmentDirection(Point& lSp,Point& lEp);
    void ReverseArcDirection(DefArc& arc);
    void ReverseEleDirection(DefElem& ele);
    void ReverseLoopElemEndPnt(DefElem& eleData);
    void ReverseLoopDirection(DefLoop &loop);
    void ReorederLoopElement(int startIndex,bool isOrder,
                             bool isAddIndex,DefLoop &loop);
    void LoopEndpoint(const DefLoop &loop,
                      bool isLastPt, Point& P);
    Point LoopEndpoint(const DefLoop &loop,bool isLastPt);
    void PointSplitElement(const DefElem& ele,                           const Point& P,
                           bool isChEndPnt,
                           DefElem& newEle);
    void TwoEleIsPossessSamePnt(const DefElem& ele1,
                                const DefElem& ele2,
                                bool &isSame);
    void ElementMidpoint(const DefElem& ele,Point& mp);
    void JudgeEleValidity(const DefElem& ele,bool& isVal);
    void JudgeEleValidity(const DefElem& ele,double sigLim,
                          double arcLim,bool& isVal);
    void ElementValidity(const DefElem& ele,double segLim,
                         double arcLim,bool& isVal);
    bool IsValidGeomArc(DefArc arc, double endVal,
                        double R, double H, bool isDecNum, int number);
    void IsValidGeomeElement(const DefElem& ele, double segPre,
                             double arcPre, double H, double R, bool& isVal);
    bool IsValidGeomeElement(const DefElem& ele,double segPre,
                             double arcPre,double H, double R);
    void ElementEndpointStepVal(const DefElem&ele,
                                double expeVal,
                                double& stepVal);
    bool IsPointInArrayPoint(const vector<Point>& vPoint,
                             const Point& P,
                             double errVal);
    bool IsIdenticalLoop(const DefLoop& loop1,
                         const DefLoop& loop2);
    void TranslateLoopToPoint(const DefLoop& loop,
                              double value,bool isBaseAngle,
                              vector<Point>&vPoint);
    void SetLoopOpenClose(DefLoop& loop);
    bool IsCloseLoop(const DefLoop& loop, double errVal);
    void ResetIndexCurrentLoop(const vector<DefLoop>& vLoop,
                               const DefLoop& curLoop,
                               int index,
                               vector<DefLoop>& vLoop_,
                               DefLoop& curLoop_);
    int LongestElementIndex(const DefLoop& loop);
    void ArrayPointAverage(const vector<Point>&v, Point &P);
    void ElemCoordDecimalPlaces(int number,DefElem &ele);
    void LoopPointBaseLenth(const DefLoop& loop, int i,
                            bool isOrder, double lenth, Point& p);
    void LoopPointBaseCurve(const DefLoop& loop,
                            const Point& p, int id,
                            bool isOrder, double lenth,Point& res);    DefCircle PointsMinCircumcircle(vector<Point>& points,
                                    vector<Point>& boundary,
                                    size_t id);
    void LoopMinCircumcircle(const DefLoop &loop, bool isAgl,
                             double err,DefCircle &circle);
};
#endif // NUMERICALCAL_H
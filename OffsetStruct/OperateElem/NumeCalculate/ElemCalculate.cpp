#include <tbb/tbb.h>
#include "ElemCalculate.h"
#if 0
#pragma optimize("", off)
#pragma GCC optimize ("O0")
#endif
#if _ViewDebug
#include"../../OperateDebug/DisplayGeom.h"
#endif
void ElemCalculate::DeduplicationPoint(vector<Point>& vPoint,double pre)
{
    int i = 0;
    GeomCalculate gocal;
    while (i < vPoint.size()) {
        int number = 0;
        for (int j = 0; j < vPoint.size(); ++j) {
            bool isSame = false;
            gocal.IsIdenticalPoint(vPoint[i],vPoint[j],pre,isSame);
            if(isSame){
                number ++;
            }
            if(number >= 2)
                break;
        }
        if(number >= 2){
            vPoint.erase(vPoint.begin() + i);
            continue;
        }
        i ++;
    }
}
void ElemCalculate::DeduplicationLoop(vector<DefLoop>& vLoop)
{
    int i = 0;
    while (i < vLoop.size()) {
        DefLoop loop = vLoop[i];
        vector<DefLoop>vLoop_;
        ExceptIndexLoop(vLoop,i,vLoop_);
        if(IsLoopInArrayLoop(vLoop_,loop)){
            vLoop.erase(vLoop.begin() + i);
            continue;
        }
        i ++;
    }
}
void ElemCalculate::ResetLoopParameters(const DefLoop &oriLoop,
                                        DefLoop &resLoop)
{
    resLoop = DefLoop();
    resLoop.SetLoopElem(oriLoop.LoopElem());
}
void ElemCalculate::ResetLoopEleParameters(const DefLoop &oriLoop,
                                           bool isResetLoop,
                                           DefLoop &loop)
{
    DefLoop loop_ = oriLoop;
    loop_.m_vElem.clear();    for (int i = 0; i < oriLoop.ElemNum(); ++i) {
        DefElem ele;
        if(oriLoop.IndexElem(i).ElemeType() == LINETYPE)
            ele._segment = oriLoop.IndexElem(i)._segment;
        else if(oriLoop.IndexElem(i).ElemeType() == ARCTYPE)
            ele._arc = oriLoop.IndexElem(i)._arc;
        else if(oriLoop.IndexElem(i).ElemeType() == CIRCLETYPE)
            ele._circle = oriLoop.IndexElem(i)._circle;
        ele.SetElemType(oriLoop.IndexElem(i).ElemeType());
        loop_.PushBackElem(ele);
    }
    if(isResetLoop)
        ResetLoopParameters(oriLoop,loop_);
    loop = loop_;
}
void ElemCalculate::ResetLoopEleParameters(DefLoop &loop,
                                           bool isSetLoop)
{
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        DefElem ele;
        if(loop.m_vElem[i]._elemType == LINETYPE)
            ele.SetElemSeg(loop.IndexElem(i).ElemSeg());
        else if(loop.m_vElem[i]._elemType == ARCTYPE)
            ele.SetElemArc(loop.IndexElem(i).ElemArc());
        else if(loop.m_vElem[i]._elemType == CIRCLETYPE)
            ele.SetElemCircle(loop.IndexElem(i).ElemCircle());
        ele.SetElemType(loop.IndexElem(i).ElemeType());
        loop.SetIndexElem(ele,i);
    }
    if(isSetLoop){
        DefLoop loop_(loop.LoopElem());
        loop = loop_;
    }
}
double ElemCalculate::LoopZCoordinate(const DefLoop& loop)
{
    if(loop.IsEmpty())
        return 0;
    if(loop.IndexElem(0).ElemeType() == LINETYPE)
        return loop.IndexElem(0).ElemSeg().SegStartPt().Z();
    else if(loop.IndexElem(0).ElemeType() == ARCTYPE)
        return loop.IndexElem(0).ElemArc().ArcCenterPt().Z();
    else if(loop.IndexElem(0).ElemeType() == CIRCLETYPE)
        return loop.IndexElem(0).ElemCircle().CircleCenter().Z();
    return 0;
}
void ElemCalculate::ResetMultiLoopEleParam(vector<DefLoop>&vLoop,
                                           bool isSetLoop)
{
    for (int i = 0; i < vLoop.size(); ++i)
        ResetLoopEleParameters(vLoop[i],isSetLoop);
}
void ElemCalculate::DeleteArrayEmptyLoop(vector<DefLoop>&vLoop)
{
    int lo = 0;
    while (lo < vLoop.size()) {
        if(vLoop[lo].m_vElem.empty()){
            vLoop.erase(vLoop.begin() + lo);
            continue;
        }        lo ++;
    }
}
void ElemCalculate::DeleteIllegalEleNumLoop(vector<DefLoop>&vLoop)
{
    int lo = 0;
    while (lo < vLoop.size()) {
        bool isLegal = false;
        IsLegalCurveNumLoop(vLoop[lo],isLegal);
        if(isLegal){
            lo ++;
            continue;
        }
        vLoop.erase(vLoop.begin() + lo);
    }
}
bool ElemCalculate::IsElementIndexValid(const DefLoop&loop,int i)
{
    if(loop.m_vElem.empty() || i < 0 || i > loop.m_vElem.size() - 1)
        return false;
    return true;
}
bool ElemCalculate::IsLoopInArrayLoop(const vector<DefLoop>& vLoop,
                                      const DefLoop& loop)
{
    for (int lo = 0; lo < vLoop.size(); ++lo) {
        if((vLoop[lo].ElemNum() != loop.ElemNum()) ||
                loop.IsEmpty())
            continue;
        bool isSame1 = true,isSame2 = true,isSame3 = true,
                isSame4 = true,isSame5 = true,isSame6 = true;
        int len = loop.ElemNum();
        if(len >= 1)
            isSame1 = IsIdenticalElement(vLoop[lo].IndexElem(0),
                                         loop.IndexElem(0),
                                         PreErr5_10);
        if(isSame1 && (len >= 2))
            isSame2 = IsIdenticalElement(vLoop[lo].IndexElem(1),
                                         loop.IndexElem(1),
                                         PreErr5_10);
        if(isSame2 && (len >= 3))
            isSame3 = IsIdenticalElement(vLoop[lo].IndexElem(len/2),
                                         loop.IndexElem(len/2),
                                         PreErr5_10);
        if(isSame3 && (len >= 4))
            isSame4 = IsIdenticalElement(vLoop[lo].IndexElem((len/2)+1),
                                         loop.IndexElem((len/2)+1),
                                         PreErr5_10);
        if(isSame4 && (len >= 5))
            isSame5 = IsIdenticalElement(vLoop[lo].IndexElem(len-1),
                                         loop.IndexElem(len-1),
                                         PreErr5_10);
        if(isSame5 && (len >= 6))
            isSame6 = IsIdenticalElement(vLoop[lo].IndexElem(len-2),
                                         loop.IndexElem(len-2),
                                         PreErr5_10);
        if(isSame1 && isSame2 && isSame3 &&
                isSame4 && isSame5 && isSame6)
            return true;
    }    return false;
}
bool ElemCalculate::IsEleExistValInt(const DefElem& ele1,
                                     const DefElem& ele2,
                                     const GeomIntRes& intRes)
{
    if(intRes._isExistInt == false)
        return false;
    if(intRes._isExiGeoInt == false &&
            ele1._elemType == LINETYPE &&
            ele2._elemType == LINETYPE )
        return false;
    return true;
}
void ElemCalculate::LoopGeometryLenth(const DefLoop& loop,double& lenth)
{
    lenth = 0;
    GeomCalculate gocal;
#if 0
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        double len = 0;
        if(loop.m_vElem[i]._elemType == LINETYPE){
            gocal.TwoPointDistance(loop.m_vElem[i]._segment._segSp,
                                   loop.m_vElem[i]._segment._segEp,len);
        }
        else if(loop.m_vElem[i]._elemType == ARCTYPE){
            gocal.CalculateArcLength(loop.m_vElem[i]._arc,len);
        }
        else if(loop.m_vElem[i]._elemType == CIRCLETYPE){
            len = loop.m_vElem[i]._circle._cirR * 2 * M_PI;
        }
        lenth += len;
    }
#else
    tbb::parallel_for(tbb::blocked_range<std::size_t>(0, loop.ElemNum()),
                      [&](tbb::blocked_range<std::size_t>& r) {
        for (std::size_t i = r.begin(); i < r.end(); ++i) {
            double len = 0;
            if(loop.m_vElem[i]._elemType == LINETYPE){
                gocal.TwoPointDistance(loop.m_vElem[i]._segment._segSp,
                                       loop.m_vElem[i]._segment._segEp,len);
            }
            else if(loop.m_vElem[i]._elemType == ARCTYPE){
                gocal.CalculateArcLength(loop.m_vElem[i]._arc,len);
            }
            else if(loop.m_vElem[i]._elemType == CIRCLETYPE){
                len = loop.m_vElem[i]._circle._cirR * 2 * M_PI;
            }
            lenth += len;
        }
    });
#endif
}
void ElemCalculate::ArrayLoopMaxLenthLoop(const vector<DefLoop>&vLoop,
                                          bool isMaxLenth,DefLoop& loop)
{
    if(vLoop.empty())
        return;    vector<double>vLenth;
    for(auto& l : vLoop){
        double lenth = 0;
        LoopGeometryLenth(l,lenth);
        vLenth.push_back(lenth);
    }
    int index;
    if(isMaxLenth){
        auto it = std::max_element(vLenth.begin(),vLenth.end());
        index = std::distance(vLenth.begin(), it);
    }
    else{
        auto it = std::min_element(vLenth.begin(),vLenth.end());
        index = std::distance(vLenth.begin(),it);
    }
    loop = vLoop[index];
}
void ElemCalculate::GetNodeLoopNum(const shared_ptr<OffsetNode>&rootNode,
                                   int& num)
{
    if(rootNode == nullptr)
        return;
    num += rootNode->_vLoopData.size();
    for(int i = 0;i < rootNode->_vOffsetNode.size();i++){
        GetNodeLoopNum(rootNode->_vOffsetNode[i],num);
    }
}
void ElemCalculate::GetNodeEleNum(const shared_ptr<OffsetNode>&rootNode,
                                  int& num)
{
    if(rootNode == nullptr)
        return;
    for(int i = 0;i < rootNode->_vLoopData.size();i++){
        num += rootNode->_vLoopData[i].m_vElem.size();
    }
    for(int i = 0;i < rootNode->_vOffsetNode.size();i++){
        GetNodeEleNum(rootNode->_vOffsetNode[i],num);
    }
}
void ElemCalculate::GetNodeEleLenth(const shared_ptr<OffsetNode>&rootNode,
                                    double& lenth)
{
    if(rootNode == nullptr)
        return;
    for(int i = 0;i < rootNode->_vLoopData.size();i++){
        double len = 0;
        LoopGeometryLenth(rootNode->_vLoopData[i],len);
        lenth += len;
    }
    for(int i = 0;i < rootNode->_vOffsetNode.size();i++){
        GetNodeEleLenth(rootNode->_vOffsetNode[i],lenth);
    }
}
bool ElemCalculate::IsValidPoint(const Point& P,bool is2D)
{
    MathCalculate mat;
    if (is2D) {
        if (!mat.IsLegalValue(P.X()) || !mat.IsLegalValue(P.Y()))
            return false;
    } else {        if (!mat.IsLegalValue(P._x) || !mat.IsLegalValue(P._y) || !
mat.IsLegalValue(P.Z()))
            return false;
    }
    return true;
}
void ElemCalculate::GetArcRadius(const DefArc &arc,double& radius)
{
    double R1 = sqrt(std::pow(arc._arcSp._x - arc._arcCp._x, 2) +
                     std::pow(arc._arcSp._y - arc._arcCp._y, 2));
    double R2 = sqrt(std::pow(arc._arcEp._x - arc._arcCp._x, 2) +
                     std::pow(arc._arcEp._y - arc._arcCp._y, 2));
    radius = (R1 + R2) / 2;
}
void ElemCalculate::IsLegalCurveNumLoop(const DefLoop& loop,
                                        bool& isLegNum)
{
    isLegNum = true;
    if(loop.m_vElem.size() > 2 ||
            (!loop.m_vElem.empty() && loop.IsOpenLoop()))
        return;
    if(loop.m_vElem.size() == 1 &&
            loop.m_vElem[0]._elemType == CIRCLETYPE)
        return;
    bool isExiArc = false;
    for (auto &it : loop.m_vElem){
        if(it._elemType == ARCTYPE){
            isExiArc = true;
            break;
        }
    }
    if(isExiArc && loop.m_vElem.size() > 1)
        return;
    isLegNum = false;
}
bool ElemCalculate::IsLegalCurveNumber(const DefLoop& loop)
{
    bool isLegal = false;
    IsLegalCurveNumLoop(loop,isLegal);
    return isLegal;
}
void ElemCalculate::ExceptIndexLoop(const vector<DefLoop>&vOriLoop,
                                    int index,DefLoop &loop,
                                    vector<DefLoop>&vLoop)
{
    if(index < 0 || index > vOriLoop.size() - 1)
        return;
    vLoop.clear();
    loop = vOriLoop[index];
    for(int i = 0;i < vOriLoop.size();++i){
        if(i != index){
            vLoop.push_back(vOriLoop[i]);
        }
    }
}
void ElemCalculate::ExceptIndexLoop(const vector<DefLoop>&vOriLoop,
                                    int index,vector<DefLoop>&vLoop)
{
    vLoop.clear();    for (int i = 0; i < vOriLoop.size(); ++i) {
        if(i != index)
            vLoop.push_back(vOriLoop[i]);
    }
}
void ElemCalculate::InitialArcRadius(DefArc &arc)
{
    double r1 = std::sqrt(std::pow(arc._arcSp.X() -
                                   arc._arcCp.X(), 2) +
                          std::pow(arc._arcSp.Y() -
                                   arc._arcCp.Y(),2));
    double r2 = std::sqrt(std::pow(arc._arcEp.X() -
                                   arc._arcCp.X(),2) +
                          std::pow(arc._arcEp.Y() -
                                   arc._arcCp.Y(),2));
    double r = (r1 + r2) / 2;
    arc.SetArcRadius(r);
}
void ElemCalculate::InitialLoopArcRadius(DefLoop &loop)
{
    for(auto& it : loop.m_vElem){
        if(it.ElemeType() == ARCTYPE)
            InitialArcRadius(it._arc);
    }
}
bool ElemCalculate::IsTwoElemIdentical(const DefElem &ele1,
                                       const DefElem &ele2,
                                       double pre)
{
    bool isSame = IsIdenticalElement(ele1,ele2,pre);
    return isSame;
}
bool ElemCalculate::IsIdenticalElement(const DefElem &ele1,
                                       const DefElem &ele2,
                                       double pre)
{
    if(ele1.ElemeType() != ele2.ElemeType())
        return false;
    GeomCalculate gocal;
    if(ele1._elemType == LINETYPE){
        bool isSame1 = false,isSame2 = false;
        gocal.IsIdenticalPoint(ele1._segment._segSp,
                               ele2._segment._segSp,
                               pre,isSame1);
        if(!isSame1)
            return false;
        gocal.IsIdenticalPoint(ele1._segment._segEp,
                               ele2._segment._segEp,
                               pre,isSame2);
        if(!isSame2)
            return false;
        return true;
    }
    else if(ele1._elemType == ARCTYPE){
        if(ele1._arc._arcRot != ele2._arc._arcRot)
            return false;
        bool isSame1 = false,isSame2 = false,isSame3 = false;
        gocal.IsIdenticalPoint(ele1._arc._arcCp,
                               ele2._arc._arcCp,pre,isSame3);
        if(!isSame3)            return false;
        gocal.IsIdenticalPoint(ele1._arc._arcSp,
                               ele2._arc._arcSp,pre,isSame1);
        if(!isSame1)
            return false;
        gocal.IsIdenticalPoint(ele1._arc._arcEp,
                               ele2._arc._arcEp,pre,isSame2);
        if(!isSame2)
            return false;
        return true;
    }
    else if(ele1._elemType == CIRCLETYPE){
        bool isSame = false;
        gocal.IsIdenticalPoint(ele1._circle._cirCp,
                               ele2._circle._cirCp,pre,isSame);
        if(!isSame)
            return false;
        if(std::abs(ele1._circle._cirR -
                    ele2._circle._cirR) > pre)
            return false;
        return true;
    }
    return false;
}
void ElemCalculate::ProfTypeSwhichString(ElemType type,string& str)
{
    if(type == LINETYPE)
        str = "LINETYPE";
    else if(type == ARCTYPE)
        str = "ARCTYPE";
    else
        str = "CIRCLETYPE";
}
void ElemCalculate::DeleteIndexLoop(const vector<int>& vIndex,
                                    vector<DefLoop>& vLoop)
{
    if((vIndex.empty()) || (vLoop.empty()))
        return;/**
    std::sort(vIndex.begin(),vIndex.end());
    vIndex.erase(std::unique(vIndex.begin(), vIndex.end()),vIndex.end());
    if(vIndex[0] < 0 || vIndex.size() > vLoop.size()){
        return;
    }
    auto max = max_element(vIndex.begin(),vIndex.end());
    if(*max > vLoop.size() - 1){
        return;
    }
    int idxInc = 0;
    for(int i = 0;i < vIndex.size();++i){
        vLoop.erase(vLoop.begin() + vIndex[i] - idxInc);
        idxInc ++;
    }*/
    vector<DefLoop>vLoop_;
    MathCalculate matca;
    for (int i = 0; i < vLoop.size(); ++i) {
        if(!matca.IsValueIn2DArray(vIndex,i))
            vLoop_.push_back(vLoop[i]);
    }
    vLoop = vLoop_;
}
void ElemCalculate::DeleteIndexElement(const vector<int>&vIndex,                                       vector<DefElem>& vElem)
{
    if((vIndex.empty()) || (vElem.empty()))
        return;/**
    std::sort(vIndex.begin(),vIndex.end());
    vIndex.erase(std::unique(vIndex.begin(), vIndex.end()),vIndex.end());
    if(vIndex[0] < 0 || vIndex.size() > vElem.size())
        return;
    auto max = max_element(vIndex.begin(),vIndex.end());
    if(*max > vElem.size() - 1)
        return;
    int idxInc = 0;
    for(int i = 0;i < vIndex.size();++i){
        vElem.erase(vElem.begin() + vIndex[i] - idxInc);
        idxInc ++;
    }*/
    MathCalculate matca;
    vector<DefElem> vElem_;
    for(int i = 0;i < vElem.size();++i){
        if(!matca.IsValueIn2DArray(vIndex,i)){
            vElem_.push_back(vElem[i]);
        }
    }
    vElem = vElem_;
}
void ElemCalculate::SetLoopElementZValue(const double& zValue,
                                         DefLoop& loop)
{
    for(auto& it : loop.m_vElem){
        if(it._elemType == LINETYPE){
            it._segment._segSp.SetZ(zValue);
            it._segment._segEp.SetZ(zValue);
        }
        else if (it._elemType == ARCTYPE) {
            it._arc._arcSp.SetZ(zValue);
            it._arc._arcEp.SetZ(zValue);
            it._arc._arcCp.SetZ(zValue);
        }
        else if(it._elemType == CIRCLETYPE){
            it._circle._cirCp.SetZ(zValue);
        }
    }
}
void ElemCalculate::SetArrayLoopElementZValue(const double& zValue,
                                              vector<DefLoop>& vloop)
{
    for(auto &lo : vloop){
        SetLoopElementZValue(zValue,lo);
    }
}
void ElemCalculate::ElementEndpoint(const DefElem &ele,
                                    bool isEp,Point& P)
{
    switch (ele.ElemeType()) {
    case LINETYPE:{
        if(isEp)
            P = ele._segment.SegEndPt();
        else
            P = ele._segment.SegStartPt();
        break;
    }    case ARCTYPE:{
        if(isEp)
            P = ele._arc.ArcEndPt();
        else
            P = ele._arc.ArcStartPt();
        break;
    }
    case CIRCLETYPE:{
        P.SetX(ele._circle.CircleCenter().X() +
               ele._circle.CircleRadius());
        P.SetY(ele._circle.CircleCenter().Y());
        P.SetZ(ele._circle.CircleCenter().Z());
        break;
    }
    default:
        break;
    }
}
Point ElemCalculate::ElementEndpoint(const DefElem &ele,bool isEp)
{
    switch (ele.ElemeType()) {
    case LINETYPE:{
        if(isEp){
            return ele._segment.SegEndPt();
        }
        else{
            return ele._segment.SegStartPt();
        }
    }
    case ARCTYPE:{
        if(isEp){
            return ele._arc.ArcEndPt();
        }
        else{
            return ele._arc.ArcStartPt();
        }
    }
    case CIRCLETYPE:{
        return ele._circle.CircleCenter();
    }
    default:
        break;
    }
    return Point();
}
void ElemCalculate::SetElemEndpointZCoord(bool isEp,double Z,DefElem &ele )
{
    switch (ele.ElemeType()) {
    case LINETYPE:{
        if(isEp)
            ele._segment._segEp.SetZ(Z);
        else
            ele._segment._segSp.SetZ(Z);
        break;
    }
    case ARCTYPE:{
        if(isEp)
            ele._arc._arcEp.SetZ(Z);
        else
            ele._arc._arcSp.SetZ(Z);
        break;    }
    case CIRCLETYPE:{
        ele._circle._cirCp.SetZ(Z);
        break;
    }
    default:
        break;
    }
}
void ElemCalculate::PointIsElemEndpoint(const DefElem &ele,
                                        const Point& p,
                                        double e,
                                        bool& isEndpoint,
                                        bool& isSp)
{
    isEndpoint = false;
    Point sp,ep;
    ElementEndpoint(ele,false,sp);
    ElementEndpoint(ele,true,ep);
    bool isEp = false;
    isEp = p.IsSamePoint2D(ep,e);
    isSp = p.IsSamePoint2D(sp,e);
    if(isSp || isEp){
        isEndpoint = true;
    }
}
void ElemCalculate::PointIsEleExtremity(const DefElem &ele,
                                        const Point& p,
                                        double pre,
                                        bool& isExtr)
{
    isExtr = IsElementEndpoint(ele,p,pre);
}
void ElemCalculate::IsElementEndpoint(const DefElem &ele,
                                      const Point& p,
                                      double pre,
                                      bool isJudgeSp,
                                      bool& is)
{
    is = false;
    Point p_;
    if(isJudgeSp){
        ElementEndpoint(ele,false,p_);
    }else{
        ElementEndpoint(ele,true,p_);
    }
    is = p.IsSamePoint2D(p_,pre);
}
bool ElemCalculate::IsElementEndpoint(const DefElem &ele,
                                      const Point& p,
                                      double pre,
                                      bool isJudgeSp)
{
    bool is = false;
    IsElementEndpoint(ele,p,pre,isJudgeSp,is);
    return is;
}bool ElemCalculate::IsElementEndpoint(const DefElem &ele,
                                      const Point& p,
                                      double pre)
{
    Point sp;
    ElementEndpoint(ele,false,sp);
    if(p.IsSamePoint2D(sp,pre))
        return true;
    Point ep;
    ElementEndpoint(ele,true,ep);
    if(p.IsSamePoint2D(ep,pre))
        return true;
    return false;
}
void ElemCalculate::SetElementEndpoint(const Point& P,
                                       bool isChEp,
                                       DefElem& ele)
{
    if(ele._elemType == LINETYPE){
        if(isChEp){
            ele._segment._segEp = P;
        }
        else{
            ele._segment._segSp = P;
        }
    }
    else if(ele._elemType == ARCTYPE){
        if(isChEp){
            ele._arc._arcEp = P;
        }
        else{
            ele._arc._arcSp = P;
        }
    }
}
void ElemCalculate::ReorederLoopElement(int startIndex,bool isOrder,
                                        bool isAddIndex,DefLoop &loop)
{
    if(startIndex < 0 || startIndex > loop.ElemNum() - 1)
        return;
    if(!isOrder){
        std::reverse(loop.m_vElem.begin(),loop.m_vElem.end());
    }
    int index = startIndex;
    if(!isAddIndex){
        index += 1;
    }
    DefLoop loop_;
    for(int i = index;i < loop.m_vElem.size();++i){
        loop_.m_vElem.push_back(loop.m_vElem[i]);
    }
    for(int i = 0;i < startIndex;++i){
        loop_.m_vElem.push_back(loop.m_vElem[i]);
    }
    loop.m_vElem = loop_.m_vElem;
}
void ElemCalculate::ReverseSegmentDirection(Point& lSp,
                                            Point& lEp)
{
    Point P = lSp;    lSp = lEp;
    lEp = P;
}
void ElemCalculate::ReverseArcDirection(DefArc& arc)
{
    Point P = arc._arcSp;
    arc._arcSp = arc._arcEp;
    arc._arcEp = P;
    arc._arcRot = arc._arcRot == CLOCKWISE ?
                ANTICLOCKWISE : CLOCKWISE;
}
void ElemCalculate::ReverseEleDirection(DefElem& ele)
{
    if(ele._elemType == LINETYPE){
        Point P = ele._segment._segSp;
        ele._segment._segSp = ele._segment._segEp;
        ele._segment._segEp = P;
    }
    else if(ele._elemType == ARCTYPE){
        ReverseArcDirection(ele._arc);
    }
}
void ElemCalculate::ReverseLoopDirection(DefLoop &loop)
{
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        ReverseEleDirection(loop.m_vElem[i]);
    }
    std::reverse(loop.m_vElem.begin(),loop.m_vElem.end());
}
void ElemCalculate::LoopEndpoint(const DefLoop &loop,
                                 bool isLastPt,Point& P)
{
    if(loop.m_vElem.empty() || (loop.m_vElem.size() == 1 &&
                                loop.m_vElem[0]._elemType == CIRCLETYPE)){
        return;
    }
    if(isLastPt){
        int len = loop.m_vElem.size() - 1;
        ElementEndpoint(loop.m_vElem[len],true,P);
        return;
    }
    ElementEndpoint(loop.m_vElem[0],false,P);
}
Point ElemCalculate::LoopEndpoint(const DefLoop &loop,
                                  bool isLastPt)
{
    Point P;
    LoopEndpoint(loop,isLastPt,P);
    return P;
}
void ElemCalculate::PointSplitElement(const DefElem& ele,
                                      const Point& P,
                                      bool isChEndPnt,
                                      DefElem& newEle)
{
    newEle = ele;
    SetElementEndpoint(P,isChEndPnt,newEle);/**
    if(ele._elemType == LINETYPE){        DefSeg seg;
        if(isChEndPnt){
            seg = DefSeg(ele._segment.SegStartPt(),P);
        }
        else{
            seg = DefSeg(P,ele._segment._segEp);
        }
        newEle.SetElemSeg(seg);
    }
    else{
        DefArc arc;
        if(isChEndPnt){
            arc = DefArc(ele._arc.ArcStartPt(),P,ele._arc.ArcCenterPt(),
                         ele._arc.ArcRadius(),ele._arc.ArcRot());
        }
        else{
            arc = DefArc(P,ele._arc.ArcEndPt(),ele._arc.ArcCenterPt(),
                         ele._arc.ArcRadius(),ele._arc.ArcRot());
        }
        newEle.SetElemArc(arc);
    }*/
}
void ElemCalculate::TwoEleIsPossessSamePnt(const DefElem& ele1,
                                           const DefElem& ele2,
                                           bool& isSame)
{
    isSame = false;
    Point pnt1,pnt2;
    if(ele1._elemType == LINETYPE){
        pnt1 = ele1._segment._segSp;
        pnt2 = ele1._segment._segEp;
    }
    else if(ele1._elemType == ARCTYPE){
        pnt1 = ele1._arc._arcSp;
        pnt2 = ele1._arc._arcEp;
    }
    GeomCalculate gocal;
    if(ele2._elemType == LINETYPE){
        bool isSame1 = false,isSame2 = false,
                isSame3 = false,isSame4 = false;
        gocal.IsIdenticalPoint(pnt1,ele2._segment._segSp,PreErr5_8,isSame1);
        gocal.IsIdenticalPoint(pnt1,ele2._segment._segEp,PreErr5_8,isSame2);
        gocal.IsIdenticalPoint(pnt2,ele2._segment._segSp,PreErr5_8,isSame3);
        gocal.IsIdenticalPoint(pnt2,ele2._segment._segEp,PreErr5_8,isSame4);
        if(isSame1 || isSame2 || isSame3 || isSame4){
            isSame = true;
            return;
        }
    }
    else if(ele2._elemType == ARCTYPE){
        bool isSame1 = false,isSame2 = false,
                isSame3 = false,isSame4 = false;
        gocal.IsIdenticalPoint(pnt1,ele2._arc._arcSp,PreErr5_8,isSame1);
        gocal.IsIdenticalPoint(pnt1,ele2._arc._arcEp,PreErr5_8,isSame2);
        gocal.IsIdenticalPoint(pnt2,ele2._arc._arcSp,PreErr5_8,isSame3);
        gocal.IsIdenticalPoint(pnt2,ele2._arc._arcEp,PreErr5_8,isSame4);
        if(isSame1 || isSame2 || isSame3 || isSame4){
            isSame = true;
            return;
        }    }
}
void ElemCalculate::ElementMidpoint(const DefElem& ele,
                                    Point& mp)
{
    if(ele._elemType == LINETYPE){
        mp = ele._segment._segSp.Midpoint(
                    ele._segment._segEp);
    }else if(ele._elemType == ARCTYPE){
        GeomCalculate().ArcMidpoint(ele._arc,mp);
    }else if(ele._elemType == CIRCLETYPE){
        mp = ele._circle._cirCp;
    }
}
void ElemCalculate::JudgeEleValidity(const DefElem& ele,bool& isVal)
{
    isVal = false;
    GeomCalculate gocal;
    if(ele._elemType == LINETYPE){
        double lenth;
        gocal.TwoPointDistance(ele._segment._segSp,
                               ele._segment._segEp,lenth);
        if(lenth < PreErr5_6){
            return;
        }
    }
    else if(ele._elemType == ARCTYPE){
        double dis,arcLen;
        gocal.TwoPointDistance(ele._arc._arcSp,
                               ele._arc._arcEp,dis);
        if(dis < PreErr5_6){
            return;
        }
        gocal.CalculateArcLength(ele._arc,arcLen);
        if(arcLen < PreErr5_6){
            return;
        }
    }
    isVal = true;
}
void ElemCalculate::JudgeEleValidity(const DefElem& ele,double sigLim,
                                     double arcLim,bool& isVal)
{
    isVal = false;
    GeomCalculate gocal;
    if(ele._elemType == LINETYPE){
        double lenth;
        gocal.TwoPointDistance(ele._segment._segSp,ele._segment._segEp,lenth);
        if(lenth < sigLim){
            return;
        }
    }
    else if(ele._elemType == ARCTYPE){
        double dis,arcLen;
        gocal.TwoPointDistance(ele._arc._arcSp,
                               ele._arc._arcEp,dis);
        if(dis < arcLim){
            return;
        }        gocal.CalculateArcLength(ele._arc,arcLen);
        if(arcLen < arcLim){
            return;
        }
        if(ele.ElemArc().ArcRadius() < arcLim){
            return;
        }
    }
    isVal = true;
}
void ElemCalculate::ElementValidity(const DefElem& ele,double segLim,
                                    double arcLim,bool& isVal)
{
    isVal = false;
    GeomCalculate gocal;
    switch (ele._elemType) {
    case LINETYPE:{
        if(gocal.TwoPointDistance(ele._segment._segSp,
                                  ele._segment._segEp) < segLim)
            return;
        break;
    }
    case ARCTYPE:{
        if(gocal.TwoPointDistance(ele._arc.ArcStartPt(),
                                  ele._arc.ArcEndPt()) < arcLim)
            return;
        if(ele._arc.ArcRadius() < arcLim)
            return;
        break;
    }
    case CIRCLETYPE:{
        if(ele._circle.CircleRadius() < arcLim)
            return;
        break;
    }
    default:
        break;
    }
    isVal = true;
}
bool ElemCalculate::IsValidGeomArc(DefArc arc,double endVal,
                                   double R,double H,
                                   bool isDecNum,int number)
{
    double dis,angle;
    GeomCalculate gocal;
    arc.ChoiceSetRadius(PreErr5_2);
    dis = arc.ArcStartPt().Distance2D(arc.ArcEndPt());
    if(dis > PreErr_2){
        return true;
    }
    gocal.ArcCenterAngle(arc,angle);
    if((dis < endVal && angle < 180) || arc.ArcRadius() < R ||
            gocal.ArcBowHeight(arc) < H){
        return false;
    }
    if(!isDecNum){
        return true;
    }
    double sx_ = arc.ArcStartPt().X(),sy_ = arc.ArcStartPt().Y(),            ex_ = arc.ArcEndPt().X(),ey_ = arc.ArcEndPt().Y();
    MathCalculate mat;
    double sx = mat.NumericalDecimalPlace(sx_,number);
    double sy = mat.NumericalDecimalPlace(sy_,number);
    double ex = mat.NumericalDecimalPlace(ex_,number);
    double ey = mat.NumericalDecimalPlace(ey_,number);
    double endPre = 1 / std::pow(10.0,number);
    double vx_ = std::abs(sx_ - ex_);
    double vy_ = std::abs(sy_ - ey_);
    double vx = std::abs(sx - ex);
    double vy = std::abs(sy - ey);
    if(((vx_ > PreErr5_10 && vx < endPre) ||
        (vy_ > PreErr5_10 && vy < endPre)) && angle < 180){
        return false;
    }
    return true;
}
void ElemCalculate::IsValidGeomeElement(const DefElem& ele,
                                        double segPre,double arcPre,
                                        double H,double R, bool &isVal)
{
    isVal = false;
    GeomCalculate gocal;
    if(ele.ElemeType() == LINETYPE){
        if(ele.ElemSeg().SegStartPt().
                Distance2D(ele.ElemSeg().SegEndPt()) < segPre)
            return;
    }
    else if(ele.ElemeType() == ARCTYPE){
        double dis,angle;
        DefArc arc = ele.ElemArc();
        arc.ChoiceSetRadius(PreErr5_2);
        dis = gocal.TwoPointDistance2D(arc.ArcStartPt(),
                                       arc.ArcEndPt());
        gocal.ArcCenterAngle(arc,angle);
        if((dis < arcPre && angle < 180) ||
                arc.ArcRadius() < R ||
                gocal.ArcBowHeight(arc) < H)
            return;/**
        double lenth;
        gocal.CalculateArcLength(ele._arc,lenth);
        if(lenth < arcPre)
            return;*/
    }
    else if(ele.ElemeType() == CIRCLETYPE){
        if(ele.ElemCircle().CircleRadius() < R)
            return;
    }
    isVal = true;
}
bool ElemCalculate::IsValidGeomeElement(const DefElem& ele,double segPre,
                                        double arcPre,double H, double R)
{
    bool isVal = false;
    IsValidGeomeElement(ele,segPre,arcPre,H,R,isVal);
    return isVal;
}
void ElemCalculate::ElementEndpointStepVal(const DefElem&ele,
                                           double expeVal,
                                           double& stepVal)
{    GeomCalculate gocal;
    double times = 1.12;
    double eleLen = gocal.ElementLenth(ele);
    ///当元素长度小于期望步长时 ,stepVal需要自适应调整.
    if(eleLen > expeVal){
        stepVal = expeVal;
    }
    else if(eleLen > PreErr_10_){
        stepVal = PreErr_10_ / times;
    }
    else if(eleLen > PreErr_9_){
        stepVal = PreErr_9_ / times;
    }
    else if(eleLen > PreErr_8_){
        stepVal = PreErr_8_ / times;
    }
    else if(eleLen > PreErr_7_){
        stepVal = PreErr_7_ / times;
    }
    else if(eleLen > PreErr_6_){
        stepVal = PreErr_6_ / times;
    }
    else if(eleLen > PreErr_5_){
        stepVal = PreErr_5_ / times;
    }
    else if(eleLen > PreErr_4_){
        stepVal = PreErr_4_ / times;
    }
    else if(eleLen > PreErr_3_){
        stepVal = PreErr_3_ / times;
    }
    else if(eleLen > PreErr_2_){
        stepVal = PreErr_2_ / times;
    }
    else if(eleLen > PreErr_1){
        stepVal = PreErr_1 / times;
    }
    else if(eleLen > PreErr5_2){
        stepVal = PreErr5_2 / times;
    }
    else if(eleLen > PreErr_2){
        stepVal = PreErr_2 / times;
    }
    else if(eleLen > PreErr5_3){
        stepVal = PreErr5_3 / times;
    }
    else if(eleLen > PreErr_3){
        stepVal = PreErr_3 / times;
    }
    else if(eleLen > PreErr5_4){
        stepVal = PreErr5_4 / times;
    }
    else if(eleLen > PreErr_4){
        stepVal = PreErr_4 / times;
    }
    else if(eleLen > PreErr5_5){
        stepVal = PreErr5_5 / times;
    }
    else if(eleLen > PreErr_5){
        stepVal = PreErr_5 / times;
    }
    else if(eleLen > PreErr5_6){
        stepVal = PreErr5_6 / times;
    }    else if(eleLen > PreErr_6){
        stepVal = PreErr_6 / times;
    }
    else if(eleLen > PreErr5_7){
        stepVal = PreErr5_7 / times;
    }
    else if(eleLen > PreErr_7){
        stepVal = PreErr_7 / times;
    }
    else if(eleLen > PreErr5_8){
        stepVal = PreErr5_8 / times;
    }
    else if(eleLen > PreErr_8){
        stepVal = PreErr_8 / times;
    }
    else {
        stepVal = eleLen / times;
    }
}
bool ElemCalculate::IsPointInArrayPoint(const vector<Point>& vPoint,
                                        const Point& P,
                                        double errVal)
{
    GeomCalculate gocal;
    for(auto& p : vPoint){
        if(gocal.IsIdenticalPoint(P,p,errVal)){
            return true;
        }
    }
    return false;
}
bool ElemCalculate::IsIdenticalLoop(const DefLoop& loop1,
                                    const DefLoop& loop2)
{
    if(loop1.m_vElem.size() != loop2.m_vElem.size()){
        return false;
    }
    for(auto& e1 : loop1.m_vElem){
        for(auto& e2 : loop1.m_vElem){
            bool isSame = IsIdenticalElement(e1,e2,PreErr5_10);
            if(!isSame){
                return false;
            }
        }
    }
    return true;
}
void ElemCalculate::TranslateLoopToPoint(const DefLoop& loop,
                                         double value,bool isBaseAngle,
                                         vector<Point>&vPoint)
{
    MathCalculate matcal;
    GeomCalculate gocal;
    vector<Point>vPoint_;
    for (int i = 0; i < loop.m_vElem.size(); ++i) {
        if(loop.m_vElem[i]._elemType == LINETYPE){
            vPoint.push_back(loop.m_vElem[i]._segment.SegStartPt());
        }
        else if(loop.m_vElem[i]._elemType == ARCTYPE){            double stepVal = value,angVal;
            DefArc arc = loop.m_vElem[i]._arc;
            if(!isBaseAngle){
                stepVal = matcal.AngleBaseBowHeight(arc.ArcRadius(),value);
            }
            gocal.ArcCenterAngle(arc,angVal);
            if(angVal < stepVal){
                stepVal = angVal / 2;
            }
            if(stepVal < PreErr5_2){
                stepVal = PreErr5_2;
            }
            gocal.DiscreteArcToPoint(arc,stepVal,vPoint_);
        }
        else if(loop.m_vElem[i]._elemType == CIRCLETYPE){
            double stepVal = value;
            DefCircle circle = loop.m_vElem[i]._circle;
            if(!isBaseAngle){
                stepVal = 
matcal.AngleBaseBowHeight(circle.CircleRadius(),value);
            }
            
gocal.DiscreteCircleToPoint(circle.CircleCenter(),circle.CircleRadius(),
                                        
loop._loopLabel._loRotDir,stepVal,vPoint_);
        }
        for (int p = 0; p < vPoint_.size(); ++p) {
            vPoint.push_back(vPoint_[p]);
        }
        vPoint_.clear();
    }
}
void ElemCalculate::SetLoopOpenClose(DefLoop& loop)
{
    if(loop.m_vElem.empty()){
        return;
    }
    if(loop.m_vElem.size() == 1 &&
            loop.m_vElem[0]._elemType == CIRCLETYPE){
        loop.SetOpenLoop(false);
    }
    Point startPt,endPt;
    int len = loop.m_vElem.size() - 1;
    DefElem lastEle = loop.m_vElem[len];
    ElementEndpoint(loop.m_vElem[0],false,startPt);
    ElementEndpoint(lastEle,true,endPt);
    GeomCalculate gocal;
    bool isClose = gocal.IsIdenticalPoint(startPt,endPt,PreErr5_8);
    loop.SetOpenLoop(!isClose);
}
bool ElemCalculate::IsCloseLoop(const DefLoop& loop,double errVal)
{
    if(loop.IsEmpty())
        return false;
    Point startPt = LoopEndpoint(loop,false),
            endPt = LoopEndpoint(loop,true);
    return GeomCalculate().IsIdenticalPoint2D(startPt,endPt,errVal);
}
void ElemCalculate::ResetIndexCurrentLoop(const vector<DefLoop>& vLoop,
                                          const DefLoop& curLoop,                                          int index,
                                          vector<DefLoop>& vLoop_,
                                          DefLoop& curLoop_)
{
    vLoop_.clear();
    vLoop_.push_back(curLoop);
    curLoop_ = vLoop[index];
    for(int i = 0;i < vLoop.size();i ++){
        if(i != index){
            vLoop_.push_back(vLoop[i]);
        }
    }
}
int ElemCalculate::LongestElementIndex(const DefLoop& loop)
{
    int index = 0;
    if(loop.m_vElem.empty()){
        return index;
    }
    GeomCalculate gocal;
    double lenth = gocal.ElementLenth(loop.m_vElem[0]);
    for (int i = 1; i < loop.m_vElem.size(); ++i) {
        double lenth_ = gocal.ElementLenth(loop.m_vElem[i]);
        if(lenth_ > lenth){
            index = i;
            lenth = lenth_;
        }
    }
    return index;
}
void ElemCalculate::ArrayPointAverage(const vector<Point>&v,Point& P)
{
    if(v.empty())
        return;
    double x = 0,y = 0,z = 0;
    for(auto& p : v){
        x += p.X();
        y += p.Y();
        z += p.Z();
    }
    P.SetX(x / v.size());
    P.SetY(y / v.size());
    P.SetZ(z / v.size());
}
void ElemCalculate::ElemCoordDecimalPlaces(int number,DefElem &ele)
{
    MathCalculate mat;
    switch (ele.ElemeType()) {
    case LINETYPE:{
        double sx = ele.ElemSeg().SegStartPt().X(),
                sy = ele.ElemSeg().SegStartPt().Y(),
                ex = ele.ElemSeg().SegEndPt().X(),
                ey = ele.ElemSeg().SegEndPt().Y();
        ele._segment._segSp.SetX(mat.NumericalDecimalPlace(sx,number));
        ele._segment._segSp.SetY(mat.NumericalDecimalPlace(sy,number));
        ele._segment._segEp.SetX(mat.NumericalDecimalPlace(ex,number));
        ele._segment._segEp.SetY(mat.NumericalDecimalPlace(ey,number));
        break;
    }
    case ARCTYPE:{
        double sx = ele.ElemArc().ArcStartPt().X(),                sy = ele.ElemArc().ArcStartPt().Y(),
                ex = ele.ElemArc().ArcEndPt().X(),
                ey = ele.ElemArc().ArcEndPt().Y()/**,
                                                                                
cx = ele.ElemArc().ArcCenterPt().X(),
                                                                                
cy = ele.ElemArc().ArcCenterPt().Y()*/;
        ele._arc._arcSp.SetX(mat.NumericalDecimalPlace(sx,number));
        ele._arc._arcSp.SetY(mat.NumericalDecimalPlace(sy,number));
        ele._arc._arcEp.SetX(mat.NumericalDecimalPlace(ex,number));
        ele._arc._arcEp.SetY(mat.NumericalDecimalPlace(ey,number));/**
        ele._arc._arcCp.SetX(mat.NumericalDecimalPlace(cx,number));
        ele._arc._arcCp.SetY(mat.NumericalDecimalPlace(cy,number));*/
        break;
    }
    case CIRCLETYPE:{
        break;
    }
    default:
        break;
    }
}
void ElemCalculate::LoopPointBaseLenth(const DefLoop& loop, int i,
                                       bool isOrder, double lenth, Point& p)
{
    if(!loop.IsValIndex(i))
        return;
    double l = 0;
    GeometryLogic golg;GeomCalculate gocal;
    if(isOrder){
        const DefElem& ele = loop.m_vElem[i];
        l = gocal.ElementLenth(ele);
        if(l >= lenth){
            golg.ApproachCurveEndpoint(ele,lenth,false,true,p);
            return;
        }
    }else{
        const DefElem& ele = loop.m_vElem[i];
        l = gocal.ElementLenth(ele);
        if(l >= lenth){
            golg.ApproachCurveEndpoint(ele,lenth,false,false,p);
            return;
        }
    }
    int id = i;
    while (l < lenth) {
        if(isOrder){
            id = loop.NextIndex(id);
        }else{
            id = loop.PreviousIndex(id);
        }
        double len = gocal.ElementLenth(loop.IndexElem(id));
        if(l + len >= lenth){
            double l_ = lenth - l;
            golg.ApproachCurveEndpoint(loop.IndexElem(id),l_,false,isOrder,p);
            return;
        }
        l += len;
    }
}void ElemCalculate::LoopPointBaseCurve(const DefLoop& loop,
                                       const Point& p, int id,
                                       bool isOrder, double lenth, Point& res)
{
    double len = 0;
    DefElem pEle,nEle;
    const DefElem& ele = loop.m_vElem[id];
    PointSplitElement(ele,p,true,pEle);
    PointSplitElement(ele,p,false,nEle);
    GeometryLogic golg;GeomCalculate gocal;
    if(isOrder){
        len = gocal.ElementLenth(nEle);
        if(len >= lenth - PreErr5_10){
            golg.ApproachCurveEndpoint(nEle,lenth,false,true,res);
            return;
        }
        LoopPointBaseLenth(loop,loop.NextIndex(id),
                           isOrder,lenth-len,res);
    }else{
        len = gocal.ElementLenth(pEle);
        if(len >= lenth - PreErr5_10){
            golg.ApproachCurveEndpoint(nEle,lenth,false,false,res);
            return;
        }
        LoopPointBaseLenth(loop,loop.PreviousIndex(id),
                           isOrder,lenth-len,res);
    }
}
DefCircle ElemCalculate::PointsMinCircumcircle(vector<Point> &points,
                                               vector<Point> &boundary,
                                               size_t id)
{
    if (id >= points.size() || boundary.size() >= 3) {
#if 0
        _mainwind->myOccView->getContext()->RemoveAll(true);
        for (int i = 0; i < points.size(); ++i) {
            DisplayGeom().DisplayPoint(points[i],_colors[6]);
        }
        _mainwind->myOccView->fitAll();
        DisplayGeom().DisplayLoop(loop,_colors.back(),1,false,true);
#endif
        if (boundary.empty()){
            return  DefCircle({0,0},0) ;
        }
        if (boundary.size() == 1){
#if 0
            DisplayGeom().DisplayPoint(boundary[0],_colors[1]);
#endif
            return  DefCircle(boundary[0],0) ;
        }
        if (boundary.size() == 2){
            DefCircle cir(boundary[0].Midpoint(boundary[1]),
                    boundary[0].Distance2D(boundary[1])/2);
#if 0
            DisplayGeom().DisplayPoint(boundary[0],_colors[2]);
            DisplayGeom().DisplayPoint(boundary[1],_colors[2]);
            DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,cir),_colors[1],1);
            _mainwind->myOccView->fitAll();#endif
            return cir;
        }
        const Point& a = boundary[0];
        const Point& b = boundary[1];
        const Point& c = boundary[2];
        double A = b.X() - a.X();
        double B = b.Y() - a.Y();
        double C = c.X() - a.X();
        double D = c.Y() - a.Y();
        double E = A * (a.X() + b.X()) + B * (a.Y() + b.Y());
        double F = C * (a.X() + c.X()) + D * (a.Y() + c.Y());
        double G = 2 * (A * (c.Y() - b.Y()) - B * (c.X() - b.X()));
        /// 三点共线,返回直径最大的圆
        if (std::abs(G) < PreErr5_8) {
            DefCircle c1 = DefCircle(a.Midpoint(b),a.Distance2D(b)/2);
            DefCircle c2 = DefCircle(a.Midpoint(c),a.Distance2D(c)/2);
            DefCircle c3 = DefCircle(b.Midpoint(c),b.Distance2D(c)/2);
            if (c1._cirR >= c2._cirR && c1._cirR >= c3._cirR) return c1;
            else if (c2._cirR >= c1._cirR && c2._cirR >= c3._cirR) return c2;
            else return c3;
        }
        double x = (D * E - B * F) / G;
        double y = (A * F - C * E) / G;
        Point cp(x,y);
        double r = cp.Distance2D(a);
        DefCircle cir_(cp, r);
#if 0
        DisplayGeom().DisplayPoint(a,_colors[2]);
        DisplayGeom().DisplayPoint(b,_colors[2]);
        DisplayGeom().DisplayPoint(c,_colors[2]);
        DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,cir_),_colors[1],1);
        _mainwind->myOccView->fitAll();
#endif
        return cir_;
    }
    ///按顺序处理点.
    const Point& p = points[id];
    ///  递归计算不包括p 的最小外接圆.
    DefCircle circle = PointsMinCircumcircle(points,boundary,id + 1);
#if 0
    DisplayGeom().DisplayPoint(p,_colors[0]);
    DisplayGeom().DisplayPoint(circle._cirCp,_colors[1]);
    DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,circle),_colors[1],1);
#endif
    ///如果p在圆内,直接返回
    if (circle.IsContainPoint(p,PreErr5_8)){
        return circle;
    }
    ///否则,将p加入边界点,重新计算.
    boundary.push_back(p);
    circle = PointsMinCircumcircle(points,boundary,id + 1);
    boundary.pop_back();
    return circle;
}
void ElemCalculate::LoopMinCircumcircle(const DefLoop &loop, bool isAgl,
                                        double err, DefCircle &circle)
{
    if(loop.IsEmpty())
        return;
    if(loop.IndexElem(0).ElemeType() == CIRCLETYPE){
        circle = loop.IndexElem(0).ElemCircle();
        return;    }
    vector<Point>points;
    GeomCalculate gocal;MathCalculate matcal;
    for(const auto& d : loop.m_vElem){
        if(d.ElemeType() == LINETYPE){
            points.emplace_back(d._segment._segSp);
        }else if(d.ElemeType() == ARCTYPE){
            vector<Point>ps;
            double step = err;
            if(!isAgl){
                step = matcal.AngleBaseBowHeight(d._arc._arcR,err);
            }
            gocal.DiscreteArcToPoint(d._arc,step,ps);
            if(!ps.empty()){
                ps.pop_back();
            }
            for(auto& p : ps){
                points.emplace_back(p);
            }
        }
    }/**
    vector<Point>points{
    {66.0681,0.8055},{61.758,28.5323},{67.9538,13.1884},{84.9249,25.0328},
    {79.5373,44.1454},{70.3782,29.6091},{67.415,60.2969},{108.9001,62.7196},
    {109.9776,38.4924},{131.7977,38.4924},{155.234,38.4924},{155.234,155.234},
    {155.234,155.234},{155.234,155.234},{155.234,155.234},{121.8305,-8.6162},
    {121.8305,-23.1525},{100.5492,-23.1525},{71.1864,-27.998},{83.3086,-
10.5005},
    {79.8067,8.0737},{108.0919,7.8045},{114.0184,25.8404},{136.6466,14.2651},
    {140.1485,-30.4207},{191.062,-36.8813},{160.8911,-65.6848},{106.745,-
64.8772},
    {120.2142,-49.8025},{210.1882,-7.2702},{244.9387,-7.5394},{236.049,23.4176},
    {48.0194,-57.8783},{-15.2857,-39.5732},{-26.061,-2.4248},{-2.4248,-33.651},
    {37.5135,-1.348},{12.7302,39.0308},{54.754,80.2171},{39.6685,24.7636},
    {37.5135,6.9969},{55.8315,-37.6889},{-24.1753,-75.9141},{-24.1753,-
75.9141}};
    for (int i = 0; i < points.size(); ++i) {
        points[i]._lable._id = i;
    }*/
#if 0
    DisplayGeom().DisplayLoop(loop,_colors.back(),1);
    for (int i = 0; i < points.size(); ++i) {
        DisplayGeom().DisplayPoint(points[i],_colors[6]);
    }
    _mainwind->myOccView->fitAll();
#endif
    /// vector<Point>boundary;
    /// circle = PointsMinCircumcircle(points,boundary,0);
    /// 初始圆:只包含第一个点
    DefCircle current = DefCircle(points[0], 0);
    for (size_t i = 1; i < points.size(); ++i) {
        /// 如果当前点不在圆内，需要更新圆
        if (current.IsContainPoint(points[i], PreErr5_10))
            continue;
        /// 新圆初始化为只包含这个点
        current = DefCircle(points[i], 0);
        /// 重新检查前面的所有点
        for (size_t j = 0; j < i; ++j) {
            if (current.IsContainPoint(points[j], PreErr5_10))
                continue;
            /// 构造包含这两个点的圆
            current = DefCircle(points[i].Midpoint(points[j]),
                                points[i].Distance2D(points[j]) / 2);
            /// 再次检查前面的点            for (size_t k = 0; k < j; ++k) {
                if (current.IsContainPoint(points[k],PreErr5_10))
                    continue;
                /// 构造包含这三个点的圆
                const Point& a = points[i];
                const Point& b = points[j];
                const Point& c = points[k];
                double A = b.X() - a.X();
                double B = b.Y() - a.Y();
                double C = c.X() - a.X();
                double D = c.Y() - a.Y();
                double E = A * (a.X() + b.X()) + B * (a.Y() + b.Y());
                double F = C * (a.X() + c.X()) + D * (a.Y() + c.Y());
                double G = 2 * (A * (c.Y() - b.Y()) - B * (c.X() - b.X()));
                /// 三点共线,返回直径最大的圆
                if (std::abs(G) < PreErr5_10) {
                    DefCircle c1 = DefCircle(a.Midpoint(b), a.Distance2D(b) / 
2);
                    DefCircle c2 = DefCircle(a.Midpoint(c), a.Distance2D(c) / 
2);
                    DefCircle c3 = DefCircle(b.Midpoint(c), b.Distance2D(c) / 
2);
                    if (c1._cirR >= c2._cirR && c1._cirR >= c3._cirR) {
                        current = c1;
                    } else if (c2._cirR >= c1._cirR && c2._cirR >= c3._cirR) {
                        current = c2;
                    } else {
                        current = c3;
                    }
                    continue;
                }
                double x = (D * E - B * F) / G;
                double y = (A * F - C * E) / G;
                Point cp(x, y);
                double r = cp.Distance2D(a);
                current = DefCircle(cp,r);
            }
        }
    }
    circle = current;
#if 0
    DisplayGeom().DisplayElement(DefElem(CIRCLETYPE,circle),
                                 _colors.back(),1);
    _mainwind->myOccView->fitAll();
#endif
}
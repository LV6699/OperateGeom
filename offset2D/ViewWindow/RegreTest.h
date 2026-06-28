#ifndef REGRETEST_H
#define REGRETEST_H
#include"HearderFile.h"
struct RegDataCom
{
    RegDataCom() {}
    RegDataCom(string file,double val,int preLoopNum,
               int preEleNum,int curLoopNum,int curEleNum)
    {
        m_fileNam = file;
        _offsetVal = val;
        m_preLoopNum = preLoopNum;
        m_preEleNum = preEleNum;
        m_curLoopNum = curLoopNum;
        m_curEleNum = curEleNum;
    }
    string m_fileNam;
    double _offsetVal;
    int m_preLoopNum;
    int m_preEleNum;
    int m_curLoopNum;
    int m_curEleNum;
};
struct RegData
{
    RegData() {}
    RegData(string fileNam,string modNam,double offsetVal,
            int loopNum,int eleNum)
    {
        m_fileNam = fileNam;
        m_modNam = modNam;
        _offsetVal = offsetVal;
        m_loopNum = loopNum;
        m_eleNum = eleNum;
    }
    string m_fileNam;
    string m_modNam;
    double _offsetVal;
    int m_loopNum;
    int m_eleNum;
    bool m_isVis = false;
};
class RegreTest
{
public:
    RegreTest(){}
    void PrintSplitErrStr(int i,string splitStr,vector<string>vStr);
    void GetDirFileTpData(string dir,vector<RegData>&vRepData);
    void GetReportFileTpData(string repFile,
                             vector<RegData> &vRepData);
    void RegComTpData(string repDir, string repFile,
                      string tpDir, string regResDir);
};
#endif // REGRETEST_H
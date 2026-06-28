#ifndef HANDLETEXT_H
#define HANDLETEXT_H
#include"../HearderFile.h"
struct TP_FileNam
{
    TP_FileNam() {}
    TP_FileNam(string file,double val,string name)
    {
        m_fileName = file;
        _offsetVal = val;
        m_modelNam = name;
    }
    string m_fileName;
    double _offsetVal;
    string m_modelNam;
};
class HandleText
{
public:
    HandleText(){}
    void SlipeString(string str,char sep,vector<string>&vStr);
    void SplitString(string str,vector<string> seperators,
                                   vector<string>&objs);
    void ReadFile(string file,vector<string>&vLine);
    void GetDirFileList(string dir, string suffixNam,
                        vector<string> &vFile);
    void GetFileFromDir(string dir, string fileNam, string suffix,
                        vector<string>vSep, bool &isFind, string& file);
    void GetFailTpContoureData(string profDir,string tpDir,
                               string tarDir);
};
#endif // HANDLETEXT_H
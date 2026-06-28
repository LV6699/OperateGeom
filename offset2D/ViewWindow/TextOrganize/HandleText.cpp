#include "HandleText.h"
void HandleText::SlipeString(string str,char sep,vector<string>&vStr)
{
    int startIdx = 0, endIdx = 0;
    for (int i = 0; i <= str.size(); i++){
        if (str[i] == sep || i == str.size()){
            endIdx = i;
            string temp;
            temp.append(str,startIdx,endIdx - startIdx);
            vStr.push_back(temp);
            startIdx = endIdx + 1;
        }
    }
}
void HandleText::SplitString(string str,vector<string> seperators,
                             vector<string>&objs)
{
    objs.clear();
    string sep;
    string it;
    while (str.size() > 0)
    {
        for(unsigned int t=0; ; t++){
            if (t == seperators.size())
{
                // No seperator matched
                if (sep.size() > 0){
                    sep = "";
                }
                it.push_back(str[0]);
                unsigned int size_take = 1;
                str = str.substr(size_take, str.size()- size_take);
                break;
            }
            size_t pos = str.find(seperators[t]);
            if (pos == 0){
                // Match some seperator
                if (it.size() != 0){
                    objs.push_back(it);
                    it = "";
                }
                sep += seperators[t];
                unsigned int size_take = seperators[t].size();
                str = str.substr(size_take, str.size() - size_take);
                break;
            }
        }
    };
    if (it.size() > 0){
        objs.push_back(it);
    }
}
void HandleText::ReadFile(string file,vector<string>&vLine)
{
    std::fstream fin(file,std::ios::in);
    if(!fin){
        return;
    }
    while(!fin.eof()){
        string str;
        std::getline(fin,str);
        vLine.push_back(str);
    }
}
void HandleText::GetDirFileList(string dir, string suffixNam,
                                vector<string> &vFile)
{
#if __linux__
    DIR *dirptr = NULL;
    struct dirent *dirp;
    if((dirptr = opendir(dir.c_str())) == NULL){
        return;
    }
#endif
    const char* surffix = suffixNam.data();
    char* surf = const_cast<char*>(surffix);
    vector<string> vSep{"_tp_","."};
#if __linux__
    while ((dirp = readdir(dirptr)) != NULL){
        if (dirp->d_type == DT_REG){
            string filNam = dirp->d_name;
            vector<string>vStr;
            SplitString(filNam,vSep,vStr);
            if(vStr.size() < 2){
                std::cout<<"readFilNamErr:"<<filNam<<std::endl;
                continue;
            }
            if(vStr.back() == suffixNam){
                vFile.push_back(filNam);
            }
        }
    }
#endif
}
void HandleText::GetFileFromDir(string dir,string fileNam,
                                string suffix,vector<string>vSep,
                                bool &isFind,string& file)
{
    isFind = false;
    vector<string> vFile;
    GetDirFileList(dir,suffix,vFile);
    for (int i = 0; i < vFile.size(); ++i) {
        vector<string>vStr;
        SplitString(vFile[i],vSep,vStr);
        if(vStr.size() < 2){
            continue;
        }
        if(vStr[0] == fileNam){
            file = vFile[i];
            isFind = true;
            return;
        }
    }
}
void HandleText::GetFailTpContoureData(string profDir,
                                       string tpDir,
                                       string tarDir)
{
    string suffix = "json";
    vector<string>vProfFile,vTpFile;
    GetDirFileList(profDir,suffix,vProfFile);
    GetDirFileList(tpDir,suffix,vTpFile);
    vector<string> vProfSep{"prof_",".step",".stp",".STEP",".STP"},
    vTpSep{"_tp_"};
    for(int i = 0;i < vTpFile.size();++i){
        vector<string>vStr;
        SplitString(vTpFile[i],vTpSep,vStr);
        if(vStr.size() < 2){
            std::cout<<"err-> i:"<< i << " "<< vTpFile[i]<<" split error."<<std::endl;
            continue;
        }
        string modNam = vStr[0],file;
        bool isFind = false;
        for (int j = 0; j < vProfFile.size(); ++j) {
            vector<string>vStr;
            SplitString(vProfFile[j],vProfSep,vStr);
            if(vStr.size() < 2){
                continue;
            }
            if(vStr[0] == modNam){
                file = vProfFile[j];
                isFind = true;
                break;
            }
        }
        ///GetFileFromDir(profDir,modNam,suffix,vProfSep,isFind,file);
        if(!isFind){
            std::cout<<"err-> i:"<< i << " "<< vTpFile[i]<<" find error."<<std::endl;
            continue;
        }
        string filePath = profDir + "/" + file;
        //cout<<filePath<<endl;
        system(("cp " + filePath + " " + tarDir).c_str());
    }
}
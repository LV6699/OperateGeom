#ifndef CNTLINEPATH_H
#define CNTLINEPATH_H

void AddLoopSteerPath(const
                      std::vector<JLC_Leads::LeadBase*>paramsIn,
                      const
                      std::vector<JLC_Leads::LeadBase*>paramsOut,
                      const JLC_Leads::ModelProtect &modelp,
                      const DefLoop& loop,
                      std::vector<JLC_Wire>& path);
void CreatePathNodeConnect(const Connect::connect& cntParam,
                           const JLC_Leads::ModelProtect& modelp,
                           const
                           std::vector<std::vector<JLC_Wire>>& wiress,
                           JLC_Opera::JLC_ToolPath*& tpath);

#endif // CNTLINEPATH_H

FnopTLHhOBtpUG_uw969HAFFGIXU
Page 1#ifndef APPENDEXTRAPATH_H
#define APPENDEXTRAPATH_H
#include"../../CommonFile/Header.h"
class AppendExtraPath
{
public:
    AppendExtraPath(){}
    void CreateAppendExtraPath(double radius double offsetVal
                               std::shared_ptr<OffsetNode> &pathNode);
    void ExicuteAppendExtraPath(double radius double offsetVal
                                std::shared_ptr<OffsetNode> &pathNode);
    void LoopAppendExtraPath(double radius  double offsetVal DefLoop &loop);
};
#endif // APPENDEXTRAPATH_H
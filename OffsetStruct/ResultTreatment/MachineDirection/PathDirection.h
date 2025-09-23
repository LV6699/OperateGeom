#ifndef PATHDIRECTION_H
#define PATHDIRECTION_H
#include"../../CommonFile/Header.h"
class PathDirection
{
public:
    PathDirection(){}
    void SpecifyPathMillDirection(OffsetParam offsetPara,
                                  shared_ptr<OffsetNode>&rootNode);
    void SetToolPathDirection(bool isToolClock, bool isClockMill,
                              bool isProfile,shared_ptr<OffsetNode>&rootNode);
    void SetAreaPathDirection(bool isToolClock,bool isAreaClock,
                              shared_ptr<OffsetNode>&rootNode);
    bool IsChangePathDirection(ToolPathType pathType,
                               bool isToolClock,
                               bool isClockMill);
};
#endif // PATHDIRECTION_H
#ifndef TrajectoryCheck_H
#define TrajectoryCheck_H
#include"../HearderFile.h"
class ManipuWidget;
namespace Ui {
class ManipuWidget;
}
class TrajectoryCheck
{
public:
    TrajectoryCheck(){}
    void ExecuteTrajectoryCheck(ManipuWidget*pointer);
    void SingleToolPathCheck(ManipuWidget *pointer, string path);
};
#endif // TrajectoryCheck_H
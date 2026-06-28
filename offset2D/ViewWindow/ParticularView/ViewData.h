#ifndef VIEWDATA_H
#define VIEWDATA_H
#include <QtWidgets/QWidget>
#include "AIS_TextLabel.hxx"
#include"../HearderFile.h"
namespace Ui {
class ViewData;
}
class ViewData : public QWidget
{
    Q_OBJECT
public:
    explicit ViewData(QWidget *parent = nullptr);
    ~ViewData();
public slots:
    void SignalViewSelShapeInfo();
    void PrintSelectLoop(AIS_Shape shape);
private:
    Ui::ViewData *ui;
    using Label = Handle(AIS_TextLabel);
    std::vector<Label>m_allLabel;
    size_t m_labelNum = 0;
    std::vector<Label>m_vNodLabel;
    void ClearAllLabel();
    Label GetNewLabel();
    void UpdateLabel(Label&label);
    void FindSelInfo(Handle(AIS_Shape)selShape,int&idx,
                     int& nodIdx,bool& isTp);
    void SameNodeLoopIdx(int nodIdx);
};
#endif // VIEWDATA_H
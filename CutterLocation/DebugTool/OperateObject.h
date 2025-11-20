#ifndef OPERATEOBJECT_H
#define OPERATEOBJECT_H

#include <QDialog>
#include "../../OffsetStruct/CommonFile/DataStructure.h"
#include "../../ViewWindow/OccHeader.h"

namespace Ui {
class OperateObject;
}

class OperateObject : public QDialog
{
    Q_OBJECT
public:

    explicit OperateObject(QWidget *parent = nullptr);
    ~OperateObject();
    using Label = Handle(AIS_TextLabel);

    void CreateInfo();

    oft::Point ToPoint(const gp_Pnt& p){return oft::Point(p.X(),p.Y(),p.Z());}
    gp_Pnt ToOccPoint(const oft::Point& p){return gp_Pnt(p.X(),p.Y(),p.Z());}

    void ClearAllLabel();
    void UpdateLabel(OperateObject::Label&label);
    void FindSelectObject();
    void FindTrianges();
    void FindSelItem();
    void FindIntBasePoint(const oft::Point& p);

    void GetSelPointIndex();

public:
    oft::Point _p,_mp;
    oft::DefElem _selObj;
    bool _isPoint = false;
    bool _hasUiFind = false,_hasFind = false;
    TCollection_ExtendedString _info;

    Handle(AIS_Shape) _trisCl_as,_tris_as;
    bool _hasDispCl = false;
    Handle(AIS_Shape) _intFig_as;

    Ui::OperateObject *ui;
    std::vector<Label>_labels;
};

#endif // OPERATEOBJECT_H

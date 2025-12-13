/********************************************************************************
** Form generated from reading UI file 'OperateObject.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPERATEOBJECT_H
#define UI_OPERATEOBJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_OperateObject
{
public:

    void setupUi(QDialog *OperateObject)
    {
        if (OperateObject->objectName().isEmpty())
            OperateObject->setObjectName(QString::fromUtf8("OperateObject"));
        OperateObject->resize(400, 300);

        retranslateUi(OperateObject);

        QMetaObject::connectSlotsByName(OperateObject);
    } // setupUi

    void retranslateUi(QDialog *OperateObject)
    {
        OperateObject->setWindowTitle(QApplication::translate("OperateObject", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OperateObject: public Ui_OperateObject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPERATEOBJECT_H

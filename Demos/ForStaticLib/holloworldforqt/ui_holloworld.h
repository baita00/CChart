/********************************************************************************
** Form generated from reading UI file 'holloworld.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOLLOWORLD_H
#define UI_HOLLOWORLD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_holloworld
{
public:

    void setupUi(QWidget *holloworld)
    {
        if (holloworld->objectName().isEmpty())
            holloworld->setObjectName(QStringLiteral("holloworld"));
        holloworld->resize(400, 300);

        retranslateUi(holloworld);

        QMetaObject::connectSlotsByName(holloworld);
    } // setupUi

    void retranslateUi(QWidget *holloworld)
    {
        holloworld->setWindowTitle(QApplication::translate("holloworld", "holloworld", 0));
    } // retranslateUi

};

namespace Ui {
    class holloworld: public Ui_holloworld {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOLLOWORLD_H

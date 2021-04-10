/********************************************************************************
** Form generated from reading UI file 'ScenariosWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCENARIOSWIDGET_H
#define UI_SCENARIOSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScenariosWidget
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;

    void setupUi(QWidget *ScenariosWidget)
    {
        if (ScenariosWidget->objectName().isEmpty())
            ScenariosWidget->setObjectName(QString::fromUtf8("ScenariosWidget"));
        ScenariosWidget->resize(373, 331);
        gridLayout = new QGridLayout(ScenariosWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(ScenariosWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(ScenariosWidget);

        QMetaObject::connectSlotsByName(ScenariosWidget);
    } // setupUi

    void retranslateUi(QWidget *ScenariosWidget)
    {
        ScenariosWidget->setWindowTitle(QCoreApplication::translate("ScenariosWidget", "ScenariosWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScenariosWidget: public Ui_ScenariosWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENARIOSWIDGET_H

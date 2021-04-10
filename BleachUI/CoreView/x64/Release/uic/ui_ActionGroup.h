/********************************************************************************
** Form generated from reading UI file 'ActionGroup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTIONGROUP_H
#define UI_ACTIONGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ActionGroupUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QFrame *line;
    QLabel *label;
    QFrame *groupSeparator;

    void setupUi(QWidget *ActionGroupUi)
    {
        if (ActionGroupUi->objectName().isEmpty())
            ActionGroupUi->setObjectName(QString::fromUtf8("ActionGroupUi"));
        ActionGroupUi->resize(275, 70);
        gridLayout = new QGridLayout(ActionGroupUi);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(ActionGroupUi);
        line->setObjectName(QString::fromUtf8("line"));
        line->setStyleSheet(QString::fromUtf8("color: #c0c0c0;"));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        verticalLayout->addWidget(line);

        label = new QLabel(ActionGroupUi);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 13));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        groupSeparator = new QFrame(ActionGroupUi);
        groupSeparator->setObjectName(QString::fromUtf8("groupSeparator"));
        groupSeparator->setStyleSheet(QString::fromUtf8("color: #c0c0c0;"));
        groupSeparator->setFrameShadow(QFrame::Plain);
        groupSeparator->setFrameShape(QFrame::VLine);

        gridLayout->addWidget(groupSeparator, 0, 1, 1, 1);


        retranslateUi(ActionGroupUi);

        QMetaObject::connectSlotsByName(ActionGroupUi);
    } // setupUi

    void retranslateUi(QWidget *ActionGroupUi)
    {
        ActionGroupUi->setWindowTitle(QCoreApplication::translate("ActionGroupUi", "Form", nullptr));
        label->setText(QCoreApplication::translate("ActionGroupUi", "Toolbar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ActionGroupUi: public Ui_ActionGroupUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTIONGROUP_H

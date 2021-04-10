/********************************************************************************
** Form generated from reading UI file 'DelayNodeUi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELAYNODEUI_H
#define UI_DELAYNODEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DelayNodeUiForm
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QLabel *label;
    QSpinBox *spinBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *DelayNodeUiForm)
    {
        if (DelayNodeUiForm->objectName().isEmpty())
            DelayNodeUiForm->setObjectName(QString::fromUtf8("DelayNodeUiForm"));
        DelayNodeUiForm->resize(338, 115);
        gridLayout = new QGridLayout(DelayNodeUiForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(DelayNodeUiForm);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        label = new QLabel(splitter);
        label->setObjectName(QString::fromUtf8("label"));
        splitter->addWidget(label);
        spinBox = new QSpinBox(splitter);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(100);
        spinBox->setMaximum(999999999);
        spinBox->setSingleStep(100);
        spinBox->setValue(1000);
        splitter->addWidget(spinBox);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(DelayNodeUiForm);

        QMetaObject::connectSlotsByName(DelayNodeUiForm);
    } // setupUi

    void retranslateUi(QWidget *DelayNodeUiForm)
    {
        DelayNodeUiForm->setWindowTitle(QCoreApplication::translate("DelayNodeUiForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("DelayNodeUiForm", "Duration (ms) :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DelayNodeUiForm: public Ui_DelayNodeUiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELAYNODEUI_H

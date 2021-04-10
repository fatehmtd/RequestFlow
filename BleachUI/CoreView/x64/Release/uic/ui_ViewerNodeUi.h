/********************************************************************************
** Form generated from reading UI file 'ViewerNodeUi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWERNODEUI_H
#define UI_VIEWERNODEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewerWidgetUiForm
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_raw;
    QGridLayout *gridLayout_4;
    QTextEdit *textEdit_raw;
    QWidget *tab_json;
    QGridLayout *gridLayout_3;
    QTextEdit *textEdit_json;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *ViewerWidgetUiForm)
    {
        if (ViewerWidgetUiForm->objectName().isEmpty())
            ViewerWidgetUiForm->setObjectName(QString::fromUtf8("ViewerWidgetUiForm"));
        ViewerWidgetUiForm->resize(412, 377);
        gridLayout_2 = new QGridLayout(ViewerWidgetUiForm);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(ViewerWidgetUiForm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(groupBox);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_raw = new QWidget();
        tab_raw->setObjectName(QString::fromUtf8("tab_raw"));
        gridLayout_4 = new QGridLayout(tab_raw);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textEdit_raw = new QTextEdit(tab_raw);
        textEdit_raw->setObjectName(QString::fromUtf8("textEdit_raw"));

        gridLayout_4->addWidget(textEdit_raw, 0, 0, 1, 1);

        tabWidget->addTab(tab_raw, QString());
        tab_json = new QWidget();
        tab_json->setObjectName(QString::fromUtf8("tab_json"));
        gridLayout_3 = new QGridLayout(tab_json);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        textEdit_json = new QTextEdit(tab_json);
        textEdit_json->setObjectName(QString::fromUtf8("textEdit_json"));

        gridLayout_3->addWidget(textEdit_json, 0, 0, 1, 1);

        tabWidget->addTab(tab_json, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(310, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(ViewerWidgetUiForm);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(ViewerWidgetUiForm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ViewerWidgetUiForm);
    } // setupUi

    void retranslateUi(QWidget *ViewerWidgetUiForm)
    {
        ViewerWidgetUiForm->setWindowTitle(QCoreApplication::translate("ViewerWidgetUiForm", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ViewerWidgetUiForm", "Visualization :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_raw), QCoreApplication::translate("ViewerWidgetUiForm", "Raw", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_json), QCoreApplication::translate("ViewerWidgetUiForm", "JSON", nullptr));
        pushButton->setText(QCoreApplication::translate("ViewerWidgetUiForm", "Save to file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewerWidgetUiForm: public Ui_ViewerWidgetUiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWERNODEUI_H

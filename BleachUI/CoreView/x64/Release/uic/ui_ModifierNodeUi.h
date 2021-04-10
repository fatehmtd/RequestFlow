/********************************************************************************
** Form generated from reading UI file 'ModifierNodeUi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFIERNODEUI_H
#define UI_MODIFIERNODEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModifierNodeUiForm
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *ModifierNodeUiForm)
    {
        if (ModifierNodeUiForm->objectName().isEmpty())
            ModifierNodeUiForm->setObjectName(QString::fromUtf8("ModifierNodeUiForm"));
        ModifierNodeUiForm->resize(649, 571);
        gridLayout = new QGridLayout(ModifierNodeUiForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(ModifierNodeUiForm);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 20)
            tableWidget->setRowCount(20);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setRowCount(20);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setHighlightSections(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(true);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        retranslateUi(ModifierNodeUiForm);

        QMetaObject::connectSlotsByName(ModifierNodeUiForm);
    } // setupUi

    void retranslateUi(QWidget *ModifierNodeUiForm)
    {
        ModifierNodeUiForm->setWindowTitle(QCoreApplication::translate("ModifierNodeUiForm", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ModifierNodeUiForm", "Path", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ModifierNodeUiForm", "Input", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ModifierNodeUiForm", "Output", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifierNodeUiForm: public Ui_ModifierNodeUiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFIERNODEUI_H

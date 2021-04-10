/********************************************************************************
** Form generated from reading UI file 'EnvironmentsWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVIRONMENTSWIDGET_H
#define UI_ENVIRONMENTSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EnvironmentsWidget
{
public:
    QGridLayout *gridLayout;
    QToolButton *toolButton_remove;
    QToolButton *toolButton_add;
    QTableWidget *tableWidget_entries;
    QComboBox *comboBox_environments;

    void setupUi(QWidget *EnvironmentsWidget)
    {
        if (EnvironmentsWidget->objectName().isEmpty())
            EnvironmentsWidget->setObjectName(QString::fromUtf8("EnvironmentsWidget"));
        EnvironmentsWidget->resize(344, 340);
        gridLayout = new QGridLayout(EnvironmentsWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        toolButton_remove = new QToolButton(EnvironmentsWidget);
        toolButton_remove->setObjectName(QString::fromUtf8("toolButton_remove"));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8(":/ui/minus")));
        toolButton_remove->setIcon(icon);

        gridLayout->addWidget(toolButton_remove, 0, 2, 1, 1);

        toolButton_add = new QToolButton(EnvironmentsWidget);
        toolButton_add->setObjectName(QString::fromUtf8("toolButton_add"));
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8(":/ui/plus")));
        toolButton_add->setIcon(icon1);

        gridLayout->addWidget(toolButton_add, 0, 1, 1, 1);

        tableWidget_entries = new QTableWidget(EnvironmentsWidget);
        if (tableWidget_entries->columnCount() < 2)
            tableWidget_entries->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_entries->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_entries->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_entries->setObjectName(QString::fromUtf8("tableWidget_entries"));
        tableWidget_entries->setAlternatingRowColors(true);
        tableWidget_entries->setSortingEnabled(true);
        tableWidget_entries->horizontalHeader()->setStretchLastSection(true);

        gridLayout->addWidget(tableWidget_entries, 1, 0, 1, 3);

        comboBox_environments = new QComboBox(EnvironmentsWidget);
        comboBox_environments->setObjectName(QString::fromUtf8("comboBox_environments"));

        gridLayout->addWidget(comboBox_environments, 0, 0, 1, 1);


        retranslateUi(EnvironmentsWidget);

        QMetaObject::connectSlotsByName(EnvironmentsWidget);
    } // setupUi

    void retranslateUi(QWidget *EnvironmentsWidget)
    {
        EnvironmentsWidget->setWindowTitle(QCoreApplication::translate("EnvironmentsWidget", "Environments", nullptr));
        toolButton_remove->setText(QCoreApplication::translate("EnvironmentsWidget", "-", nullptr));
        toolButton_add->setText(QCoreApplication::translate("EnvironmentsWidget", "+", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_entries->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("EnvironmentsWidget", "Key", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_entries->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("EnvironmentsWidget", "Value", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EnvironmentsWidget: public Ui_EnvironmentsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVIRONMENTSWIDGET_H

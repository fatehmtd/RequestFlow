/********************************************************************************
** Form generated from reading UI file 'PayloadNodeUi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAYLOADNODEUI_H
#define UI_PAYLOADNODEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PayloadNodeUi
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_body;
    QGridLayout *gridLayout_2;
    QTextEdit *textEdit_body;
    QWidget *tab_query;
    QGridLayout *gridLayout_4;
    QTableWidget *tableWidget_query;
    QWidget *tab_path;
    QGridLayout *gridLayout_3;
    QTableWidget *tableWidget_path;

    void setupUi(QWidget *PayloadNodeUi)
    {
        if (PayloadNodeUi->objectName().isEmpty())
            PayloadNodeUi->setObjectName(QString::fromUtf8("PayloadNodeUi"));
        PayloadNodeUi->resize(400, 300);
        gridLayout = new QGridLayout(PayloadNodeUi);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(PayloadNodeUi);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_body = new QWidget();
        tab_body->setObjectName(QString::fromUtf8("tab_body"));
        gridLayout_2 = new QGridLayout(tab_body);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textEdit_body = new QTextEdit(tab_body);
        textEdit_body->setObjectName(QString::fromUtf8("textEdit_body"));

        gridLayout_2->addWidget(textEdit_body, 0, 0, 1, 1);

        tabWidget->addTab(tab_body, QString());
        tab_query = new QWidget();
        tab_query->setObjectName(QString::fromUtf8("tab_query"));
        gridLayout_4 = new QGridLayout(tab_query);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tableWidget_query = new QTableWidget(tab_query);
        if (tableWidget_query->columnCount() < 2)
            tableWidget_query->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_query->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_query->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_query->setObjectName(QString::fromUtf8("tableWidget_query"));
        tableWidget_query->horizontalHeader()->setStretchLastSection(true);

        gridLayout_4->addWidget(tableWidget_query, 0, 0, 1, 1);

        tabWidget->addTab(tab_query, QString());
        tab_path = new QWidget();
        tab_path->setObjectName(QString::fromUtf8("tab_path"));
        gridLayout_3 = new QGridLayout(tab_path);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tableWidget_path = new QTableWidget(tab_path);
        if (tableWidget_path->columnCount() < 2)
            tableWidget_path->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_path->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_path->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        tableWidget_path->setObjectName(QString::fromUtf8("tableWidget_path"));
        tableWidget_path->horizontalHeader()->setStretchLastSection(true);

        gridLayout_3->addWidget(tableWidget_path, 0, 0, 1, 1);

        tabWidget->addTab(tab_path, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(PayloadNodeUi);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PayloadNodeUi);
    } // setupUi

    void retranslateUi(QWidget *PayloadNodeUi)
    {
        PayloadNodeUi->setWindowTitle(QCoreApplication::translate("PayloadNodeUi", "PayloadNode", nullptr));
        textEdit_body->setPlaceholderText(QCoreApplication::translate("PayloadNodeUi", "{ \"body\" : {} }", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_body), QCoreApplication::translate("PayloadNodeUi", "Body", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_query->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("PayloadNodeUi", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_query->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("PayloadNodeUi", "Value", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_query), QCoreApplication::translate("PayloadNodeUi", "Query", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_path->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("PayloadNodeUi", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_path->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("PayloadNodeUi", "Value", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_path), QCoreApplication::translate("PayloadNodeUi", "Path", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PayloadNodeUi: public Ui_PayloadNodeUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAYLOADNODEUI_H

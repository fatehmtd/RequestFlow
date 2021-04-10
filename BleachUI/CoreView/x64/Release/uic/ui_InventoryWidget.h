/********************************************************************************
** Form generated from reading UI file 'InventoryWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INVENTORYWIDGET_H
#define UI_INVENTORYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InventoryWidget
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QTreeView *treeView;

    void setupUi(QWidget *InventoryWidget)
    {
        if (InventoryWidget->objectName().isEmpty())
            InventoryWidget->setObjectName(QString::fromUtf8("InventoryWidget"));
        InventoryWidget->resize(355, 317);
        gridLayout = new QGridLayout(InventoryWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit = new QLineEdit(InventoryWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 0, 1, 1);

        toolButton = new QToolButton(InventoryWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        gridLayout->addWidget(toolButton, 0, 1, 1, 1);

        toolButton_2 = new QToolButton(InventoryWidget);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        gridLayout->addWidget(toolButton_2, 0, 2, 1, 1);

        treeView = new QTreeView(InventoryWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 1, 0, 1, 3);


        retranslateUi(InventoryWidget);

        QMetaObject::connectSlotsByName(InventoryWidget);
    } // setupUi

    void retranslateUi(QWidget *InventoryWidget)
    {
        InventoryWidget->setWindowTitle(QCoreApplication::translate("InventoryWidget", "InventoryWidget", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("InventoryWidget", "Filter...", nullptr));
        toolButton->setText(QCoreApplication::translate("InventoryWidget", "...", nullptr));
        toolButton_2->setText(QCoreApplication::translate("InventoryWidget", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InventoryWidget: public Ui_InventoryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INVENTORYWIDGET_H

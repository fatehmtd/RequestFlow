/********************************************************************************
** Form generated from reading UI file 'EndpointNodeUi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENDPOINTNODEUI_H
#define UI_ENDPOINTNODEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EndpointNodeUiWidget
{
public:
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QTabWidget *tabWidget_2;
    QWidget *tab;
    QGridLayout *gridLayout_10;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *comboBox_method;
    QLabel *label_2;
    QLineEdit *lineEdit_url;
    QLabel *label_3;
    QComboBox *comboBox_contentType;
    QLabel *label_8;
    QSpinBox *spinBox_timeout;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_9;
    QLabel *label_4;
    QComboBox *comboBox_authType;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QLabel *label_6;
    QLineEdit *lineEdit_2;
    QLabel *label_7;
    QLineEdit *lineEdit_3;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_json;
    QGridLayout *gridLayout_8;
    QPlainTextEdit *plainTextEdit_json;
    QWidget *tab_raw;
    QGridLayout *gridLayout_5;
    QPlainTextEdit *plainTextEdit_raw;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QPlainTextEdit *plainTextEdit_response;

    void setupUi(QWidget *EndpointNodeUiWidget)
    {
        if (EndpointNodeUiWidget->objectName().isEmpty())
            EndpointNodeUiWidget->setObjectName(QString::fromUtf8("EndpointNodeUiWidget"));
        EndpointNodeUiWidget->resize(533, 602);
        gridLayout_7 = new QGridLayout(EndpointNodeUiWidget);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(EndpointNodeUiWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tabWidget_2 = new QTabWidget(groupBox_3);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_10 = new QGridLayout(tab);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        comboBox_method = new QComboBox(groupBox);
        comboBox_method->addItem(QString());
        comboBox_method->addItem(QString());
        comboBox_method->addItem(QString());
        comboBox_method->addItem(QString());
        comboBox_method->addItem(QString());
        comboBox_method->setObjectName(QString::fromUtf8("comboBox_method"));

        gridLayout->addWidget(comboBox_method, 0, 2, 1, 2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_url = new QLineEdit(groupBox);
        lineEdit_url->setObjectName(QString::fromUtf8("lineEdit_url"));

        gridLayout->addWidget(lineEdit_url, 1, 1, 1, 3);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 3);

        comboBox_contentType = new QComboBox(groupBox);
        comboBox_contentType->addItem(QString());
        comboBox_contentType->addItem(QString());
        comboBox_contentType->addItem(QString());
        comboBox_contentType->setObjectName(QString::fromUtf8("comboBox_contentType"));

        gridLayout->addWidget(comboBox_contentType, 2, 3, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 3, 0, 1, 2);

        spinBox_timeout = new QSpinBox(groupBox);
        spinBox_timeout->setObjectName(QString::fromUtf8("spinBox_timeout"));
        spinBox_timeout->setMinimum(10);
        spinBox_timeout->setMaximum(999999999);
        spinBox_timeout->setSingleStep(100);
        spinBox_timeout->setValue(3000);

        gridLayout->addWidget(spinBox_timeout, 3, 3, 1, 1);


        gridLayout_10->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_9 = new QGridLayout(groupBox_4);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_9->addWidget(label_4, 0, 0, 1, 1);

        comboBox_authType = new QComboBox(groupBox_4);
        comboBox_authType->addItem(QString());
        comboBox_authType->addItem(QString());
        comboBox_authType->setObjectName(QString::fromUtf8("comboBox_authType"));

        gridLayout_9->addWidget(comboBox_authType, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer, 0, 2, 2, 1);

        groupBox_5 = new QGroupBox(groupBox_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_3 = new QGridLayout(groupBox_5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox_5);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_3->addWidget(lineEdit, 0, 1, 1, 1);

        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        lineEdit_2 = new QLineEdit(groupBox_5);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setEchoMode(QLineEdit::PasswordEchoOnEdit);

        gridLayout_3->addWidget(lineEdit_2, 1, 1, 1, 1);

        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 2, 0, 1, 1);

        lineEdit_3 = new QLineEdit(groupBox_5);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setReadOnly(true);

        gridLayout_3->addWidget(lineEdit_3, 2, 1, 1, 1);

        checkBox = new QCheckBox(groupBox_5);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_3->addWidget(checkBox, 3, 1, 1, 1);


        gridLayout_9->addWidget(groupBox_5, 1, 0, 2, 2);

        horizontalSpacer_2 = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_2, 3, 1, 1, 1);


        gridLayout_10->addWidget(groupBox_4, 1, 0, 1, 1);

        tabWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget = new QTabWidget(tab_2);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_json = new QWidget();
        tab_json->setObjectName(QString::fromUtf8("tab_json"));
        gridLayout_8 = new QGridLayout(tab_json);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        plainTextEdit_json = new QPlainTextEdit(tab_json);
        plainTextEdit_json->setObjectName(QString::fromUtf8("plainTextEdit_json"));
        plainTextEdit_json->setReadOnly(true);

        gridLayout_8->addWidget(plainTextEdit_json, 0, 0, 1, 1);

        tabWidget->addTab(tab_json, QString());
        tab_raw = new QWidget();
        tab_raw->setObjectName(QString::fromUtf8("tab_raw"));
        gridLayout_5 = new QGridLayout(tab_raw);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        plainTextEdit_raw = new QPlainTextEdit(tab_raw);
        plainTextEdit_raw->setObjectName(QString::fromUtf8("plainTextEdit_raw"));
        plainTextEdit_raw->setReadOnly(true);

        gridLayout_5->addWidget(plainTextEdit_raw, 0, 0, 1, 1);

        tabWidget->addTab(tab_raw, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);

        tabWidget_2->addTab(tab_2, QString());

        gridLayout_4->addWidget(tabWidget_2, 0, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_3, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(EndpointNodeUiWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        plainTextEdit_response = new QPlainTextEdit(groupBox_2);
        plainTextEdit_response->setObjectName(QString::fromUtf8("plainTextEdit_response"));
        plainTextEdit_response->setReadOnly(true);

        gridLayout_6->addWidget(plainTextEdit_response, 0, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_2, 1, 0, 1, 1);


        retranslateUi(EndpointNodeUiWidget);

        tabWidget_2->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(EndpointNodeUiWidget);
    } // setupUi

    void retranslateUi(QWidget *EndpointNodeUiWidget)
    {
        EndpointNodeUiWidget->setWindowTitle(QCoreApplication::translate("EndpointNodeUiWidget", "EndpointNode", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("EndpointNodeUiWidget", "Request :", nullptr));
        groupBox->setTitle(QCoreApplication::translate("EndpointNodeUiWidget", "Connection : ", nullptr));
        label->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Method :", nullptr));
        comboBox_method->setItemText(0, QCoreApplication::translate("EndpointNodeUiWidget", "GET", nullptr));
        comboBox_method->setItemText(1, QCoreApplication::translate("EndpointNodeUiWidget", "POST", nullptr));
        comboBox_method->setItemText(2, QCoreApplication::translate("EndpointNodeUiWidget", "PUT", nullptr));
        comboBox_method->setItemText(3, QCoreApplication::translate("EndpointNodeUiWidget", "PATCH", nullptr));
        comboBox_method->setItemText(4, QCoreApplication::translate("EndpointNodeUiWidget", "DEL", nullptr));

        label_2->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Url :", nullptr));
        lineEdit_url->setPlaceholderText(QCoreApplication::translate("EndpointNodeUiWidget", "http://endpoint.url", nullptr));
        label_3->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Content-Type :", nullptr));
        comboBox_contentType->setItemText(0, QCoreApplication::translate("EndpointNodeUiWidget", "application/json", nullptr));
        comboBox_contentType->setItemText(1, QCoreApplication::translate("EndpointNodeUiWidget", "text/plain", nullptr));
        comboBox_contentType->setItemText(2, QCoreApplication::translate("EndpointNodeUiWidget", "text/html", nullptr));

        label_8->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Timeout (msec) : ", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("EndpointNodeUiWidget", "Authentication :", nullptr));
        label_4->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Method :", nullptr));
        comboBox_authType->setItemText(0, QCoreApplication::translate("EndpointNodeUiWidget", "None", nullptr));
        comboBox_authType->setItemText(1, QCoreApplication::translate("EndpointNodeUiWidget", "Basic", nullptr));

        groupBox_5->setTitle(QCoreApplication::translate("EndpointNodeUiWidget", "Credentials :", nullptr));
        label_5->setText(QCoreApplication::translate("EndpointNodeUiWidget", "User :", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("EndpointNodeUiWidget", "admin", nullptr));
        label_6->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Pwd : ", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("EndpointNodeUiWidget", "admin", nullptr));
        label_7->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Passphrase :", nullptr));
        checkBox->setText(QCoreApplication::translate("EndpointNodeUiWidget", "Preemptive authentication", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QCoreApplication::translate("EndpointNodeUiWidget", "Parameters", nullptr));
        plainTextEdit_json->setPlaceholderText(QCoreApplication::translate("EndpointNodeUiWidget", "{...}", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_json), QCoreApplication::translate("EndpointNodeUiWidget", "JSON", nullptr));
        plainTextEdit_raw->setPlaceholderText(QCoreApplication::translate("EndpointNodeUiWidget", "{...}", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_raw), QCoreApplication::translate("EndpointNodeUiWidget", "Raw", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QCoreApplication::translate("EndpointNodeUiWidget", "Payload", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("EndpointNodeUiWidget", "Response :", nullptr));
        plainTextEdit_response->setPlaceholderText(QCoreApplication::translate("EndpointNodeUiWidget", "Console ...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EndpointNodeUiWidget: public Ui_EndpointNodeUiWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENDPOINTNODEUI_H

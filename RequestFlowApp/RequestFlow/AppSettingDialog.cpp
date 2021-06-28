#include "AppSettingDialog.h"
#include "ui_AppSettingDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QNetworkProxy>

AppSettingDialog::AppSettingDialog(view::SettingsManager *settingsManager, QWidget *parent) : QDialog(parent), ui(new Ui::AppSettingDialog), _settingsManager(settingsManager)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);

    _networkAccessManager = new QNetworkAccessManager(this);
    _networkAccessManager->setTransferTimeout(30000);

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &AppSettingDialog::onDialogButtonClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(ui->pushButton_checkConnection, &QPushButton::clicked, this, &AppSettingDialog::attemptConnection);

    loadSettings();
}

AppSettingDialog::~AppSettingDialog()
{
    delete ui;
}

void AppSettingDialog::onDialogButtonClicked(QAbstractButton *button)
{
    switch(ui->buttonBox->buttonRole(button))
    {
    case QDialogButtonBox::ButtonRole::ApplyRole:
    case QDialogButtonBox::ButtonRole::AcceptRole:
        applySettings();
        break;
    default:
        break;
    }
}

void AppSettingDialog::onReplyReceived()
{
    ui->pushButton_checkConnection->setEnabled(true);
    ui->lineEdit_connectionAddress->setEnabled(true);
    ui->progressBar->setVisible(false);

    if(!_faillureStatus)
    {
        QMessageBox::information(this, "Connection Status", "Success");
    }
}

void AppSettingDialog::onErrorOccured(QNetworkReply::NetworkError error)
{
    ui->pushButton_checkConnection->setEnabled(true);
    ui->lineEdit_connectionAddress->setEnabled(true);
    ui->progressBar->setVisible(false);

    _faillureStatus = true;
    auto networkReply = dynamic_cast<QNetworkReply*>(sender());

    QMessageBox::critical(this, "Connection Status", QString("Error : %1").arg(networkReply->errorString()));
}

void AppSettingDialog::attemptConnection()
{
    handleProxySettings();

    ui->progressBar->setVisible(true);
    ui->pushButton_checkConnection->setEnabled(false);
    ui->lineEdit_connectionAddress->setEnabled(false);

    _faillureStatus = false;
    QNetworkRequest request(QUrl(ui->lineEdit_connectionAddress->text()));
    auto reply = _networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &AppSettingDialog::onReplyReceived);
    connect(reply, &QNetworkReply::errorOccurred, this, &AppSettingDialog::onErrorOccured);
}

void AppSettingDialog::applySettings()
{
    // proxy
    handleProxySettings();
    view::SettingsManager::ProxyConfig proxy;
    proxy.authNeeded = ui->groupBox_authentication->isChecked();
    proxy.userName = ui->lineEdit_userName->text();
    proxy.password = ui->lineEdit_password->text();
    proxy.hostName = ui->lineEdit_hostName->text();
    proxy.port = ui->spinBox_portNumber->value();
    proxy.proxyType = ui->radioButton_http->isChecked() ? QNetworkProxy::HttpProxy : QNetworkProxy::Socks5Proxy;
    _settingsManager->setProxyStatus(ui->radioButton_manualProxy->isChecked());
    _settingsManager->setProxyConfig(proxy);
    qDebug() << __FUNCTION__ << proxy.proxyType;
}

void AppSettingDialog::loadSettings()
{
    // proxy
    ui->radioButton_manualProxy->setChecked(_settingsManager->getProxyStatus());
    auto proxy = _settingsManager->getProxyConfig();
    ui->radioButton_http->setChecked(proxy.proxyType == QNetworkProxy::HttpProxy);
    ui->radioButton_socks->setChecked(proxy.proxyType == QNetworkProxy::Socks5Proxy);
    ui->lineEdit_hostName->setText(proxy.hostName);
    ui->lineEdit_userName->setText(proxy.userName);
    ui->lineEdit_password->setText(proxy.password);
    ui->spinBox_portNumber->setValue(proxy.port);
    ui->groupBox_authentication->setChecked(proxy.authNeeded);
}

void AppSettingDialog::handleProxySettings()
{
    QNetworkProxy proxy;
    if(ui->radioButton_manualProxy->isChecked())
    {
        proxy.setType(ui->radioButton_http->isChecked() ? QNetworkProxy::ProxyType::HttpProxy : QNetworkProxy::ProxyType::Socks5Proxy);
        proxy.setPort(ui->spinBox_portNumber->value());
        proxy.setHostName(ui->lineEdit_hostName->text());
        if(ui->groupBox_authentication->isChecked())
        {
            proxy.setUser(ui->lineEdit_userName->text());
            proxy.setPassword(ui->lineEdit_password->text());
        }
    }
    else
    {
        proxy.setType(QNetworkProxy::ProxyType::NoProxy);
    }

    QNetworkProxy::setApplicationProxy(proxy);
}

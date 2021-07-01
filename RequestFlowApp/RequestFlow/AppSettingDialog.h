#ifndef APPSETTINGDIALOG_H
#define APPSETTINGDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "SettingsManager.h"

namespace Ui {
class AppSettingDialog;
}

class AppSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettingDialog(view::SettingsManager *settingsManager, QWidget *parent = nullptr);
    ~AppSettingDialog();

private slots:
    void onDialogButtonClicked(QAbstractButton* button);
    void onReplyReceived();
    void onErrorOccured(QNetworkReply::NetworkError error);
    void attemptConnection();

private:
    void applySettings();
    void loadSettings();
    void handleProxySettings();

    void setUiStatus(bool status);

private:
    bool _faillureStatus = false;
    Ui::AppSettingDialog *ui;
    QNetworkAccessManager* _networkAccessManager = nullptr;
    view::SettingsManager *_settingsManager=nullptr;
};

#endif // APPSETTINGDIALOG_H

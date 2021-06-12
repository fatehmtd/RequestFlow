#ifndef APPSETTINGDIALOG_H
#define APPSETTINGDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class AppSettingDialog;
}

class AppSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettingDialog(QWidget *parent = nullptr);
    ~AppSettingDialog();

private slots:
    void onButtonClicked(QAbstractButton* button);

private:
    Ui::AppSettingDialog *ui;
};

#endif // APPSETTINGDIALOG_H

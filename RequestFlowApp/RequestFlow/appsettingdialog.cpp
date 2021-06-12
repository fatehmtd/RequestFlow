#include "appsettingdialog.h"
#include "ui_appsettingdialog.h"
#include <QDebug>

AppSettingDialog::AppSettingDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AppSettingDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &AppSettingDialog::onButtonClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

AppSettingDialog::~AppSettingDialog()
{
    delete ui;
}

void AppSettingDialog::onButtonClicked(QAbstractButton *button)
{
    switch(ui->buttonBox->buttonRole(button))
    {
    case QDialogButtonBox::ButtonRole::ApplyRole:
    case QDialogButtonBox::ButtonRole::AcceptRole:
        // TODO: apply changes
        break;
    default:
        break;
    }
}

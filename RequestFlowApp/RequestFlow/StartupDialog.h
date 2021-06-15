#pragma once

#include "ui_StartupDialog.h"

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupDialog(QWidget *parent = nullptr);

private:
    Ui::StartupDialog ui;
};


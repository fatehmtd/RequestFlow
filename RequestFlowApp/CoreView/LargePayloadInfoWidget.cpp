#include "LargePayloadInfoWidget.h"
#include "ui_LargePayloadInfoWidget.h"

LargePayloadInfoWidget::LargePayloadInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LargePayloadInfoWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_viewWidget, &QPushButton::clicked, this, &LargePayloadInfoWidget::viewClicked);
}

LargePayloadInfoWidget::~LargePayloadInfoWidget()
{
    delete ui;
}

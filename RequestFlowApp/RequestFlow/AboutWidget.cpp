#include "AboutWidget.h"
#include "ui_AboutWidget.h"
#include <QImage>
#include <QPixmap>

#include "AppVersion.h"

AboutWidget::AboutWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
    //QImage img(":/ui/network");

    QPixmap pixmap(":/ui/requestflow-grey-256");
    ui->label_logo->setPixmap(pixmap.scaledToWidth(128, Qt::TransformationMode::SmoothTransformation));
    //setWindowFlag(Qt::WindowType::FramelessWindowHint);
    //setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    //setWindowFlag(Qt::WindowType::WindowMinMaxButtonsHint, false);
    //setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    ui->label_version->setText(QString("Version : %1").arg(APP_VERSION));
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

#include "AboutWidget.h"
#include "ui_AboutWidget.h"
#include <QPixmap>
#include <QImage>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
    //QImage img(":/ui/network");

    QPixmap pixmap(":/ui/network");
    ui->label_logo->setPixmap(pixmap.scaledToWidth(128, Qt::TransformationMode::SmoothTransformation));
    //setWindowFlag(Qt::WindowType::FramelessWindowHint);
    //setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    //setWindowFlag(Qt::WindowType::WindowMinMaxButtonsHint, false);
    //setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

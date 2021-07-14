#include "ContentWidget.h"
#include <QPaintEvent>

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>

view::ContentWidget::ContentWidget(QWidget* widget)
    : QWidget(widget)
{
    _progressBar = new QProgressBar(this);
    _progressBar->setTextVisible(false);
    _progressBar->setFixedHeight(15);
    _progressBar->setRange(0, 0);
    auto layout = new QVBoxLayout();
    //_layout->setMargin(0);
    //_layout->setSpacing(0);
    setLayout(layout);
    layout->addWidget(_progressBar);
    //setAttribute(Qt::WA_NoBackground, true);
    setAutoFillBackground(true);
    setStyleSheet("color: black; ");
    disableProgressBar();
}

void view::ContentWidget::enableProgressBar()
{
    _progressBar->setVisible(true);
}

void view::ContentWidget::disableProgressBar()
{
    _progressBar->setVisible(false);
}

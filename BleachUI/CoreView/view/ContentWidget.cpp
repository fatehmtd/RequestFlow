#include "ContentWidget.h"
#include <QPaintEvent>

#include <QDebug>
#include <QCoreApplication>
#include <QApplication>

view::ContentWidget::ContentWidget(QWidget* widget) : QWidget(widget)
{
	auto layout = new QVBoxLayout();
	//_layout->setMargin(0);
	//_layout->setSpacing(0);
	setLayout(layout);
	setAttribute(Qt::WA_NoBackground, true);
	setAutoFillBackground(true);
	//qDebug() << qApp->styleSheet();
	setStyleSheet("color: black; ");
}
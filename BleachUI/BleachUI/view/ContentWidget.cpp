#include "ContentWidget.h"

view::ContentWidget::ContentWidget(QWidget* widget) : QWidget(widget)
{
	_layout = new QVBoxLayout();
	//_layout->setMargin(0);
	//_layout->setSpacing(0);
	setLayout(_layout);
	setAttribute(Qt::WA_NoBackground, true);
	setAutoFillBackground(true);
	//setStyleSheet("background: transparent; color: white");
}
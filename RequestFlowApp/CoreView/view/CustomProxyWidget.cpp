#include "CustomProxyWidget.h"
#include <QGraphicsSceneResizeEvent>
#include <QDebug>
#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QStyleOptionGraphicsItem>

#include <QApplication>

void view::CustomProxyWidget::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{/*
	if (_refreshNeeded)
	{
		auto bg = qApp->palette().color(QPalette::ColorRole::Background);
		QBrush brush(bg);

		QPainter tempPainter(&_pixmap);

		auto style = new QStyleOptionGraphicsItem(*option);
		style->exposedRect.setWidth(_pixmap.width());
		style->exposedRect.setHeight(_pixmap.height());

		tempPainter.setBrush(brush);
		tempPainter.setPen(Qt::NoPen);
		tempPainter.fillRect(_pixmap.rect(), brush);

		QGraphicsProxyWidget::paint(&tempPainter, style, widget);

		delete style;
		_refreshNeeded = false;
	}

	painter->drawPixmap(this->widget()->rect(), _pixmap);
	*/
	//setAutoFillBackground(true);
	QGraphicsProxyWidget::paint(painter, option, widget);
}

bool view::CustomProxyWidget::eventFilter(QObject* object, QEvent* event)
{
    /*
	bool isupdate = false;
	switch (event->type())
	{
	case QEvent::Type::GraphicsSceneResize:
	case QEvent::Type::Resize:
	{
		_pixmap = QPixmap(widget()->size());
	}
	break;
	case QEvent::Type::GraphicsSceneHoverEnter:
	case QEvent::Type::GraphicsSceneHoverMove:
	case QEvent::Type::GraphicsSceneHoverLeave:
	case QEvent::Type::GraphicsSceneMouseMove:
	case QEvent::Type::GraphicsSceneMousePress:
	case QEvent::Type::GraphicsSceneMouseRelease:
		break;
	case QEvent::Type::UpdateLater:
	case QEvent::Type::Paint:
	case QEvent::Type::UpdateRequest:
		isupdate = true;
		break;
	}
	if (!isupdate)
	{
		//widget()->update();
	}
	_refreshNeeded = true;
//*/
	return QGraphicsProxyWidget::eventFilter(object, event);
}

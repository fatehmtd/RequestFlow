#pragma once

#include <QPainter>
#include <QGraphicsProxyWidget>
#include <QPixmap>
#include <QTimer>

namespace view
{
	class CustomProxyWidget : public QGraphicsProxyWidget
	{
	public:
		using QGraphicsProxyWidget::QGraphicsProxyWidget;
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

		bool eventFilter(QObject* object, QEvent* event) override;
	protected:
		QPixmap _pixmap;
		bool _refreshNeeded = false;
	};
}
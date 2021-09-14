#include "CustomProxyWidget.h"
#include <QPainter>

view::CustomProxyWidget::CustomProxyWidget(QGraphicsItem* parent)
    : QGraphicsProxyWidget(parent)
{
}

void view::CustomProxyWidget::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{ /*
    if (_isDirty) {
        auto rect = geometry();
        _cachedPixmap = QPixmap(rect.width(), rect.height());
        QPainter localPainter(&_cachedPixmap);
        QGraphicsProxyWidget::paint(&localPainter, option, widget);
        _isDirty = false;
    } else {
        painter->drawPixmap(0, 0, _cachedPixmap.width(), _cachedPixmap.height(), _cachedPixmap,
            0, 0, _cachedPixmap.width(), _cachedPixmap.height());
        //_isDirty = true;
    }*/
    QGraphicsProxyWidget::paint(painter, option, widget);
}

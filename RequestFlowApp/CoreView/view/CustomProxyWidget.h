#pragma once
#include <QGraphicsProxyWidget>
#include <QPixmap>

namespace view {
class CustomProxyWidget : public QGraphicsProxyWidget {
public:
    CustomProxyWidget(QGraphicsItem* parent);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool _isDirty = true;
    QPixmap _cachedPixmap;
};
}

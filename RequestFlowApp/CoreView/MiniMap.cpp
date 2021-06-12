#include "MiniMap.h"

view::MiniMap::MiniMap()
{
    _anchor = Anchor::BOTTOM_RIGHT;
    _coords = QPointF(0, 0);
    _size = QSize(200, 200);
}

#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>

#include <QDebug>

#include "view/SceneGraphWidget.h"

void view::MiniMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath backgroundPath;
    //auto bgColor = colors::light::purple;
    auto bgColor = QColor("#F0F0F0");
    QPen pen(bgColor);

    //qDebug() << __FUNCTION__ << _coords << _size;

    QBrush backgroundBrush(bgColor);
    backgroundPath.setFillRule(Qt::FillRule::WindingFill);
    backgroundPath.addRoundedRect(_coords.x(), _coords.y(), _size.width(), _size.height(), 0, 0);
    painter->setPen(pen);
    painter->setBrush(backgroundBrush);
    painter->drawPath(backgroundPath);
}

QRectF view::MiniMap::boundingRect() const
{
    return QRectF(0, 0, _size.width(), _size.height());
}


view::MiniMap::Anchor view::MiniMap::anchor() const
{
    return _anchor;
}

void view::MiniMap::setAnchor(Anchor newAnchor)
{
    _anchor = newAnchor;
}

const QSize &view::MiniMap::size() const
{
    return _size;
}

void view::MiniMap::setSize(const QSize &newSize)
{
    _size = newSize;
}


QPointF view::MiniMap::coords() const
{
    return _coords;
}

void view::MiniMap::setCoords(QPointF newCoords)
{
    _coords = newCoords;
}

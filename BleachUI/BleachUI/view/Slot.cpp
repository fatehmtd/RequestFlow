#include "Slot.h"
#include <QPainterPath>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include "Colors.h"

view::Slot::Slot(Node* parent, bool isInput, Style style) : QGraphicsItem(parent), _style(style), _isInput(isInput)
{
    //_style = isInput ? SQUARE : TRIANGLE;
    _style = CIRCLE;
    _size = 10;
    setFlag(GraphicsItemFlag::ItemIsSelectable);
    setFlag(GraphicsItemFlag::ItemIsMovable);
    setAcceptHoverEvents(true);

    _title = new QGraphicsTextItem(this);
    _title->setPlainText("Slot Name");
    _title->setDefaultTextColor(Qt::white);
    //_title->setTextWidth(2);
}

view::Slot::~Slot()
{
}

bool view::Slot::isInput() const
{
    return _isInput;
}

QRectF view::Slot::boundingRect() const
{
    const float halfSize = (float)_size * 0.5;
    auto basePos = getBasePosition();
    return QRectF(basePos .x()-halfSize, basePos.y()-halfSize, _size, _size);
}

void view::Slot::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    QPainterPath path, outline;
    QPen pen(colors::purple);
    QBrush brush(isInput() ? colors::green : colors::red);
    
    path.setFillRule(Qt::FillRule::WindingFill);
    painter->setPen(pen);
    painter->setBrush(brush);
    const float halfSize = _size * 0.5f;


    auto basePos = getBasePosition();

    auto textRect = _title->boundingRect();
    
    float outlineSize = _size * 1.05f;
    float outlineHalfSize = outlineSize * 0.5f;

    float textX = isInput() ? halfSize : (basePos.x() - (textRect.width() + halfSize));
    _title->setPos(textX, basePos.y() - textRect.height()*0.5f);

    switch (_style)
    {
    case CIRCLE:
    {
        path.addEllipse(basePos, halfSize, halfSize);
        outline.addEllipse(basePos, outlineSize * 0.5f, outlineSize * 0.5f);
    }
        break;
    case SQUARE:
    {
        path.addRoundedRect(basePos.x()-halfSize, basePos.y() -halfSize, _size, _size, 2, 2);
        float outlineSize = _size * 1.5f;
        float outlineHalfSize = outlineSize * 0.5f;
        outline.addRoundedRect(basePos.x() - outlineHalfSize, basePos.y() - outlineHalfSize, outlineSize, outlineSize, 2, 2);
    }
        break;
    case TRIANGLE:
    {
        float x = basePos.x() + halfSize*0.5f;
        QPolygon polygon;
        polygon.append(QPoint(x-halfSize, basePos.y()-halfSize));
        polygon.append(QPoint(x-halfSize, basePos.y()+halfSize));
        polygon.append(QPoint(x+halfSize, basePos.y()));
        path.addPolygon(polygon);

        QPolygon outlinePolygon;
        outlinePolygon.append(QPoint(x - outlineHalfSize, basePos.y() - outlineHalfSize));
        outlinePolygon.append(QPoint(x - outlineHalfSize, basePos.y() + outlineHalfSize));
        outlinePolygon.append(QPoint(x + outlineHalfSize, basePos.y()));
        outline.addPolygon(outlinePolygon);

    }
        break;
    }

    painter->drawPath(path.simplified());

    if (_mouseHover)
    {
        painter->setBrush(Qt::BrushStyle::NoBrush);
        QPen pen(QColor("#FFAAAA"), 3.0f);
        painter->setPen(pen);
        painter->drawPath(outline.simplified());
    }
}

void view::Slot::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
}

void view::Slot::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //setSelected(true);
    QGraphicsItem::mousePressEvent(event);
}

void view::Slot::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    //setSelected(false);
    QGraphicsItem::mouseReleaseEvent(event);
}

void view::Slot::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    _mouseHover = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void view::Slot::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsItem::hoverMoveEvent(event);
}

void view::Slot::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    _mouseHover = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

QPointF view::Slot::getBasePosition(bool global) const
{
    auto node = dynamic_cast<Node*>(parentItem());

    const int index = node->getSlotIndex(this);
    const float x = isInput() ? 0 : node->width();
    const float y = 35 + index * _title->boundingRect().height();

    auto p = QPointF(x, y);

    return global ? (p + scenePos()) : p;
}

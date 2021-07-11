#include "Slot.h"
#include "Colors.h"
#include "Node.h"
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <model/Slot.h>

view::Slot::Slot(Node* parent, model::Slot* slot)
    : QGraphicsSvgItem(parent)
    , _slot(slot)
{
    _size = 20;
    setFlag(GraphicsItemFlag::ItemIsSelectable);
    setAcceptDrops(true);

    setAcceptHoverEvents(true);
    QFont font;
    font.setPointSize(12);
    //font.setBold(true);
    _title = new QGraphicsTextItem(this);
    _title->setFont(font);
    _title->setPlainText(slot->getName());
    //_title->setDefaultTextColor(QColor(128, 128, 128));
    _title->setDefaultTextColor(Qt::white);
    //setZValue(-100);

    //_icon = new QGraphicsSvgItem(slot->getDirection() == model::Slot::INPUT ? ":/slots/input/normal" : ":/slots/output/normal", this);
    //_icon->setFlag(GraphicsItemFlag::ItemIsSelectable);
    //_icon->setAcceptTouchEvents(false);
    //_icon->setAcceptHoverEvents(false);
    //_icon->setVisible(false);
    //_icon->setAcceptedMouseButtons(false);

    auto renderer = new QSvgRenderer(QString(slot->getDirection() == model::Slot::INPUT ? ":/slots/input/normal" : ":/slots/output/normal"), parent);
    setSharedRenderer(renderer);
    setCacheMode(CacheMode::DeviceCoordinateCache);
}

view::Slot::~Slot()
{
}

bool view::Slot::isInput() const
{
    return _slot->getDirection() == model::Slot::Direction::INPUT;
}

QRectF view::Slot::boundingRect() const
{
    return QGraphicsSvgItem::boundingRect();
    const float halfSize = (float)_size * 0.85f;
    auto basePos = getBasePosition();
    return QRectF(basePos.x() - halfSize, basePos.y() - halfSize, _size, _size);
}

#include <QStyleOptionGraphicsItem>

void view::Slot::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    float textSpace = 14;

    auto basePos = getBasePosition();

    auto finalPosition = basePos;

    finalPosition += QPointF(-10, -10);

    auto textRect = _title->boundingRect();

    float textX = isInput() ? (boundingRect().width() + textSpace) : (-(textSpace + textRect.width()));
    _title->setPos(textX, -8);

    setPos(finalPosition);

    if (_mouseHovering) { /*
        auto originalBoundingRect = QGraphicsSvgItem::boundingRect();
        float size = std::max(originalBoundingRect.width(), originalBoundingRect.height());
        QPainterPath path;
        QPen pen(Qt::PenStyle::SolidLine);
        pen.setWidth(5);
        pen.setColor(QColor(255, 255, 0));
        painter->setPen(pen);
        painter->drawEllipse(0, 0, size, size);*/
        setScale(1.1f);
    } else {
        setScale(1.0f);
    }

    QGraphicsSvgItem::paint(painter, option, widget);

    /*
    const float halfSize = _size * 0.5f;
    float outlineSize = _size * 1.1f;
    //float outlineHalfSize = outlineSize * 0.5f;
    QPainterPath path, outline;
    QPen pen(QColor("#4D4B4D"));
    QBrush brush(isInput() ? colors::green : colors::red);
    //QBrush brush(QColor("#4D4B4D"));

    path.setFillRule(Qt::FillRule::WindingFill);
    painter->setPen(pen);
    painter->setBrush(brush);
    path.addEllipse(basePos, halfSize, halfSize);
    outline.addEllipse(basePos, outlineSize * 0.5f, outlineSize * 0.5f);

    painter->drawPath(path.simplified());

    if (_mouseHovering)
    {
        painter->setBrush(Qt::BrushStyle::NoBrush);
        QPen pen(brush.color(), 5.0f);
        painter->setPen(pen);
        painter->drawPath(outline.simplified());
    }

    _icon->paint(painter, option, widget);
    //*/
}

void view::Slot::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    _mouseHovering = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void view::Slot::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    _mouseHovering = true;
    QGraphicsItem::hoverMoveEvent(event);
}

void view::Slot::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    _mouseHovering = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

QPointF view::Slot::getBasePosition(bool global) const
{
    auto node = dynamic_cast<Node*>(parentItem());
    float horizontalSpacing = 10.0f;
    const int index = node->getSlotIndex(this);
    const float x = isInput() ? (0 - horizontalSpacing) : (node->width() + horizontalSpacing);
    const float y = node->getHeaderHeight() * 1.0f + 5 + index * node->getSlotHeight();
    //_title->boundingRect().height()

    auto p = QPointF(x, y);

    return global ? (p + scenePos()) : p;
}

QPointF view::Slot::getCenterPosition() const
{
    return scenePos() + QPointF(10, 10);
}

bool view::Slot::acceptConnection(Slot* origin) const
{
    return getModelSlot()->getNode()->getGraph()->canConnectSlots(origin->getModelSlot(), getModelSlot());
}

model::Slot* view::Slot::getModelSlot() const
{
    return _slot;
}

view::Node* view::Slot::getNode() const
{
    return dynamic_cast<Node*>(parentItem());
}

void view::Slot::setName(const QString& name)
{
    _title->setPlainText(name);
    getModelSlot()->setName(name);
    update();
}

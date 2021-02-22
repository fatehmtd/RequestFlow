#include "Slot.h"
#include <QPainterPath>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QDebug>
#include "Node.h"
#include "Colors.h"
#include <model/Slot.h>

view::Slot::Slot(Node* parent, model::Slot* slot) : QGraphicsObject(parent), _slot(slot)
{
	_size = 10;
	setFlag(GraphicsItemFlag::ItemIsSelectable);
	//setFlag(GraphicsItemFlag::ItemIsMovable);
	//setAcceptDrops(true);
	setAcceptHoverEvents(true);

	_title = new QGraphicsTextItem(this);
	_title->setPlainText(slot->objectName());
	_title->setDefaultTextColor(Qt::black);
}

bool view::Slot::isInput() const
{
	return _slot->getDirection() == model::Slot::Direction::INPUT;
}

QRectF view::Slot::boundingRect() const
{
	const float halfSize = (float)_size * 0.5f;
	auto basePos = getBasePosition();
	return QRectF(basePos.x() - halfSize, basePos.y() - halfSize, _size, _size);
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

	float outlineSize = _size * 1.1f;
	float outlineHalfSize = outlineSize * 0.5f;

	float textX = isInput() ? halfSize : (basePos.x() - (textRect.width() + halfSize));
	_title->setPos(textX, basePos.y() - textRect.height() * 0.5f);

	path.addEllipse(basePos, halfSize, halfSize);
	outline.addEllipse(basePos, outlineSize * 0.5f, outlineSize * 0.5f);

	painter->drawPath(path.simplified());

	if (_mouseHovering)
	{
		painter->setBrush(Qt::BrushStyle::NoBrush);
		QPen pen(QColor("#FFAAAA"), 3.0f);
		painter->setPen(pen);
		painter->drawPath(outline.simplified());
	}
}

void view::Slot::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHovering = true;
	QGraphicsItem::hoverEnterEvent(event);
}

void view::Slot::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
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

	const int index = node->getSlotIndex(this);
	const float x = isInput() ? 0 : node->boundingRect().width();
	const float y = 35 + index * _title->boundingRect().height();

	auto p = QPointF(x, y);

	return global ? (p + scenePos()) : p;
}

bool view::Slot::acceptConnection(Slot* origin) const
{
	if (isInput() && !origin->isInput())
	{
		return getModelSlot()->getDataType() == origin->getModelSlot()->getDataType();
	}

	return false;
}

model::Slot* view::Slot::getModelSlot() const
{
	return _slot;
}

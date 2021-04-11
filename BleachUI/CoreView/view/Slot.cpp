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
	_size = 20;
	setFlag(GraphicsItemFlag::ItemIsSelectable);
	setAcceptDrops(true);
	
	setAcceptHoverEvents(true);
	QFont font;
	font.setPointSize(12);
	//font.setBold(true);
	_title = new QGraphicsTextItem(this);
	_title->setFont(font);
	_title->setPlainText(slot->objectName());
	_title->setDefaultTextColor(Qt::white);
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
	const float halfSize = (float)_size * 0.5f;
	auto basePos = getBasePosition();
	return QRectF(basePos.x() - halfSize, basePos.y() - halfSize, _size, _size);
}

#include <QStyleOptionGraphicsItem>

void view::Slot::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPainterPath path, outline;
	QPen pen(QColor("#4D4B4D"));
	QBrush brush(isInput() ? colors::green : colors::red);
	//QBrush brush(QColor("#4D4B4D"));

	path.setFillRule(Qt::FillRule::WindingFill);
	painter->setPen(pen);
	painter->setBrush(brush);

	const float halfSize = _size * 0.5f;
	float textSpace = 18;

	auto basePos = getBasePosition();

	auto textRect = _title->boundingRect();

	float outlineSize = _size * 1.1f;
	float outlineHalfSize = outlineSize * 0.5f;

	float textX = isInput() ? textSpace : (basePos.x() - (textRect.width() + textSpace));
	_title->setPos(textX, basePos.y() - textRect.height() * 0.5f);

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
	float horizontalSpacing = 5.0f;
	const int index = node->getSlotIndex(this);
	const float x = isInput() ? (0 - horizontalSpacing) : (node->boundingRect().width() + horizontalSpacing);
	const float y = node->getHeaderHeight() * 0.8f + index * node->getSlotHeight();
	//_title->boundingRect().height()

	auto p = QPointF(x, y);

	return global ? (p + scenePos()) : p;
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

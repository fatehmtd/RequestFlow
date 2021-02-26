#include "Edge.h"
#include "Slot.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>
#include <QDebug>

view::Edge::Edge(Slot* origin, Slot* destination) : _slotDestination(destination), _slotOrigin(origin)
{
	setZValue(-1);
	setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
	setAcceptHoverEvents(true);
	_thickness = 5.0f;
}

view::Edge::~Edge()
{

}

QPainterPath view::Edge::shape() const
{
	QPainterPathStroker stroker;
	stroker.setWidth(_thickness * 2.0f);
	return stroker.createStroke(buildPath()).simplified();
}

void view::Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{	
	painter->beginNativePainting();
	auto path = buildPath();
	setPath(path);
	QPen pen(isSelected() ? QColor("#22AAAA") : QColor("#222222"), _thickness, _mouseHovering ? Qt::PenStyle::DotLine : Qt::PenStyle::SolidLine);
	painter->setPen(pen);
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath(path);
	painter->endNativePainting();
}

QPainterPath view::Edge::buildPath() const
{
	auto inPosition = _slotDestination->getBasePosition(true);
	auto outPosition = _slotOrigin->getBasePosition(true);
	QPointF ctrlPointA, ctrlPointB;

	float dx = (outPosition.x() - inPosition.x());

	float cltrX = dx < 20.0f ? 250 : 50;

	ctrlPointA = outPosition + QPointF(-cltrX, 0);
	ctrlPointB = inPosition + QPointF(cltrX, 0);

	QPainterPath path(outPosition);

	path.cubicTo(ctrlPointA, ctrlPointB, inPosition);

	return path;
}

QRectF view::Edge::boundingRect() const
{
	return buildPath().boundingRect();
}

void view::Edge::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHovering = true;
	QGraphicsItem::hoverEnterEvent(event);
}

void view::Edge::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	QGraphicsItem::hoverMoveEvent(event);
}

void view::Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHovering = false;
	QGraphicsItem::hoverLeaveEvent(event);
}
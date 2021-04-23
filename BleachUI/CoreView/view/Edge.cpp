#include "Edge.h"
#include "Slot.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>
#include <QDebug>
#include "Colors.h"

view::Edge::Edge(SceneGraph* graph, model::Edge* edge) : _edge(edge)
{
	connect(graph->getModelGraph(), &model::Graph::started, this, &Edge::onGraphStarted, Qt::ConnectionType::DirectConnection);
	//connect(edge->getOriginSlot(), &model::OutputSlot::dataSent, this, &Edge::onDataReceived);
	//connect(edge->getDestinationSlot(), &model::InputSlot::dataReceived, this, &Edge::onDataReceived);
	connect(edge, &model::Edge::dataReceived, this, &Edge::onDataReceived, Qt::ConnectionType::QueuedConnection);
	connect(edge, &model::Edge::failed, this, &Edge::onFailed, Qt::ConnectionType::QueuedConnection);

	_slotOrigin = graph->findbyModel(edge->getDestinationSlot());
	_slotDestination = graph->findbyModel(edge->getOriginSlot());

	setZValue(-1);
	setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable);
	setAcceptHoverEvents(true);
	_thickness = 7.0f;

	_idleColor = QColor("#4D4B4D");
	_successColor = colors::blue;
	_failureColor = colors::red;
	_hoverColor = colors::orange;
}

view::Edge::~Edge()
{
}

QPainterPath view::Edge::shape() const
{
	QPainterPathStroker stroker;
	stroker.setWidth(_thickness);
	return stroker.createStroke(buildPath()).simplified();
}

#include <QStyleOptionGraphicsItem>

void view::Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//painter->setClipPath(shape()); 
	//painter->setClipRect(option->exposedRect);

	auto inPosition = _slotDestination->getBasePosition(true);
	auto outPosition = _slotOrigin->getBasePosition(true);

	float dx = (outPosition.x() - inPosition.x());
	//setZValue(dx < 20 ? 1 : -1);

	auto path = buildPath();
	setPath(path);
	//QPen pen(isSelected() ? colors::orange : (_mouseHovering ? colors::orange : QColor("#4D4B4D")), _thickness, _mouseHovering ? Qt::PenStyle::DotLine : Qt::PenStyle::SolidLine);
	auto currentColor = (isSelected() || _mouseHovering) ? _hoverColor : evalColor();
	QPen pen(currentColor, _thickness, _mouseHovering ? Qt::PenStyle::DotLine : Qt::PenStyle::SolidLine);
	
	painter->setPen(pen);
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath(path);
}

QPainterPath view::Edge::buildPath() const
{
	auto inPosition = _slotDestination->getBasePosition(true);
	auto outPosition = _slotOrigin->getBasePosition(true);

	float dx = (outPosition.x() - inPosition.x());

	float thresh = abs(dx) * 0.5f;

	float cltrX = std::min(300.0f, std::max(thresh, 100.0f));
	 
	auto ctrlPointA = outPosition + QPointF(-cltrX, 0);
	auto ctrlPointB = inPosition + QPointF(cltrX, 0);

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

QColor view::Edge::evalColor() const
{
	switch (_transferStatus)
	{
	case -1: // faillure
		return _failureColor;
	case 0:
		return _idleColor;
	case 1:
		return _successColor;
	}
	return QColor();
}

void view::Edge::onGraphStarted()
{
	_transferStatus = 0;
	update();
}

void view::Edge::onDataReceived()
{
	_transferStatus = 1;
	update();
}

void view::Edge::onFailed()
{
	_transferStatus = -1;
	update();
}

view::Slot* view::Edge::getOriginSlot() const
{
	return _slotOrigin;
}

view::Slot* view::Edge::getDestinationSlot() const
{
	return _slotDestination;
}

#include "Node.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "Colors.h"
#include "Slot.h"

view::Node::Node()
{
	setupUi();
}

view::Node::~Node()
{
}

int view::Node::width() const
{
	return _width;
}

int view::Node::height() const
{
	return _height;
}

int view::Node::getSlotIndex(const Slot* slot) const
{
	const auto allChildren = childItems();
	int index = 0;
	for (auto child : allChildren)
	{
		auto childSlot = dynamic_cast<Slot*>(child);
		if (childSlot == nullptr) continue;
		if (childSlot->isInput() == slot->isInput())
		{
			if (childSlot == slot) break;
			++index;
		}
	}
	return index;
}

void view::Node::setupUi()
{
	_width = 256;
	_height = 128;
	_edgeSize = 5;

	setAcceptHoverEvents(true);
	
	setFlag(GraphicsItemFlag::ItemIsSelectable);
	setFlag(GraphicsItemFlag::ItemIsMovable);

	_title = new QGraphicsTextItem(this);
	_title->setPlainText(QString("Entry point").arg(rand()%100));
	_title->setDefaultTextColor(Qt::white);
}

QRectF view::Node::boundingRect() const
{
	return QRectF(0, 0, _width, _height).normalized();
}

void view::Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	// Background
	{
		QPainterPath backgroundPath;
		auto bgColor = colors::light::green;
		QPen pen(bgColor);

		QBrush backgroundBrush(bgColor);
		backgroundPath.setFillRule(Qt::FillRule::WindingFill);
		backgroundPath.addRoundedRect(0, 0, _width, _height, _edgeSize, _edgeSize);
		painter->setPen(pen);
		painter->setBrush(backgroundBrush);
		painter->drawPath(backgroundPath.simplified());
	}

	// Title background
	{
		QPainterPath backgroundPath;
		auto bgColor = colors::dark::green;
		QPen pen(bgColor);

		QBrush backgroundBrush(bgColor);
		backgroundPath.setFillRule(Qt::FillRule::WindingFill);
		backgroundPath.addRoundedRect(0, 0, _width, 20, _edgeSize, _edgeSize);
		painter->setPen(pen);
		painter->setBrush(backgroundBrush);
		painter->drawPath(backgroundPath.simplified());
	}

	// Outline
	{
		QPainterPath outlinePath;
		outlinePath.addRoundedRect(0, 0, _width, _height, _edgeSize, _edgeSize);
		QPen pen(isSelected() ? QColor("#FFFFA637") : QColor("#7F000000"), isSelected() ? 2.0f : 1.0f);
		painter->setPen(pen);
		painter->setBrush(Qt::BrushStyle::NoBrush);
		painter->drawPath(outlinePath.simplified());
	}

	// Outline Hover
	if(_mouseHover)
	{
		QPainterPath outlinePath;
		float outlineEdgeSize = _edgeSize * 1.5f;
		float halfEdgeSize = 0.5f * outlineEdgeSize;
		outlinePath.addRoundedRect(-halfEdgeSize, -halfEdgeSize, _width + outlineEdgeSize, _height + outlineEdgeSize, outlineEdgeSize, outlineEdgeSize);
		QPen pen (QColor("#FFFFA637"), _edgeSize, Qt::PenStyle::DashDotDotLine);
		painter->setPen(pen);
		painter->setBrush(Qt::BrushStyle::NoBrush);
		painter->drawPath(outlinePath.simplified());
	}
}

void view::Node::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//QGraphicsItem::mouseMoveEvent(event);
	auto pos = event->pos() + _mouseShift;	

	setPos(mapToScene(pos));
}

void view::Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mousePressEvent(event);
	_mouseShift = pos() - mapToScene(event->pos());
}

void view::Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

void view::Node::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHover = true;
	QGraphicsItem::hoverEnterEvent(event);
}

void view::Node::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	QGraphicsItem::hoverMoveEvent(event);
}

void view::Node::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHover = false;
	QGraphicsItem::hoverLeaveEvent(event);
}

#include "Edge.h"
#include "Slot.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>

view::Edge::Edge(Slot* slotOut, Slot* slotIn) : _slotIn(slotIn), _slotOut(slotOut)
{
	setZValue(-1);
}

view::Edge::~Edge()
{

}

void view::Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto inPosition = _slotIn->getBasePosition(true);
	auto outPosition = _slotOut->getBasePosition(true);
	QPointF ctrlPointA, ctrlPointB;

	ctrlPointA = outPosition + QPointF(-50, 0);
	ctrlPointB = inPosition + QPointF(50, 0);

	QPainterPath path(outPosition);
	path.cubicTo(ctrlPointA, ctrlPointB, inPosition);

	QPen pen(QColor("#555555"), 5);
	painter->setPen(pen);
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath(path);
}

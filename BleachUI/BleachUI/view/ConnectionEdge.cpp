#include "ConnectionEdge.h"
#include "Slot.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>

view::ConnectionEdge::ConnectionEdge()
{
	_thickness = 10.0f;
	_eligibility = NO_CANDIDATE;
	setZValue(1);
}

void view::ConnectionEdge::setOrigin(Slot* slot)
{
	_slotOrigin = slot;
}

void view::ConnectionEdge::setDestination(const QPointF& position)
{
	_destinationPos = position;
}

void view::ConnectionEdge::setEligible()
{
	_eligibility = Eligibility::ELIGIBLE;
}

void view::ConnectionEdge::setNotEligible()
{
	_eligibility = Eligibility::NOT_ELIGIBLE;
}

void view::ConnectionEdge::setNoCandidateAvaible()
{
	_eligibility = Eligibility::NO_CANDIDATE;
}

void view::ConnectionEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (_slotOrigin != nullptr)
	{
		auto originPosition = _destinationPos;
		auto destinationPosition = _slotOrigin->getBasePosition(true);

		QColor noCandidateColor("#FFFFA637");
		QColor notEligibleColor("#FF0000");
		QColor eligibleColor("#00FF00");

		QColor color = noCandidateColor;
		Qt::PenStyle style = Qt::PenStyle::DashDotDotLine;
		float thickness = _thickness;
		switch (_eligibility)
		{
		case Eligibility::NO_CANDIDATE:
		{
			color = noCandidateColor;
			style = Qt::PenStyle::DashDotDotLine;
			thickness = _thickness;
		}
			break;
		case Eligibility::NOT_ELIGIBLE:
		{
			color = notEligibleColor;
			style = Qt::PenStyle::DotLine;
			thickness = _thickness*0.5f;
		}
			break;
		case Eligibility::ELIGIBLE:
		{
			color = eligibleColor;
			style = Qt::PenStyle::SolidLine;
			thickness = _thickness*0.75f;
		}
			break;
		}

		QPen pen(color);
		QBrush brush(color);

		// origin point
		{
			QPainterPath path;
			float halfSize = thickness * 1.1f;
			path.addEllipse(originPosition, halfSize, halfSize);
			painter->setPen(pen);
			painter->setBrush(brush);
			painter->drawPath(path.simplified());
		}

		// destination point
		{
			QPainterPath path;
			float halfSize = thickness * 1.1f;
			path.addEllipse(destinationPosition, halfSize, halfSize);

			painter->setPen(pen);
			painter->setBrush(brush);
			painter->drawPath(path.simplified());
		}

		QPointF ctrlPointA, ctrlPointB;

		float dx = (originPosition.x() - destinationPosition.x());

		float cltrX = dx < 20.0f ? 250 : 50;

		ctrlPointA = originPosition + QPointF(-cltrX, 0);
		ctrlPointB = destinationPosition + QPointF(cltrX, 0);

		{
			QPainterPath path(originPosition);
			path.cubicTo(ctrlPointA, ctrlPointB, destinationPosition);

			QPen pen(color, thickness, style);
			painter->setPen(pen);
			painter->setBrush(Qt::BrushStyle::NoBrush);
			painter->drawPath(path);
		}
	}
}

QPainterPath view::ConnectionEdge::buildPath() const
{
	if (_slotOrigin != nullptr)
	{
		auto destinationPosition = _slotOrigin->getBasePosition(true);
		auto originPosition = _destinationPos;

		float dx = (originPosition.x() - destinationPosition.x());

		float cltrX = dx < 20.0f ? 250 : 50;

		auto ctrlPointA = originPosition + QPointF(-cltrX, 0);
		auto ctrlPointB = destinationPosition + QPointF(cltrX, 0);

		QPainterPath path(originPosition);
		path.cubicTo(ctrlPointA, ctrlPointB, destinationPosition);

		return path;
	}
	else
		return QPainterPath();
}

QRectF view::ConnectionEdge::boundingRect() const
{
	return buildPath().simplified().boundingRect();
}

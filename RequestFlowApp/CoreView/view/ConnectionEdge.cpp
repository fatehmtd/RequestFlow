#include "ConnectionEdge.h"
#include "Slot.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>
#include <QDebug>

QColor view::ConnectionEdge::_noCandidateColor = QColor("#FFFFA637");
QColor view::ConnectionEdge::_notEligibleColor = QColor("#FF0000");
QColor view::ConnectionEdge::_eligibleColor = QColor("#00FF00");

view::ConnectionEdge::ConnectionEdge()
{
	_thickness = 7.0f;
	_eligibility = NO_CANDIDATE;
	setZValue(1);
	//setAttribute(Qt::WA_OpaquePaintEvent, true);
}

void view::ConnectionEdge::setOrigin(Slot* slot)
{
	_slotOrigin = slot;
	if (slot != nullptr)
        //setOriginP(slot->getBasePosition(true));
        setOriginP(slot->getCenterPosition());
}

void view::ConnectionEdge::setOriginP(const QPointF& position)
{
	_originPos = position;
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
	painter->setClipRect(boundingRect());
	painter->setClipping(false);

	if (_slotOrigin != nullptr)
	{
		auto destinationPosition = _destinationPos;
        //auto originPosition = _slotOrigin->getBasePosition(true);
        auto originPosition = _slotOrigin->getCenterPosition();

		QColor color = _noCandidateColor;
		Qt::PenStyle style = Qt::PenStyle::DashDotDotLine;
		float thickness = _thickness;
		switch (_eligibility)
		{
		case Eligibility::NO_CANDIDATE:
		{
			color = _noCandidateColor;
			style = Qt::PenStyle::DashDotDotLine;
			thickness = _thickness;
		}
		break;
		case Eligibility::NOT_ELIGIBLE:
		{
			color = _notEligibleColor;
			style = Qt::PenStyle::DotLine;
			thickness = _thickness * 0.75f;
		}
		break;
		case Eligibility::ELIGIBLE:
		{
			color = _eligibleColor;
			style = Qt::PenStyle::SolidLine;
			thickness = _thickness * 1.25f;
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
			painter->drawPath(path);
		}

		// destination point
		{
			QPainterPath path;
			float halfSize = thickness * 1.1f;
			path.addEllipse(destinationPosition, halfSize, halfSize);

			painter->setPen(pen);
			painter->setBrush(brush);
			painter->drawPath(path);
		}
		
		{
			auto path = buildPath();

			QPen pen(color, thickness, style);
			painter->setPen(pen);
			painter->setBrush(Qt::BrushStyle::NoBrush);
			painter->drawPath(path);
		}
	}
}

#include <math.h>

QPainterPath view::ConnectionEdge::buildPath() const
{
	if (_slotOrigin != nullptr)
	{
        auto destinationPosition = _slotOrigin->getCenterPosition();
		auto originPosition = _destinationPos;

		float dx = (originPosition.x() - destinationPosition.x());		
        float thresh = abs(dx) * 0.5f;
		float cltrX = std::min(300.0f, std::max(thresh, 100.0f));

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
	//TODO: fix this workaround to render using the proper boundingRect
	float size = 1 << 18;
	float hsize = size * 0.5f;
	return QRectF(-hsize, -hsize, size, size);
}

QPainterPath view::ConnectionEdge::shape() const
{
	return buildPath().simplified();
}

#include "ConnectionEdge.h"
#include "Slot.h"
#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

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

void view::ConnectionEdge::setHeadSlot(Slot* slot)
{
    _headSlot = slot;
    _creationDirection = _headSlot->isInput() ? CreationDirection::DESTINATION_TO_ORIGIN : CreationDirection::ORIGIN_TO_DESTINATION;
}

void view::ConnectionEdge::setTailSlot(Slot* slot)
{
    _tailSlot = slot;
}

void view::ConnectionEdge::setTailPosition(const QPointF& p)
{
    _tailPosition = p;
}

view::Slot* view::ConnectionEdge::getHeadSlot() const
{
    return _headSlot;
}

view::Slot* view::ConnectionEdge::getTailSlot() const
{
    return _tailSlot;
}

QPointF view::ConnectionEdge::getTailPosition() const
{
    return _tailPosition;
}

void view::ConnectionEdge::reset()
{
    _headSlot = nullptr;
    _creationDirection = CreationDirection::UNKNOWN;
}

void view::ConnectionEdge::resetTailSlot()
{
    _tailSlot = nullptr;
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

view::ConnectionEdge::CreationDirection view::ConnectionEdge::getDirection() const
{
    return _creationDirection;
}

void view::ConnectionEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setClipRect(boundingRect());
    painter->setClipping(false);

    if (_headSlot != nullptr) {
        QPointF destinationPosition, originPosition;
        switch (_creationDirection) {
        case view::ConnectionEdge::UNKNOWN:
            return;
        case CreationDirection::DESTINATION_TO_ORIGIN: {
            destinationPosition = _headSlot->getCenterPosition();
            originPosition = _tailPosition;
        } break;
        case CreationDirection::ORIGIN_TO_DESTINATION: {
            originPosition = _headSlot->getCenterPosition();
            destinationPosition = _tailPosition;
        } break;
        }

        QColor color = _noCandidateColor;
        Qt::PenStyle style = Qt::PenStyle::DashDotDotLine;
        float thickness = _thickness;
        switch (_eligibility) {
        case Eligibility::NO_CANDIDATE: {
            color = _noCandidateColor;
            style = Qt::PenStyle::DashDotDotLine;
            thickness = _thickness;
        } break;
        case Eligibility::NOT_ELIGIBLE: {
            color = _notEligibleColor;
            style = Qt::PenStyle::DotLine;
            thickness = _thickness * 0.75f;
        } break;
        case Eligibility::ELIGIBLE: {
            color = _eligibleColor;
            style = Qt::PenStyle::SolidLine;
            thickness = _thickness * 1.25f;
        } break;
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
    if (_headSlot != nullptr) {
        auto destinationPosition = getDestinationPosition();
        auto originPosition = getOriginPosition();

        float thresh = abs(originPosition.x() - destinationPosition.x()) * 0.5f;
        float cltrX = std::min(300.0f, std::max(thresh, 100.0f));

        auto ctrlPointA = originPosition + QPointF(cltrX, 0);
        auto ctrlPointB = destinationPosition + QPointF(-cltrX, 0);

        QPainterPath path(originPosition);
        path.cubicTo(ctrlPointA, ctrlPointB, destinationPosition);

        return path;
    }

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

QPointF view::ConnectionEdge::getDestinationPosition() const
{
    switch (_creationDirection) {
    case view::ConnectionEdge::UNKNOWN:
        return QPointF();
    case CreationDirection::DESTINATION_TO_ORIGIN:
        return _headSlot->getCenterPosition();
    case CreationDirection::ORIGIN_TO_DESTINATION:
        return _tailPosition;
    default:
        return QPointF();
    }
    return QPointF();
}

QPointF view::ConnectionEdge::getOriginPosition() const
{
    switch (_creationDirection) {
    case view::ConnectionEdge::UNKNOWN:
        return QPointF();
    case CreationDirection::DESTINATION_TO_ORIGIN:
        return _tailPosition;
    case CreationDirection::ORIGIN_TO_DESTINATION:
        return _headSlot->getCenterPosition();
    default:
        return QPointF();
    }
    return QPointF();
}

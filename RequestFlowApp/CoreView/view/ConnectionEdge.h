#pragma once
#include "coreview_global.h"

#include <QColor>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <model/Slot.h>

namespace view {
class Slot;

class COREVIEW_EXPORT ConnectionEdge : public QGraphicsPathItem {
public:
    ConnectionEdge();

    void setHeadSlot(Slot* slot);
    void setTailSlot(Slot* slot);

    void setTailPosition(const QPointF& p);

    Slot* getHeadSlot() const;
    Slot* getTailSlot() const;
    QPointF getTailPosition() const;

    void reset();
    void resetTailSlot();

    enum Eligibility {
        ELIGIBLE,
        NOT_ELIGIBLE,
        NO_CANDIDATE
    };

    enum CreationDirection {
        UNKNOWN = -1,
        ORIGIN_TO_DESTINATION,
        DESTINATION_TO_ORIGIN
    };

    void setEligible();
    void setNotEligible();
    void setNoCandidateAvaible();

    CreationDirection getDirection() const;

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    QPainterPath buildPath() const;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

    QPointF getDestinationPosition() const;
    QPointF getOriginPosition() const;

protected:
    Slot* _headSlot = nullptr;
    Slot* _tailSlot = nullptr;

    QPointF _tailPosition;

    int _thickness = 10;
    Eligibility _eligibility;

    CreationDirection _creationDirection;

    static QColor _noCandidateColor;
    static QColor _notEligibleColor;
    static QColor _eligibleColor;
};
}

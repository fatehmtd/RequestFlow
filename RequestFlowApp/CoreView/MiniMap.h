#pragma once

#include <QGraphicsObject>

namespace view
{
    class MiniMap : public QGraphicsObject
    {
    public:
        MiniMap();

        enum Anchor
        {
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_RIGHT,
            BOTTOM_LEFT
        };

        QPointF coords() const;
        void setCoords(QPointF newCoords);

        const QSize &size() const;
        void setSize(const QSize &newSize);

        Anchor anchor() const;
        void setAnchor(Anchor newAnchor);

    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;

    private:
        QPointF _coords;
        QSize _size;
        Anchor _anchor;
    };
}


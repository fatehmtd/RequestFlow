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

        QPointF getCoords() const;
        void setCoords(QPointF newCoords);

        const QSize &getSize() const;
        void setSize(const QSize &newSize);

        Anchor getAnchor() const;
        void setAnchor(Anchor newAnchor);

        void setGap(float g);
        float getGap() const;

        void setParentViewport(QRectF rect);

    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
        QRectF boundingRect() const override;

        void processAnchoring();

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        QPointF _coords;
        QSize _size;
        float _gap = 0.0f;
        float _invScale = 1.0f;
        Anchor _anchor;
        QRectF _parentViewport;

        bool _mousePressed = false;
    };
}


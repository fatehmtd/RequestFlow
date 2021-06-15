#include "MiniMap.h"

view::MiniMap::MiniMap()
{
    _anchor = Anchor::TOP_RIGHT;
    _coords = QPointF(0, 0);
    _size = QSize(192, 192);
    setZValue(100);
    setGap(0);
    setFlag(GraphicsItemFlag::ItemIsSelectable);
}

#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QPainter>

#include <QDebug>
#include "view/Colors.h"
#include "view/SceneGraphWidget.h"
#include <QDebug>
#include "view/Node.h"

void view::MiniMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto rect = boundingRect();
    // Nodes
    {
        auto sceneRect = scene()->sceneRect();

        QList<Node*> nodes;
        for(auto item : scene()->items())
        {
            auto node = dynamic_cast<view::Node*>(item);
            if(node != nullptr)
            {
                nodes << node;
            }
        }

        for(auto node : nodes)
        {
            auto p = node->scenePos();
            float w = rect.width()*(node->width() / sceneRect.width());
            float h = rect.height()*(node->height() / sceneRect.height());

            float x = rect.width()*((p.x() - sceneRect.x()) / sceneRect.width());
            float y = rect.height()*((p.y() - sceneRect.y()) / sceneRect.height());

            QPainterPath backgroundPath;
            auto bgColor = node->getBackgroundColor();
            QPen pen(bgColor);

            QBrush backgroundBrush(bgColor);
            backgroundPath.addRoundedRect(x, y, w, h, 0, 0);
            painter->setPen(pen);
            painter->setBrush(backgroundBrush);
            painter->drawPath(backgroundPath);
        }
    }

    // Outline
    {
        QPainterPath backgroundPath;
        auto bgColor = colors::darkGrey;
        QPen pen(bgColor);

        QBrush backgroundBrush(bgColor);
        backgroundPath.addRoundedRect(0, 0, rect.width(), rect.height(), 0, 0);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(backgroundPath);
    }

    // Viewport area
    {
        auto sceneRect = scene()->sceneRect();

        float w = rect.width()*(_parentViewport.width() / sceneRect.width());
        float h = rect.height()*(_parentViewport.height() / sceneRect.height());

        float x = rect.width()*((_parentViewport.x() - sceneRect.x()) / sceneRect.width());
        float y = rect.height()*((_parentViewport.y() - sceneRect.y()) / sceneRect.height());

        QPainterPath backgroundPath;
        auto bgColor = colors::light::green;
        QPen pen(bgColor);

        QBrush backgroundBrush(bgColor);
        backgroundPath.addRoundedRect(x, y, w, h, 0, 0);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(backgroundPath);
    }
}

QRectF view::MiniMap::boundingRect() const
{
    //return QGraphicsObject::boundingRect();
    return QRectF(0, 0, _size.width()*_invScale, _size.height()*_invScale);
}

void view::MiniMap::processAnchoring()
{
    auto tr = scene()->views().first()->transform();
    float scaleFactor = 1.0f/tr.m11();
    _invScale = scaleFactor;

    float scaledW = getSize().width() * _invScale;
    float scaledH = getSize().height() * _invScale;

    switch(getAnchor())
    {
    case view::MiniMap::TOP_LEFT:
    {
        setPos(_parentViewport.topLeft() + QPointF(_gap, _gap));
        break;
    }
    case view::MiniMap::TOP_RIGHT:
    {
        setPos(_parentViewport.topRight()
               - QPointF(scaledW, 0) + QPointF(-_gap, _gap));
        break;
    }
    case view::MiniMap::BOTTOM_RIGHT:
    {
        setPos(_parentViewport.bottomRight()
               - QPointF(scaledW, scaledH) + QPointF(-_gap, -_gap));
        break;
    }
    case view::MiniMap::BOTTOM_LEFT:
    {
        setPos(_parentViewport.bottomLeft()
               - QPointF(0, scaledH) + QPointF(_gap, -_gap));
        break;
    }
    }
    update();
}

void view::MiniMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        _mousePressed = true;

        auto rect = boundingRect();
        auto p = event->pos();

        float partialX = p.x() / rect.width();
        float partialY = p.y() / rect.height();

        if(partialX < 0.0f || partialX > 1.0f) return;
        if(partialY < 0.0f || partialY > 1.0f) return;

        auto srect = scene()->sceneRect();
        float x = srect.x() + partialX*srect.width();
        float y = srect.y() + partialY*srect.height();

        auto sgw = dynamic_cast<SceneGraphWidget*>(scene()->views().first());
        sgw->setCenterAnimated(QPointF(x, y), false);
        //sgw->centerOn(QPointF(x, y));
        //sgw->getSceneGraph()->customUpdate();
    }
}

#include "view/SceneGraphWidget.h"

void view::MiniMap::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_mousePressed)
    {

    }
}

void view::MiniMap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        _mousePressed = false;


    }
}

int view::MiniMap::getAnchor() const
{
    return _anchor;
}

void view::MiniMap::setAnchor(int newAnchor)
{
    _anchor = newAnchor;
}

void view::MiniMap::setGap(float g)
{
    _gap = g;
}

float view::MiniMap::getGap() const
{
    return _gap;
}

void view::MiniMap::setParentViewport(QRectF rect)
{
    _parentViewport = rect;
    processAnchoring();
}

const QSize &view::MiniMap::getSize() const
{
    return _size;
}

void view::MiniMap::setSize(const QSize &newSize)
{
    _size = newSize;
}

QPointF view::MiniMap::getCoords() const
{
    return _coords;
}

void view::MiniMap::setCoords(QPointF newCoords)
{
    _coords = newCoords;
    processAnchoring();
}

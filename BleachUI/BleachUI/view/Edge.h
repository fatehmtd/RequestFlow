#pragma once
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <model/Slot.h>

namespace view
{
	class Slot;

	class Edge : public QGraphicsPathItem
	{
	public:
		Edge(Slot* origin, Slot* destination);
		~Edge();

		virtual QPainterPath shape() const override;
	protected:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		QPainterPath buildPath() const;	
		virtual QRectF boundingRect() const override;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

	protected:
		Slot* _slotOrigin = nullptr, * _slotDestination = nullptr;
		float _thickness = 10;
		bool _mouseHovering = false;
	};
}
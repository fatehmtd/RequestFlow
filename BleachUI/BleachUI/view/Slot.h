#pragma once
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <model/Slot.h>

namespace view
{
	class Node;

	class Slot : public QGraphicsObject
	{
		Q_OBJECT
	public:
		Slot(Node* parent, model::Slot* slot);
		~Slot();

		bool isInput() const;

		QPointF getBasePosition(bool global = false) const;

		bool acceptConnection(Slot* origin) const;

		model::Slot* getModelSlot() const;
	protected:
		virtual QRectF boundingRect() const override;
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	protected:
		QGraphicsTextItem* _title = nullptr;
		model::Slot* _slot = nullptr;
		bool _isInput = false;
		int _size = 10;
		bool _mouseHovering = false;
	};
}
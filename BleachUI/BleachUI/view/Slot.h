#pragma once
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include "Node.h"
#include <model/Slot.h>

namespace view
{
	class Slot : public QGraphicsItem
	{
	public:
		enum Style
		{
			SQUARE,
			CIRCLE,
			TRIANGLE
		};

		Slot(Node* parent, bool isInput, Style style = Style::CIRCLE);
		~Slot();

		bool isInput() const;

		QPointF getBasePosition(bool global = false) const;

	protected:
		virtual QRectF boundingRect() const override;
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

	protected:
		QGraphicsTextItem* _title = nullptr;
		model::Slot* _slot = nullptr;
		bool _isInput = false;
		Style _style;
		int _size = 10;
		bool _mouseHover = false;
	};
}
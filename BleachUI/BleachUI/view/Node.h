#pragma once
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <model/Node.h>

namespace view
{
	class Slot;

	class Node : public QGraphicsItem
	{
	public:
		Node();
		~Node();

		int width() const;
		int height() const;

		int getSlotIndex(const Slot* slot) const;
	protected:
		void setupUi();

		// Inherited via QGraphicsItem
		virtual QRectF boundingRect() const override;
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

	protected:
		bool _mouseHover = false;
		model::Node* _node = nullptr;
		QGraphicsTextItem* _title = nullptr;
		int _width, _height;
		int _padding;
		int _edgeSize;
		QPointF _mouseShift;
	};
}


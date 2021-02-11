#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <model/Slot.h>

namespace view
{
	class Slot;

	class Edge : public QGraphicsPathItem
	{
	public:

		Edge(Slot* slotOut, Slot* slotIn);
		~Edge();

	protected:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		/*
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
		//*/
	protected:
		Slot* _slotOut = nullptr, * _slotIn = nullptr;
		int _thickness = 10;
		bool _mouseHover = false;
	};
}
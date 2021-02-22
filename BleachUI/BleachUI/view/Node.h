#pragma once
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsObject>
#include <QGraphicsWidget>
#include <QObject>
#include <model/Node.h>
#include "ContentWidget.h"
#include "Colors.h"

namespace view
{
	class Slot;

	class Node : public QGraphicsObject
	{
		Q_OBJECT
	public:
		Node(model::Node* modelNode);
		~Node();

		int width() const;
		int height() const;

		int getSlotIndex(const Slot* slot) const;
		ContentWidget* getContentWidget() const;

		model::Node* getModelNode() const;
		Slot* getSlot(const QString& name) const;

		// Inherited via QGraphicsItem
		virtual QRectF boundingRect() const override;
		
		void setTitle(const QString& txt);
		QString getTitle() const;

		void setSize(int w, int h);

	protected:
		enum Handle
		{
			LEFT = 1,
			RIGHT = 2,
			TOP = 4, 
			BOTTOM = 8,
			TOP_LEFT = 5,
			TOP_RIGHT = 6,
			BOTTOM_LEFT = 9,
			BOTTOM_RIGHT = 10
		};

		virtual void setupUi();
		virtual void setupContentWidget();
		virtual void setupUIForModel();
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event)	 override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)	 override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)	 override;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	private:
		int computeGripCorner(const QPointF& cursorPos);

	protected:
		model::Node* _node = nullptr;
		QGraphicsTextItem* _title = nullptr;
		ContentWidget* _contentWidget = nullptr;
		int _width, _height;
		int _padding;
		int _edgeSize;
		QPointF _mouseShift;
		QColor _bgColor;
		bool _mouseHover = false;

		// resizing
		QPointF _anchorPoint;
		bool _resizeMode = false;
	};
}


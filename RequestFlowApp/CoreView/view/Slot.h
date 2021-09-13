#pragma once

#include "coreview_global.h"

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <model/Slot.h>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

namespace view
{
	class Node;

    class COREVIEW_EXPORT Slot : public QGraphicsSvgItem
	{
		Q_OBJECT
	public:
		Slot(Node* parent, model::Slot* slot);
		~Slot();

		bool isInput() const;

		QPointF getBasePosition(bool global = false) const;

        QPointF getCenterPosition() const;

		bool acceptConnection(Slot* origin) const;

		model::Slot* getModelSlot() const;
		
		Node* getNode() const;
		void setName(const QString& name);

        QRectF boundingRect() const override;
    protected:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

	protected:
		QGraphicsTextItem* _title = nullptr;
		model::Slot* _slot = nullptr;
        QGraphicsSvgItem* _icon = nullptr;

		bool _isInput = false;
		int _size = 10;
		bool _mouseHovering = false;
	};
}

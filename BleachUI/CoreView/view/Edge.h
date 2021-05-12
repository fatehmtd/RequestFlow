#pragma once
#include "coreview_global.h"

#include <QGraphicsPathItem>
#include <QGraphicsTextItem>

#include "SceneGraph.h"

#include <model/Slot.h>
#include <model/Edge.h>
#include <model/Graph.h>

namespace view
{
	class Slot;

	class COREVIEW_EXPORT Edge : public QObject, public QGraphicsPathItem
	{
		Q_OBJECT
	public:
		Edge(SceneGraph* graph, model::Edge* edge);
		~Edge();

		model::Edge* getModelEdge() const { return _edge; }

		Slot* getOriginSlot() const;
		Slot* getDestinationSlot() const;

		virtual QPainterPath shape() const override;
	protected:
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
		QPainterPath buildPath() const;	
		QPainterPath buildPathCubic() const;	
		QPainterPath buildPathSegmented() const;	
		virtual QRectF boundingRect() const override;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

		QColor evalColor() const;

	protected slots:
		void onGraphStarted();
		void onDataReceived();
		void onFailed();

	protected:
		Slot* _slotOrigin = nullptr, * _slotDestination = nullptr;
		model::Edge* _edge = nullptr;
		int  _transferStatus = 0;
		float _thickness = 10;
		bool _mouseHovering = false;
		QColor _idleColor, _successColor, _failureColor, _hoverColor;
	};
}

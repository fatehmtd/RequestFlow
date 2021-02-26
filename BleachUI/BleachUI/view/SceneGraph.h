#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <model/Graph.h>
#include <model/Edge.h>
#include <model/Node.h>
#include <model/Slot.h>

namespace view
{
	class Node;
	class Link;
	class Slot;
	class ConnectionEdge;
	class Edge;

	class SceneGraph : public QGraphicsScene
	{
	public:
		SceneGraph(QObject* parent = nullptr);

		Slot* findbyModel(model::Slot* slot) const;
		Node* findbyModel(model::Node* node) const;
		Edge* findbyModel(model::Edge* edge) const;

	protected:
		virtual void drawBackground(QPainter* painter, const QRectF& rect) override;

		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

		virtual void setupUi();

		void createSampleScenario();

		Node* createEndpointNode();
		Node* createPayloadNode();
		Node* createViewerNode();

	protected:
		model::Graph* _graph = nullptr;
		QColor _background;
		QColor _lightGrid, _darkGrid;
		ushort _cellSize, _blockSize;
		float _zoomLevel;
		Slot* _originSlot = nullptr, * _destinationSlot = nullptr;
		QPointF _cursorPosition;
		ConnectionEdge* _connectionEdge = nullptr;
	};
}
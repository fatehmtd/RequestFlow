#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <functional>
#include <QMap>

#include <model/Graph.h>
#include <model/Edge.h>
#include <model/Node.h>
#include <model/Slot.h>

namespace view
{
	class InteractionsHandler;
	class Node;
	class Link;
	class Slot;
	class ConnectionEdge;
	class Edge;

	class SceneGraph : public QGraphicsScene
	{
	public:
		SceneGraph(model::Graph* modelGraph, QObject* parent = nullptr);

		Slot* findbyModel(model::Slot* slot) const;
		Node* findbyModel(model::Node* node) const;
		Edge* findbyModel(model::Edge* edge) const;

		QList<Node*> getNodes() const;
		QList<Edge*> getEdges() const;

		Node* getNodeAt(const QPointF& p) const;

		model::Graph* getModelGraph() const;

		void registerEdgeAction(QString name, std::function<void(view::Edge*)> func);
		void registerNodeAction(QString name, std::function<void(view::Node*)> func);

		void clearNodes();
		void clearScene();

		void persist(const QString& fileName) const;
		bool load(const QString& fileName);

		void bringToFront(Node* node) const;

	protected:
		virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
		void drawPointsBackground(QPainter* painter, const QRectF& rect);
		void drawGridBackground(QPainter* painter, const QRectF& rect);

		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

		void bringToFront(QPointF pos) const;

		virtual void setupUi();

		void createEdge();

	protected:
		model::Graph* _modelGraph = nullptr;
		QColor _background;
		QColor _lightGrid, _darkGrid;
		ushort _cellSize, _blockSize;
		float _zoomLevel;
		Slot* _originSlot = nullptr, * _destinationSlot = nullptr;
		QPointF _cursorPosition;
		ConnectionEdge* _connectionEdge = nullptr;

		InteractionsHandler* _interactionsHandler = nullptr;

		QMap<QString, std::function<void(view::Edge*)>> _edgesActions;
		QMap<QString, std::function<void(view::Node*)>> _nodesActions;
	};
}

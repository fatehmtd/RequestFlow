#pragma once
#include "coreview_global.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <functional>
#include <QMap>

#include <model/Graph.h>
#include <model/Edge.h>
#include <model/Node.h>
#include <model/Slot.h>

#include "MiniMap.h"

namespace view
{
	class InteractionsHandler;
	class Node;
	class Link;
	class Slot;
	class ConnectionEdge;
	class Edge;

	class COREVIEW_EXPORT SceneGraph : public QGraphicsScene
	{
        Q_OBJECT
	public:
		SceneGraph(model::Graph* modelGraph, QObject* parent = nullptr);

		Slot* findbyModel(model::Slot* slot) const;
		Node* findbyModel(model::Node* node) const;
		Edge* findbyModel(model::Edge* edge) const;

		void createGeometricNodesForModel();

		QList<Node*> getNodes() const;
		QList<Edge*> getEdges() const;

		Node* getNodeAt(const QPointF& p) const;

		model::Graph* getModelGraph() const;

		void registerEdgeAction(QString name, std::function<void(view::Edge*)> func);
		void registerNodeAction(QString name, std::function<void(view::Node*)> func);

		void clearNodes();
		void clearScene();

		void bringToFront(Node* node) const;

		Node* cloneNode(Node* originalNode);

		InteractionsHandler* getInteractionsHandler() const;

        void deleteSelectedItems() const;
        void duplicateSelectedItems() const;
        void renameSelectedNode() const;

        // Visuals

        enum BackgroundType
        {
            SOLID,
            DOTS,
            CROSSES,
            GRID,
            QUADS
        };

        enum EdgeType
        {
            CURVES,
            LINES
        };

        QRectF computeBoundingRect(const QList<view::Node*> & nodes, qreal padding=0) const;

        void setBackgroundType(BackgroundType bgType);
        int getBackgroundType() const;

        void setEdgeType(EdgeType type);
        int getEdgeType() const;

        MiniMap *getMiniMap() const;

        void addItem(QGraphicsItem *item);
        void customUpdate();


        QImage takeScreenShotSvg(QRectF rect, qreal multiplier = 4.0f);
        QImage takeScreenShot(QRectF rect, qreal multiplier = 4.0f);
        QImage takeScreenShot(qreal padding=20, qreal multiplier=2.0f);

    signals:
        void nodeDoubleClicked(view::Node*);

	protected:        

		Node* createGeometryForModel(model::Node* node);

		virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
        virtual void drawForeground(QPainter* painter, const QRectF& rect) override;

        void drawDotsBackground(QPainter* painter, const QRectF& rect) const;
        void drawGridBackground(QPainter* painter, const QRectF& rect) const;
        void drawCrossBackground(QPainter* painter, const QRectF& rect) const;
        void drawQuadsBackground(QPainter* painter, const QRectF& rect) const;

        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

		void bringToFront(QPointF pos) const;

		virtual void setupUi();

		void createEdge();

		virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
		virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
		virtual void dropEvent(QGraphicsSceneDragDropEvent* event) override;

	protected:
        int _backgroundType = 0;
        int _edgeType = 0;

		model::Graph* _modelGraph = nullptr;
		QColor _background;
		QColor _lightGrid, _darkGrid;
		ushort _cellSize, _blockSize;
		float _zoomLevel;
		Slot* _originSlot = nullptr, * _destinationSlot = nullptr;
		QPointF _cursorPosition;
		ConnectionEdge* _connectionEdge = nullptr;

		InteractionsHandler* _interactionsHandler = nullptr;

        MiniMap *_miniMap = nullptr;

		QMap<QString, std::function<void(view::Edge*)>> _edgesActions;
		QMap<QString, std::function<void(view::Node*)>> _nodesActions;
	};
}

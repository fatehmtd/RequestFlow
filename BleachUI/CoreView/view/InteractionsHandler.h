#pragma once
#include <QObject>
#include <QMenu>
#include <QList>
#include <functional>
#include <QGraphicsItem>

namespace view
{
	class SceneGraph;
	class Node;
	class Slot;
	class Edge;

	class InteractionsHandler : public QObject
	{
	public:
		InteractionsHandler(SceneGraph* scene);
		~InteractionsHandler();

		void registerGenericAction(const QString& name, std::function<bool(QGraphicsItem*)> filter, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerNodeTypeAction(const QString& name, const QString& nodeType, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerNodeAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerEdgeAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerEmptySpaceAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);


		Node* createEndpointNode();
		Node* createPayloadNode();
		Node* createViewerNode();
		Node* createDelayNode();
		Node* createScriptNode();
		Node* createAssertionNode();

		void deleteNode(Node* node);
		void deleteEdge(Edge* edge);
		Node* cloneNode(Node* node);

		QMenu* createContextMenu(const QPointF& p);
	private:
		struct ItemAction
		{
			int order;			
			QString name;
			std::function<void(const QPointF&)> func;
			std::function<bool(QGraphicsItem*)> filter;
			QIcon icon;

			bool operator < (const ItemAction& ia) const
			{
				return this->order >= ia.order;
			}
		};

		void registerCommonActions();

		QList<ItemAction> _itemActionsList;
		SceneGraph* _sceneGraph = nullptr;
	};
}


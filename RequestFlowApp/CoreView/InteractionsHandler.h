#pragma once
#include "coreview_global.h"
#include <QObject>
#include <QMenu>
#include <QList>
#include <functional>
#include <QGraphicsItem>

namespace model
{
	class EndpointEntry;
}

namespace view
{
	class SceneGraph;
	class Node;
	class Slot;
	class Edge;

	class COREVIEW_EXPORT InteractionsHandler : public QObject
	{
	public:
		InteractionsHandler(SceneGraph* scene);
		~InteractionsHandler();

		void registerGenericAction(const QString& name, std::function<bool(QGraphicsItem*)> filter, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerNodeTypeAction(const QString& name, const QString& nodeType, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerNodeAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerEdgeAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);
		void registerEmptySpaceAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon = QIcon(), int order=99);

		Node* createEndpointNode(const model::EndpointEntry* entry = nullptr);
		Node* createPayloadNode();
		Node* createViewerNode();
		Node* createDelayNode();
		Node* createScriptNode();
		Node* createAssertionNode();
        Node* createExternalNode();

		void deleteNode(Node* node);
		void deleteEdge(Edge* edge);
		void deleteInputSlot(Slot* slot);

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
		void renameInputSlots(view::Node* node) const;

		QList<ItemAction> _itemActionsList;
		SceneGraph* _sceneGraph = nullptr;
	};
}


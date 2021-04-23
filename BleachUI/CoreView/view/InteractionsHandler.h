#pragma once
#include <QObject>
#include <QMenu>

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

		Node* createEndpointNode();
		Node* createPayloadNode();
		Node* createViewerNode();
		Node* createDelayNode();
		Node* createScriptNode();
		Node* createAssertionNode();

		Node* createNode(QString nodeType);

		void deleteNode(Node* node);
		void deleteEdge(Edge* edge);
		Node* cloneNode(Node* node);

		QMenu* createContextMenu(const QPointF& p);
	private:
		SceneGraph* _sceneGraph = nullptr;
	};
}


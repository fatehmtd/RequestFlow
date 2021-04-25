#include "InteractionsHandler.h"
#include "SceneGraph.h"
#include "Node.h"
#include "Slot.h"
#include "Colors.h"
#include "Edge.h"
#include "ConnectionEdge.h"

#include "../logic/PayloadNode.h"
#include "../logic/EndpointNode.h"
#include "../logic/ViewerNode.h"
#include "../logic/DelayNode.h"
#include "../logic/ScriptNode.h"
#include "../logic/AssertionNode.h"

view::InteractionsHandler::InteractionsHandler(SceneGraph* scene) : QObject(scene), _sceneGraph(scene)
{
	registerCommonActions();
}

view::InteractionsHandler::~InteractionsHandler()
{
}

void view::InteractionsHandler::registerGenericAction(const QString& name,
	std::function<bool(QGraphicsItem*)> filter,
	std::function<void(const QPointF&)> func,
	QIcon icon, int order)
{
	ItemAction action = { order, name, func, filter, icon };
	_itemActionsList << action;

	qSort(_itemActionsList);
}

void view::InteractionsHandler::registerNodeTypeAction(const QString& name, const QString& nodeType, std::function<void(const QPointF&)> func, QIcon icon, int order)
{
	registerGenericAction(name, [nodeType](QGraphicsItem* item)
		{
			auto node = dynamic_cast<view::Node*>(item);
			if (node == nullptr) return false;
			return nodeType.toLower() == node->getNodeType().toLower();
		}, func, icon, order);
}

void view::InteractionsHandler::registerNodeAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon, int order)
{
	registerGenericAction(name, [](QGraphicsItem* item)
		{
			auto node = dynamic_cast<view::Node*>(item);
			return node != nullptr;
		}, func, icon, order);
}

void view::InteractionsHandler::registerEdgeAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon, int order)
{
	registerGenericAction(name, [](QGraphicsItem* item)
		{
			auto edge = dynamic_cast<view::Edge*>(item);
			return edge != nullptr;
		}, func, icon, order);
}

void view::InteractionsHandler::registerEmptySpaceAction(const QString& name, std::function<void(const QPointF&)> func, QIcon icon, int order)
{
	registerGenericAction(name, [](QGraphicsItem* item)
		{			
			return item == nullptr;
		}, func, icon, order);
}

view::Node* view::InteractionsHandler::createEndpointNode()
{
	auto nodeModel = new model::EndpointNode(_sceneGraph->getModelGraph());
	nodeModel->createModel();

	auto grNodeC = new logic::EndpointNode(nodeModel);
	_sceneGraph->addItem(grNodeC);
	return grNodeC;
}

view::Node* view::InteractionsHandler::createPayloadNode()
{
	auto modelNode = new model::PayloadNode(_sceneGraph->getModelGraph());
	modelNode->createModel();
	auto grNodeA = new logic::PayloadNode(modelNode);
	_sceneGraph->addItem(grNodeA);
	return grNodeA;
}

view::Node* view::InteractionsHandler::createViewerNode()
{
	auto modelNode = new model::ViewerNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto grNodeB = new logic::ViewerNode(modelNode);
	_sceneGraph->addItem(grNodeB);
	return grNodeB;
}

view::Node* view::InteractionsHandler::createDelayNode()
{
	auto modelNode = new model::DelayNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto graphicNode = new logic::DelayNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createScriptNode()
{
	auto modelNode = new model::ScriptNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto graphicNode = new logic::ScriptNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createAssertionNode()
{
	auto modelNode = new model::AssertionNode(_sceneGraph->getModelGraph());
	modelNode->createModel();

	auto graphicNode = new logic::AssertionNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

void view::InteractionsHandler::deleteNode(Node* node)
{
	auto edges = _sceneGraph->getModelGraph()->findEdges(node->getModelNode());
	for (auto edge : edges)
	{
		auto grEdge = _sceneGraph->findbyModel(edge);
		delete grEdge;
		delete edge;
	}

	delete node->getModelNode();
	delete node;
}

void view::InteractionsHandler::deleteEdge(Edge* edge)
{
	delete edge->getModelEdge();
	delete edge;
}

view::Node* view::InteractionsHandler::cloneNode(Node* originalNode)
{/*
	QJSEngine engine;
	auto nodeValue = originalNode->toJSValue(engine);
	auto nodeTypeStr = nodeValue.property("_type").toString();
	auto node = createNode(nodeTypeStr);
	if (node != nullptr)
	{
		node->fromJSValue(nodeValue);
		node->update();
	}
	return node;*/
	return nullptr;
}

#include <QMenu>

QMenu* view::InteractionsHandler::createContextMenu(const QPointF& p)
{
	auto selectedItems = _sceneGraph->selectedItems();

	auto item = _sceneGraph->itemAt(p, QTransform());
	QMenu* menu = new QMenu();

	QList<ItemAction> availableActions;
	for (const auto& action : _itemActionsList)
	{
		if (action.filter(item))
			availableActions << action;
	}

	qSort(availableActions);

	int prevOrder = -1;
	if (availableActions.size() > 0)
		prevOrder = availableActions[0].order;

	for (const auto& action : availableActions)
	{
		if (prevOrder != action.order)
			menu->addSeparator();

		auto qaction = menu->addAction(action.icon, action.name);
		connect(qaction, &QAction::triggered, this, [=]()
			{
				action.func(p);
			});
		prevOrder = action.order;
	}

	return menu;
}

void view::InteractionsHandler::registerCommonActions()
{
	registerGenericAction("Delete", 
		[](QGraphicsItem* item)
		{
			if (dynamic_cast<Node*>(item)) return true;
			if (dynamic_cast<Edge*>(item)) return true;
			return false;
		},
		[=](const QPointF& p)
		{
			auto item = _sceneGraph->itemAt(p, QTransform());
			auto edge = dynamic_cast<Edge*>(item);
			if (edge != nullptr)
			{
				deleteEdge(edge);
			}
			else
			{
				auto node = dynamic_cast<Node*>(item);
				deleteNode(node);
			}
		}, QIcon(":/BleachUI/delete"));


	registerEmptySpaceAction("Start Graph", [=](const QPointF& p)
		{
			qDebug() << "Graph start result : " << _sceneGraph->getModelGraph()->start();
		}, QIcon(":/BleachUI/play"));

	registerEmptySpaceAction("Create Payload Node", [=](const QPointF& p)
		{
			auto node = createPayloadNode();
			node->setPos(p);
		}, QIcon(":/BleachUI/data"), 1);

	registerEmptySpaceAction("Create EndpointNode Node", [=](const QPointF& p)
		{
			auto node = createEndpointNode();
			node->setPos(p);
		}, QIcon(":/BleachUI/transfer"), 1);

	registerEmptySpaceAction("Create Viewer Node", [=](const QPointF& p)
		{
			auto node = createViewerNode();
			node->setPos(p);
		}, QIcon(":/BleachUI/view"), 1);

	registerEmptySpaceAction("Create Delay Node", [=](const QPointF& p)
		{
			auto node = createDelayNode();
			node->setPos(p);
		}, QIcon(":/BleachUI/stopWatch"), 1);

	registerEmptySpaceAction("Create Script Node", [=](const QPointF& p)
		{
			auto node = createScriptNode();
			node->setPos(p);
		}, QIcon(":/BleachUI/menu"), 1);

	registerEmptySpaceAction("Create Assertion Node", [=](const QPointF& p)
		{
			auto node = createAssertionNode();
			node->setPos(p);
		}, QIcon(":/ui/warning"), 1);

	registerNodeAction("Clone Node", [=](const QPointF& p)
		{
			//auto node = createAssertionNode();
			//node->setPos(p);
		}, QIcon(":/BleachUI/copy"), 1);
}
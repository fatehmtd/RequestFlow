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
#include "../logic/ModifierNode.h"
#include "../logic/DelayNode.h"
#include "../logic/ScriptNode.h"

view::InteractionsHandler::InteractionsHandler(SceneGraph* scene) : QObject(scene), _sceneGraph(scene)
{
}

view::InteractionsHandler::~InteractionsHandler()
{
}

view::Node* view::InteractionsHandler::createEndpointNode()
{
	auto nodeC = new model::Node(_sceneGraph->getModelGraph(), "Endpoint");

	auto slotC0 = nodeC->addInputSlot("in", model::Slot::DataType::CUSTOM + 0);
	auto slotC1 = nodeC->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto grNodeC = new logic::EndpointNode(nodeC);
	_sceneGraph->addItem(grNodeC);
	return grNodeC;
}

view::Node* view::InteractionsHandler::createPayloadNode()
{
	auto nodeA = new model::Node(_sceneGraph->getModelGraph(), "Payload");

	auto slotA = nodeA->addOutputSlot("out", model::Slot::DataType::CUSTOM + 0);
	auto grNodeA = new logic::PayloadNode(nodeA);
	_sceneGraph->addItem(grNodeA);
	return grNodeA;
}

view::Node* view::InteractionsHandler::createViewerNode()
{
	auto nodeB = new model::Node(_sceneGraph->getModelGraph(), "Result");

	auto slotB0 = nodeB->addInputSlot("in", model::Slot::DataType::CUSTOM);

	auto grNodeB = new logic::ViewerNode(nodeB);
	_sceneGraph->addItem(grNodeB);
	return grNodeB;
}

view::Node* view::InteractionsHandler::createModifierNode()
{
	auto modelNode = new model::Node(_sceneGraph->getModelGraph(), "Modifier");

	auto slotIn = modelNode->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotOut = modelNode->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto graphicNode = new logic::ModifierNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createDelayNode()
{
	auto modelNode = new model::Node(_sceneGraph->getModelGraph(), "Modifier");

	auto slotIn = modelNode->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotOut = modelNode->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto graphicNode = new logic::DelayNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createScriptNode()
{
	auto modelNode = new model::Node(_sceneGraph->getModelGraph(), "Script");

	auto slotIn = modelNode->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotOut = modelNode->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto graphicNode = new logic::ScriptNode(modelNode);
	_sceneGraph->addItem(graphicNode);
	return graphicNode;
}

view::Node* view::InteractionsHandler::createNode(QString nodeType)
{
	if (nodeType.contains("Delay")) return createDelayNode();
	if (nodeType.contains("Endpoint")) return createEndpointNode();
	if (nodeType.contains("Payload")) return createPayloadNode();
	if (nodeType.contains("Script")) return createScriptNode();
	if (nodeType.contains("Viewer")) return createViewerNode();
	return nullptr;
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
{
	QJSEngine engine;
	auto nodeValue = originalNode->toJSValue(engine);
	auto nodeTypeStr = nodeValue.property("_type").toString();
	auto node = createNode(nodeTypeStr);
	if (node != nullptr)
	{
		node->fromJSValue(nodeValue);
		node->update();
	}
	return node;
}

#include <QMenu>

QMenu* view::InteractionsHandler::createContextMenu(const QPointF& p)
{
	auto item = _sceneGraph->itemAt(p, QTransform());
	QMenu* menu = new QMenu();

	// handle right clicking on empty space
	if (item == nullptr)
	{
		auto startGraphAction = menu->addAction(QIcon(":/BleachUI/play"), "Start Graph");
		connect(startGraphAction, &QAction::triggered, this, [=]()
			{
				qDebug() << "Graph start result : " << _sceneGraph->getModelGraph()->start();
			});

		menu->addSeparator();

		auto createPayloadNodeAction = menu->addAction(QIcon(":/BleachUI/data"), "Create Payload Node");
		connect(createPayloadNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createPayloadNode();
				node->setPos(p);
			});

		auto createEndpointNodeAction = menu->addAction(QIcon(":/BleachUI/transfer"), "Create Endpoint Node");
		connect(createEndpointNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createEndpointNode();
				node->setPos(p);
			});

		auto createViewerNodeAction = menu->addAction(QIcon(":/BleachUI/view"), "Create Viewer Node");
		connect(createViewerNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createViewerNode();
				node->setPos(p);
			});

		auto createDelayNodeAction = menu->addAction(QIcon(":/BleachUI/stopWatch"), "Create Delay Node");
		connect(createDelayNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createDelayNode();
				node->setPos(p);
			});

		auto createScriptNodeAction = menu->addAction(QIcon(":/BleachUI/menu"), "Create Script Node");
		connect(createScriptNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createScriptNode();
				node->setPos(p);
			});

		menu->addSeparator();

		auto saveToFile = menu->addAction(QIcon(":/BleachUI/save"), "Save to file...");
		connect(saveToFile, &QAction::triggered, this, [=]()
			{
				_sceneGraph->persist("d:/scene.json");
			});

		auto loadFromFile = menu->addAction(QIcon(":/BleachUI/load"), "Load from file...");
		connect(loadFromFile, &QAction::triggered, this, [=]()
			{
				_sceneGraph->load("d:/scene.json");
			});
	}
	else
	{
		// handle right clicking on an edge
		{
			auto edge = dynamic_cast<Edge*>(item);
			if (edge != nullptr)
			{
				auto deleteEdgeAction = menu->addAction(QIcon(":/BleachUI/delete"), "Delete");
				connect(deleteEdgeAction, &QAction::triggered, this, [=]()
					{
						deleteEdge(edge);
					});
			}
		}

		// handle right clicking on a node
		{
			auto node = dynamic_cast<Node*>(item);
			if (node != nullptr)
			{
				auto deleteAction = menu->addAction(QIcon(":/BleachUI/delete"), "Delete");
				connect(deleteAction, &QAction::triggered, this, [=]()
					{
						deleteNode(node);
					});

				auto cloneAction = menu->addAction(QIcon(":/BleachUI/copy"), "Clone");

				connect(cloneAction, &QAction::triggered, this, [=]()
					{
						auto newNode = cloneNode(node);
						if (newNode != nullptr)
						{
							newNode->setPos(p);
						}
					});
			}
		}
	}

	return menu;
}

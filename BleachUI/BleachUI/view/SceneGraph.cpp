#include "SceneGraph.h"
#include <QPainter>
#include <QLine>
#include <QDebug>
#include <QPainterPath>

#include <QMenu>

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

view::SceneGraph::SceneGraph(model::Graph* modelGraph, QObject* parent) : QGraphicsScene(parent), _modelGraph(modelGraph)
{
	setupUi();
}

view::Slot* view::SceneGraph::findbyModel(model::Slot* slot) const
{
	for (auto item : items())
	{
		auto gr = dynamic_cast<view::Slot*>(item);
		if (gr != nullptr)
		{
			if (gr->getModelSlot() == slot) return gr;
		}
	}
	return nullptr;
}

view::Node* view::SceneGraph::findbyModel(model::Node* node) const
{
	for (auto item : items())
	{
		auto gr = dynamic_cast<view::Node*>(item);
		if (gr != nullptr)
		{
			if (gr->getModelNode() == node) return gr;
		}
	}
	return nullptr;
}

view::Edge* view::SceneGraph::findbyModel(model::Edge* edge) const
{
	for (auto item : items())
	{
		auto gr = dynamic_cast<view::Edge*>(item);
		if (gr != nullptr)
		{
			if (gr->getModelEdge() == edge) return gr;
		}
	}
	return nullptr;
}

model::Graph* view::SceneGraph::getModelGraph() const
{
	return _modelGraph;
}

void view::SceneGraph::registerEdgeAction(QString name, std::function<void(view::Edge*)> func)
{
	_edgesActions[name] = func;
}

void view::SceneGraph::registerNodeAction(QString name, std::function<void(view::Node*)> func)
{
	_nodesActions[name] = func;
}

void view::SceneGraph::clearNodes()
{
	for (auto item : items())
	{
		auto gr = dynamic_cast<view::Node*>(item);
		if (gr != nullptr)
		{
			gr->clearUI();
		}
	}
}

void view::SceneGraph::drawBackground(QPainter* painter, const QRectF& rect)
{
	QGraphicsScene::drawBackground(painter, rect);
	return;
	
	const int left = floor(rect.left());
	const int right = ceil(rect.right());
	const int top = floor(rect.top());
	const int bottom = ceil(rect.bottom());

	// cells
	{
		QVector<QLine> _lines;

		const int firstLeft = left - (left % _cellSize);
		const int firstTop = top - (top % _cellSize);

		for (int i = firstLeft; i < right; i += _cellSize)
		{
			_lines.push_back(QLine(i, top, i, bottom));
		}

		for (int i = firstTop; i < bottom; i += _cellSize)
		{
			_lines.push_back(QLine(left, i, right, i));
		}

		QPen pen(QBrush(_lightGrid), 1.0f);
		pen.setStyle(Qt::PenStyle::DashLine);
		painter->setPen(pen);
		painter->drawLines(_lines);
	}

	// blocks
	{
		QVector<QLine> _lines;

		const int blockSizePixels = _cellSize * _blockSize;

		const int firstLeft = left - (left % blockSizePixels);
		const int firstTop = top - (top % blockSizePixels);

		for (int i = firstLeft; i < right; i += blockSizePixels)
		{
			_lines.push_back(QLine(i, top, i, bottom));
		}

		for (int i = firstTop; i < bottom; i += blockSizePixels)
		{
			_lines.push_back(QLine(left, i, right, i));
		}

		QPen pen(QBrush(_darkGrid), 1.0f);
		painter->setPen(pen);
		painter->drawLines(_lines);
	}

	// Temporary edge
	{
		if (_originSlot != nullptr)
		{
			_connectionEdge->setDestination(_cursorPosition);
		}
	}
}

#include <QGraphicsView>

void view::SceneGraph::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto item = itemAt(event->scenePos(), QTransform());

	// handle right clicking on empty space
	if (item == nullptr)
	{
		QMenu menu;
		auto startGraphAction = menu.addAction("Start Graph");
		connect(startGraphAction, &QAction::triggered, this, [=]()
			{
				qDebug() << "Graph start result : " << _modelGraph->start();
			});

		auto createPayloadNodeAction = menu.addAction("Create Payload Node");
		connect(createPayloadNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createPayloadNode();
				node->setPos(event->scenePos());
			});

		auto createEndpointNodeAction = menu.addAction("Create Endpoint Node");
		connect(createEndpointNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createEndpointNode();
				node->setPos(event->scenePos());
			});

		auto createViewerNodeAction = menu.addAction("Create Viewer Node");
		connect(createViewerNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createViewerNode();
				node->setPos(event->scenePos());
			});

		auto createModiferNodeAction = menu.addAction("Create Modifier Node");
		connect(createModiferNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createModifierNode();
				node->setPos(event->scenePos());
			});

		auto createDelayNodeAction = menu.addAction("Create Delay Node");
		connect(createDelayNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createDelayNode();
				node->setPos(event->scenePos());
			});

		auto createScriptNodeAction = menu.addAction("Create Script Node");
		connect(createScriptNodeAction, &QAction::triggered, this, [=]()
			{
				auto node = createScriptNode();
				node->setPos(event->scenePos());
			});

		menu.exec(event->screenPos());
	}
	else
	{
		// handle right clicking on an edge
		{
			auto edge = dynamic_cast<Edge*>(item);
			if (edge != nullptr)
			{
				deleteEdge(edge);
				return;
			}
		}

		// handle right clicking on a node
		{
			auto node = dynamic_cast<Node*>(item);
			if (node != nullptr)
			{
				QMenu menu;
				auto deleteAction = menu.addAction("Delete");
				connect(deleteAction, &QAction::triggered, this, [=]()
					{
						deleteNode(node);
					});

				auto cloneAction = menu.addAction("Clone");

				connect(deleteAction, &QAction::triggered, this, [=]()
					{
						qDebug() << "clone";
						//delete node;
					});
				menu.exec(event->screenPos());
			}
		}
	}

	QGraphicsScene::contextMenuEvent(event);
}

void view::SceneGraph::setupUi()
{
	_cellSize = 40; // in pixels
	_blockSize = 10; // in cells

	_background = QColor(140, 140, 140);
	_lightGrid = QColor(120, 120, 120);
	_darkGrid = QColor(60, 60, 60);
	//*/
/*
	_background = colors::whiteGrey;
	_lightGrid = colors::lightestGrey;
	_darkGrid = colors::lightGrey;
	//*/

	const int gridSize = 1 << 17;
	const int hgridSize = gridSize >> 1;
	qDebug() << gridSize << hgridSize;

	setSceneRect(QRectF(-hgridSize, -hgridSize, gridSize, gridSize));
	setBackgroundBrush(QBrush(_background));

	_connectionEdge = new ConnectionEdge();
	addItem(_connectionEdge);

	connect(_modelGraph, &model::Graph::started, [=]()
		{
			clearNodes();
		});

	//createSampleScenario();
}

#include <QGraphicsWidget>

void view::SceneGraph::createSampleScenario()
{

	auto nodeA = new model::Node(_modelGraph, "Payload");
	auto nodeB = new model::Node(_modelGraph, "Result");
	auto nodeC = new model::Node(_modelGraph, "Endpoint");

	auto slotA = nodeA->addOutputSlot("out", model::Slot::DataType::CUSTOM + 0);
	auto slotB0 = nodeB->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotC0 = nodeC->addInputSlot("in", model::Slot::DataType::CUSTOM + 0);
	auto slotC1 = nodeC->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto edgeAC = new model::Edge(_modelGraph);
	edgeAC->setOrigin(slotA);
	edgeAC->setDestination(slotC0);

	auto edgeCB = new model::Edge(_modelGraph);
	edgeCB->setOrigin(slotC1);
	edgeCB->setDestination(slotB0);

	auto grNodeA = new logic::PayloadNode(nodeA);
	auto grNodeB = new logic::ViewerNode(nodeB);
	auto grNodeC = new logic::EndpointNode(nodeC);

	grNodeA->setPos(0, 0);
	grNodeB->setPos(1000, 0);
	grNodeC->setPos(600, 0);

	addItem(grNodeA);
	addItem(grNodeB);

	addItem(grNodeC);

	for (auto edge : _modelGraph->getEdges())
	{
		auto originSlot = findbyModel(edge->getOriginSlot());
		auto destinationSlot = findbyModel(edge->getDestinationSlot());
		if (destinationSlot->acceptConnection(originSlot))
		{
			auto grEdge = new view::Edge(this, edge);
			addItem(grEdge);
		}
	}


	auto scriptNode = createScriptNode();
	scriptNode->setPos(300, -800);
	//*/

	//createEndpointNode();
	//createViewerNode();
}

view::Node* view::SceneGraph::createEndpointNode()
{
	auto nodeC = new model::Node(_modelGraph, "Endpoint");

	auto slotC0 = nodeC->addInputSlot("in", model::Slot::DataType::CUSTOM + 0);
	auto slotC1 = nodeC->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto grNodeC = new logic::EndpointNode(nodeC);
	addItem(grNodeC);
	return grNodeC;
}

view::Node* view::SceneGraph::createPayloadNode()
{
	auto nodeA = new model::Node(_modelGraph, "Payload");

	auto slotA = nodeA->addOutputSlot("out", model::Slot::DataType::CUSTOM + 0);
	auto grNodeA = new logic::PayloadNode(nodeA);
	addItem(grNodeA);
	return grNodeA;
}

view::Node* view::SceneGraph::createViewerNode()
{
	auto nodeB = new model::Node(_modelGraph, "Result");

	auto slotB0 = nodeB->addInputSlot("in", model::Slot::DataType::CUSTOM);

	auto grNodeB = new logic::ViewerNode(nodeB);
	addItem(grNodeB);
	return grNodeB;
}

view::Node* view::SceneGraph::createModifierNode()
{
	auto modelNode = new model::Node(_modelGraph, "Modifier");

	auto slotIn = modelNode->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotOut = modelNode->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto graphicNode = new logic::ModifierNode(modelNode);
	addItem(graphicNode);
	return graphicNode;
}

view::Node* view::SceneGraph::createDelayNode()
{
	auto modelNode = new model::Node(_modelGraph, "Modifier");

	auto slotIn = modelNode->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotOut = modelNode->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto graphicNode = new logic::DelayNode(modelNode);
	addItem(graphicNode);
	return graphicNode;
}

view::Node* view::SceneGraph::createScriptNode()
{
	auto modelNode = new model::Node(_modelGraph, "Script");

	auto slotIn = modelNode->addInputSlot("in", model::Slot::DataType::CUSTOM);
	auto slotOut = modelNode->addOutputSlot("out", model::Slot::DataType::CUSTOM);

	auto graphicNode = new logic::ScriptNode(modelNode);
	addItem(graphicNode);
	return graphicNode;
}

void view::SceneGraph::deleteNode(Node* node)
{
	auto edges = getModelGraph()->findEdges(node->getModelNode());
	for (auto edge : edges)
	{
		auto grEdge = findbyModel(edge);
		delete grEdge;
		delete edge;
	}

	delete node->getModelNode();
	delete node;
}

void view::SceneGraph::deleteEdge(Edge* edge)
{
	delete edge->getModelEdge();
	delete edge;
}

void view::SceneGraph::cloneNode(Node* node)
{
}

void view::SceneGraph::createEdge()
{
	if (_originSlot && _destinationSlot)
	{
		auto modelEdge = _modelGraph->connectSlots(
			dynamic_cast<model::OutputSlot*>(_originSlot->getModelSlot()),
			dynamic_cast<model::InputSlot*>(_destinationSlot->getModelSlot()));

		if (modelEdge != nullptr)
		{
			addItem(new Edge(this, modelEdge));
		}
	}
}

void view::SceneGraph::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	//bring to front
	if (event->button())
	{
		bringToFront(event->scenePos());
	}

	if (event->button() == Qt::MouseButton::LeftButton)
	{
		auto slot = dynamic_cast<Slot*>(itemAt(event->scenePos(), QTransform()));

		if (slot != nullptr && !slot->isInput())
		{
			_originSlot = slot;
			_connectionEdge->setOrigin(_originSlot);
			_connectionEdge->setDestination(event->scenePos());
		}
	}

	_cursorPosition = event->scenePos();

	QGraphicsScene::mousePressEvent(event);
}

void view::SceneGraph::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//bring to front
	if(event->button())
	{
		bringToFront(event->scenePos());
	}

	_cursorPosition = event->scenePos();

	if (_originSlot != nullptr)
	{
		auto slot = dynamic_cast<Slot*>(itemAt(event->scenePos(), QTransform()));
		if (slot != nullptr)
		{
			bool eligibleStatus = slot->acceptConnection(_originSlot);
			if (eligibleStatus)
			{
				_destinationSlot = slot;
				_connectionEdge->setEligible();
			}
			else
			{
				_destinationSlot = nullptr;
				_connectionEdge->setNotEligible();
			}
		}
		else
		{
			_destinationSlot = nullptr;
			_connectionEdge->setNoCandidateAvaible();
		}
		_connectionEdge->setDestination(_cursorPosition);
		
		_connectionEdge->update();
	}

	QGraphicsScene::mouseMoveEvent(event);
}

void view::SceneGraph::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	createEdge();
	_destinationSlot = nullptr;
	_originSlot = nullptr;
	_connectionEdge->setOrigin(_originSlot);
	QGraphicsScene::mouseReleaseEvent(event);
}

void view::SceneGraph::bringToFront(QPointF pos) const
{
	auto item = itemAt(pos, QTransform());
	auto node = dynamic_cast<Node*>(item);
	if (node != nullptr)
	{
		auto collidingItemsList = collidingItems(item);
		for (auto collidingItem : collidingItemsList)
		{
			auto collidingNode = dynamic_cast<Node*>(collidingItem);

			if (collidingNode == nullptr) continue;
			if (collidingItem == item) continue;

			collidingItem->stackBefore(node);
		}
	}
}

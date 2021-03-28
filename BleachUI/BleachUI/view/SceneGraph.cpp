#include "SceneGraph.h"
#include <QPainter>
#include <QLine>
#include <QDebug>
#include <QPainterPath>

#include <QJSValue>
#include <QJSEngine>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJSValueIterator>

#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QMetaObject>

#include <QMenu>

#include "Node.h"
#include "Slot.h"
#include "Colors.h"
#include "Edge.h"
#include "ConnectionEdge.h"
#include "InteractionsHandler.h"

view::SceneGraph::SceneGraph(model::Graph* modelGraph, QObject* parent) : QGraphicsScene(parent),
_modelGraph(modelGraph)
{
	_interactionsHandler = new InteractionsHandler(this);
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

QList<view::Node*> view::SceneGraph::getNodes() const
{
	QList<Node*> nodesList;

	for (auto item : items())
	{
		auto node = dynamic_cast<Node*>(item);
		if (node != nullptr)
			nodesList << node;
	}

	return nodesList;
}

QList<view::Edge*> view::SceneGraph::getEdges() const
{
	auto modelEdgesList = getModelGraph()->getEdges();
	QList<Edge*> edgesList;
	for (auto modelEdge : modelEdgesList)
	{
		edgesList << findbyModel(modelEdge);
	}
	return edgesList;
}

view::Node* view::SceneGraph::getNodeAt(const QPointF& p) const
{
	auto item = itemAt(p, QTransform());
	Node* node = nullptr;

	// Go up the hierarchy to find a parent node;
	while (item != nullptr)
	{
		node = dynamic_cast<Node*>(item);
		if (node != nullptr) break;
		item = item->parentItem();
	}

	return node;
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

void view::SceneGraph::clearScene()
{
	for (auto node : getNodes())
	{
		_interactionsHandler->deleteNode(node);
	}
}

void view::SceneGraph::persist(const QString& fileName) const
{
	QJSEngine engine;
	QJSValue project = engine.newObject();

	// nodes
	QList<Node*> nodesList = getNodes();
	{
		auto nodesValue = engine.newArray(nodesList.size());
		int index = 0;
		for (auto node : nodesList)
		{
			nodesValue.setProperty(index++, node->toJSValue(engine));
		}
		project.setProperty("_nodes", nodesValue);
	}

	// edges
	{
		QList<Edge*> edgesList = getEdges();

		auto edgesValue = engine.newArray(edgesList.size());

		int index = 0;
		for (auto edge : edgesList)
		{
			auto originNode = edge->getOriginSlot()->getNode();
			auto destinationNode = edge->getDestinationSlot()->getNode();
			int idxOrigin = nodesList.indexOf(originNode);
			int idxDestination = nodesList.indexOf(destinationNode);
			QJSValue edgeValue = engine.newObject();
			edgeValue.setProperty("_o", idxOrigin);
			edgeValue.setProperty("_d", idxDestination);
			edgesValue.setProperty(index++, edgeValue);
		}

		project.setProperty("_edges", edgesValue);
	}

	// save to file
	auto document = QJsonDocument::fromVariant(project.toVariant());

	QFile fp(fileName);
	if (fp.open(QIODevice::Text | QIODevice::WriteOnly))
	{
		QTextStream out(&fp);
		out << QString(document.toJson(QJsonDocument::JsonFormat::Compact));
	}
}

bool view::SceneGraph::load(const QString& fileName)
{
	QFile fp(fileName);
	if (fp.open(QIODevice::Text | QIODevice::ReadOnly))
	{
		QTextStream in(&fp);
		auto contents = in.readAll();
		QJSEngine engine;

		auto project = engine.toScriptValue<QVariant>(QJsonDocument::fromJson(contents.toUtf8()).toVariant());

		if (project.isError()) return false;

		clearScene(); // delete all scene nodes

		auto nodes = project.property("_nodes");
		QList<Node*> nodesList;
		{
			int numNodes = nodes.property("length").toInt();
			for (auto i = 0; i < numNodes; i++)
			{
				auto nodeValue = nodes.property(i);
				auto nodeTypeStr = nodeValue.property("_type").toString();
				auto node = _interactionsHandler->createNode(nodeTypeStr);
				if (node != nullptr)
				{
					node->fromJSValue(nodeValue);
					node->update();
					nodesList << node;
				}
			}
		}

		auto edges = project.property("_edges");
		{
			int numEdges = edges.property("length").toInt();
			for (auto i = 0; i < numEdges; i++)
			{
				auto edgeValue = edges.property(i);
				int originIndex = edgeValue.property("_d").toInt();
				int destinationIndex = edgeValue.property("_o").toInt();

				auto originNode = nodesList[originIndex];
				auto destinationNode = nodesList[destinationIndex];
				if (originNode != nullptr && destinationNode != nullptr)
				{
					auto originSlot = originNode->getModelNode()->getOutputSlots().first();
					auto destinationSlot = destinationNode->getModelNode()->getInputSlots().first();
					auto edge = _modelGraph->connectSlots(originSlot, destinationSlot);
					if (edge != nullptr)
					{
						auto grEdge = new view::Edge(this, edge);
						addItem(grEdge);
					}
				}
			}

		}
		return true;
	}

	return false;
}

void view::SceneGraph::drawBackground(QPainter* painter, const QRectF& rect)
{
	drawPointsBackground(painter, rect);
	//drawGridBackground(painter, rect);
}

void view::SceneGraph::drawPointsBackground(QPainter* painter, const QRectF& rect)
{
	const int left = floor(rect.left());
	const int right = ceil(rect.right());
	const int top = floor(rect.top());
	const int bottom = ceil(rect.bottom());

	// points

	const int firstLeft = left - (left % _cellSize);
	const int firstTop = top - (top % _cellSize);
	int count = (1 + (right - firstLeft) / _cellSize) * (1 + (bottom - firstTop) / _cellSize);
	QVector<QPointF> points(count);

	int index = 0;
	for (int i = firstLeft; i < right; i += _cellSize)
	{
		for (int j = firstTop; j < bottom; j += _cellSize)
		{
			points[index++] = QPointF(i, j);
		}
	}

	QPen pen(QBrush(QColor("#E4E3E4")), 4.0f);
	//pen.setStyle(Qt::PenStyle::DashLine);
	painter->setPen(pen);
	painter->drawPoints(points.data(), points.size());
}

void view::SceneGraph::drawGridBackground(QPainter* painter, const QRectF& rect)
{
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
}

void view::SceneGraph::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	auto menu = _interactionsHandler->createContextMenu(event->scenePos());
	menu->exec(event->screenPos());
	menu->deleteLater();

	QGraphicsScene::contextMenuEvent(event);
}

void view::SceneGraph::setupUi()
{
	_cellSize = 40; // in pixels
	_blockSize = 10; // in cells

	//_background = QColor(140, 140, 140);
	_background = QColor("#F4F3F4");
	_lightGrid = QColor(120, 120, 120);
	_darkGrid = QColor(60, 60, 60);
	//*/
/*
	_background = colors::whiteGrey;
	_lightGrid = colors::lightestGrey;
	_darkGrid = colors::lightGrey;
	//*/

	const int gridSize = 1 << 18;
	const int hgridSize = gridSize >> 1;

	setSceneRect(QRectF(-hgridSize, -hgridSize, gridSize, gridSize));
	setBackgroundBrush(QBrush(_background));

	_connectionEdge = new ConnectionEdge();
	addItem(_connectionEdge);

	connect(_modelGraph, &model::Graph::started, [=]()
		{
			clearNodes();
		});
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
	if (event->button())
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
	auto node = getNodeAt(pos);

	if (node != nullptr)
	{
		bringToFront(node);
	}
}

void view::SceneGraph::bringToFront(Node* node) const
{
	auto collidingItemsList = collidingItems(node);
	for (auto collidingItem : collidingItemsList)
	{
		auto collidingNode = dynamic_cast<Node*>(collidingItem);

		if (collidingNode == nullptr) continue;
		if (collidingItem == node) continue;

		collidingItem->stackBefore(node);
	}
}

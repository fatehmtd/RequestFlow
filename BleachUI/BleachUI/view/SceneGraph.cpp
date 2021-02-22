#include "SceneGraph.h"
#include <QPainter>
#include <QLine>
#include <QDebug>
#include <QPainterPath>

#include "Node.h"
#include "Slot.h"
#include "Colors.h"
#include "Edge.h"
#include "ConnectionEdge.h"
#include "../logic/PayloadNode.h"
#include "../logic/EndpointNode.h"
#include "../logic/ViewerNode.h"

view::SceneGraph::SceneGraph(QObject* parent) : QGraphicsScene(parent)
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
	return nullptr;
}

void view::SceneGraph::drawBackground(QPainter* painter, const QRectF& rect)
{
	QGraphicsScene::drawBackground(painter, rect);

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

void view::SceneGraph::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	qDebug() << __FUNCTION__;
	QGraphicsScene::contextMenuEvent(event);
	qDebug() << _graph->start();
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

	const int gridSize = 10240;
	const int hgridSize = gridSize / 2;

	setSceneRect(QRectF(-hgridSize, -hgridSize, gridSize, gridSize));
	setBackgroundBrush(QBrush(_background));

	_connectionEdge = new ConnectionEdge();
	addItem(_connectionEdge);

	createSampleScenario();
}

#include <QGraphicsWidget>

void view::SceneGraph::createSampleScenario()
{
	_graph = new model::Graph();

	auto nodeA = new model::Node(_graph, "Payload");
	auto nodeB = new model::Node(_graph, "Result");
	auto nodeC = new model::Node(_graph, "Endpoint");

	auto slotA = nodeA->addOutputSlot("out", model::Slot::DataType::CUSTOM + 1);
	auto slotB0 = nodeB->addInputSlot("in", model::Slot::DataType::CUSTOM + 1);
	auto slotC0 = nodeC->addInputSlot("in", model::Slot::DataType::CUSTOM + 1);
	auto slotC1 = nodeC->addOutputSlot("out", model::Slot::DataType::CUSTOM + 1);

	auto edgeAC = new model::Edge(_graph);
	edgeAC->setOrigin(slotA);
	edgeAC->setDestination(slotC0);

	auto edgeCB = new model::Edge(_graph);
	edgeCB->setOrigin(slotC1);
	edgeCB->setDestination(slotB0);


	auto grNodeA = new logic::PayloadNode(nodeA);
	auto grNodeB = new logic::ViewerNode(nodeB);
	auto grNodeC = new logic::EndpointNode(nodeC);

	grNodeA->setPos(0, 0);
	grNodeB->setPos(800, 0);
	grNodeC->setPos(400, 0);

	addItem(grNodeA);
	addItem(grNodeB);
	addItem(grNodeC);	

	for (auto edge : _graph->getEdges())
	{
		auto originSlot = findbyModel(edge->getOriginSlot());
		auto destinationSlot = findbyModel(edge->getDestinationSlot());
		if (destinationSlot->acceptConnection(originSlot))
		{
			auto grEdge = new view::Edge(destinationSlot, originSlot);
			addItem(grEdge);
		}
	}
}

void view::SceneGraph::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mousePressEvent(event);

	if (event->button() == Qt::MouseButton::LeftButton)
	{
		auto slot = dynamic_cast<Slot*>(itemAt(event->scenePos(), QTransform()));
		if (slot != nullptr && !slot->isInput())
		{
			_originSlot = slot;
			_connectionEdge->setOrigin(_originSlot);
		}
	}

	_cursorPosition = event->scenePos();
}

void view::SceneGraph::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mouseMoveEvent(event);
	_cursorPosition = event->scenePos();

	if (_originSlot != nullptr)
	{
		auto slot = dynamic_cast<Slot*>(itemAt(event->scenePos(), QTransform()));
		if (slot != nullptr)
		{
			bool eligibleStatus = slot->acceptConnection(_originSlot);
			_destinationSlot = eligibleStatus ? slot : nullptr;
			if (eligibleStatus)
			{
				_connectionEdge->setEligible();
			}
			else
			{
				_connectionEdge->setNotEligible();
			}
		}
		else
		{
			_destinationSlot = nullptr;
			_connectionEdge->setNoCandidateAvaible();
		}
		update();
	}
}

void view::SceneGraph::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	//auto item = itemAt(event->scenePos(), QTransform());

	if (_originSlot && _destinationSlot)
	{
		auto modelEdge = _graph->connectSlots(
			dynamic_cast<model::OutputSlot*>(_originSlot->getModelSlot()), 
			dynamic_cast<model::InputSlot*>(_destinationSlot->getModelSlot()));

		if (modelEdge != nullptr)
		{
			addItem(new Edge(_destinationSlot, _originSlot));
		}
	}

	_destinationSlot = nullptr;
	_originSlot = nullptr;
	_connectionEdge->setOrigin(_originSlot);
}

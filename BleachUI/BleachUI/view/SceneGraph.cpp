#include "SceneGraph.h"
#include <QPainter>
#include <QLine>
#include <QDebug>
#include <QPainterPath>

#include "Node.h"
#include "Slot.h"
#include "Colors.h"
#include "Edge.h"

view::SceneGraph::SceneGraph(QObject* parent) : QGraphicsScene(parent)
{
	setupUi();
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
}

void view::SceneGraph::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mousePressEvent(event);
}

void view::SceneGraph::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mouseReleaseEvent(event);
}

void view::SceneGraph::setupUi()
{
	_cellSize = 40; // in pixels
	_blockSize = 5; // in cells
	
	_background = QColor(140, 140, 140);
	_lightGrid = QColor(120, 120, 120);
	_darkGrid = QColor(60, 60, 60);
	//*/

	/*
	_background = colors::whiteGrey;
	_lightGrid = colors::lightestGrey;
	_darkGrid = colors::lightGrey;
	//*/

	const int gridSize = 102400;
	const int hgridSize = gridSize / 2;

	setSceneRect(QRectF(-hgridSize, -hgridSize, gridSize, gridSize));
	setBackgroundBrush(QBrush(_background));

	auto nodeA = new Node();
	auto nodeB = new Node();
	auto nodeC = new Node();
	auto nodeD = new Node();

	nodeA->setPos(0, 0);
	nodeB->setPos(100, 50);
	nodeC->setPos(150, 200);
	nodeD->setPos(-50, -100);

	auto nodeAslotA = new Slot(nodeA, true);
	auto nodeAslotB = new Slot(nodeA, true);
	auto nodeBslotA = new Slot(nodeB, false);
	auto nodeCslotA = new Slot(nodeC, false);
	auto nodeDslotA = new Slot(nodeD, true);
	
	addItem(nodeA);
	addItem(nodeB);
	addItem(nodeC);
	addItem(nodeD);

	addItem(new Edge(nodeAslotA, nodeBslotA));
	addItem(new Edge(nodeAslotB, nodeCslotA));
	addItem(new Edge(nodeDslotA, nodeCslotA));
}

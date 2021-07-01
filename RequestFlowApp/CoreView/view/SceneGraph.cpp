#include "SceneGraph.h"
#include <QDebug>
#include <QLine>
#include <QPainter>
#include <QPainterPath>

#include <QFile>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueIterator>
#include <QJsonDocument>
#include <QTextStream>

#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QMetaObject>

#include <QSvgGenerator>

#include <QMenu>

#include "Colors.h"
#include "ConnectionEdge.h"
#include "Edge.h"
#include "InteractionsHandler.h"
#include "Node.h"
#include "Slot.h"

#include "logic/AssertionNode.h"
#include "logic/DelayNode.h"
#include "logic/EndpointNode.h"
#include "logic/PayloadNode.h"
#include "logic/ScriptNode.h"
#include "logic/ViewerNode.h"
#include "logic/ExternalNode.h"

#include "SceneGraphWidget.h"

#include <model/EndpointEntry.h>
#include <QMimeData>
#include <QStandardItem>
#include <math.h>
#include <functional>

view::SceneGraph::SceneGraph(model::Graph *modelGraph, QObject *parent)
    : QGraphicsScene(parent), _modelGraph(modelGraph)
{
    _interactionsHandler = new InteractionsHandler(this);
    setupUi();
    createGeometricNodesForModel();
}

view::Slot *view::SceneGraph::findbyModel(model::Slot *slot) const
{
    for (auto item : items()) {
        auto gr = dynamic_cast<view::Slot *>(item);
        if (gr != nullptr) {
            if (gr->getModelSlot() == slot)
                return gr;
        }
    }
    return nullptr;
}

view::Node *view::SceneGraph::findbyModel(model::Node *node) const
{
    for (auto item : items()) {
        auto gr = dynamic_cast<view::Node *>(item);
        if (gr != nullptr) {
            if (gr->getModelNode() == node)
                return gr;
        }
    }
    return nullptr;
}

view::Edge *view::SceneGraph::findbyModel(model::Edge *edge) const
{
    for (auto item : items()) {
        auto gr = dynamic_cast<view::Edge *>(item);
        if (gr != nullptr) {
            if (gr->getModelEdge() == edge)
                return gr;
        }
    }
    return nullptr;
}

void view::SceneGraph::createGeometricNodesForModel()
{
    for (auto modelNode : getModelGraph()->getNodes()) {
        auto node = createGeometryForModel(modelNode);
        addItem(node);
        node->update();
        for (auto child : node->childItems()) {
            child->update();
        }
    }

    for (auto modelEdge : getModelGraph()->getEdges()) {
        auto edge = new view::Edge(this, modelEdge);
        addItem(edge);
        edge->update();
    }

    update();
}

QList<view::Node *> view::SceneGraph::getNodes() const
{
    QList<Node *> nodesList;

    for (auto item : items()) {
        auto node = dynamic_cast<Node *>(item);
        if (node != nullptr)
            nodesList << node;
    }

    return nodesList;
}

QList<view::Edge *> view::SceneGraph::getEdges() const
{
    auto modelEdgesList = getModelGraph()->getEdges();
    QList<Edge *> edgesList;
    for (auto modelEdge : modelEdgesList) {
        edgesList << findbyModel(modelEdge);
    }
    return edgesList;
}

QList<view::Node *> view::SceneGraph::getSelectedNodes() const
{
    QList<view::Node*> nodes;
    for(auto node: getNodes())
    {
        if(node->isSelected())
            nodes << node;
    }

    return nodes;
}

view::Node *view::SceneGraph::getNodeAt(const QPointF &p) const
{
    auto item = itemAt(p, QTransform());
    Node *node = nullptr;

    // Go up the hierarchy to find a parent node;
    while (item != nullptr) {
        node = dynamic_cast<Node *>(item);
        if (node != nullptr)
            break;
        item = item->parentItem();
    }

    return node;
}

model::Graph *view::SceneGraph::getModelGraph() const
{
    return _modelGraph;
}

void view::SceneGraph::clearNodes()
{
    for (auto item : items()) {
        auto gr = dynamic_cast<view::Node *>(item);
        if (gr != nullptr) {
            gr->clearUI();
        }
    }
}

void view::SceneGraph::clearScene()
{
    for (auto node : getNodes()) {
        _interactionsHandler->deleteNode(node);
    }
}

view::Node *view::SceneGraph::createGeometryForModel(model::Node *node)
{
    QMap<QString, std::function<view::Node *(model::Node *)>> nodesMap;
    nodesMap["Payload"] = [](model::Node *modelNode) {
        return new logic::PayloadNode((model::PayloadNode *) modelNode);
    };

    nodesMap["Endpoint"] = [](model::Node *modelNode) {
        return new logic::EndpointNode((model::EndpointNode *) modelNode);
    };
    nodesMap["Script"] = [](model::Node *modelNode) {
        return new logic::ScriptNode((model::ScriptNode *) modelNode);
    };
    nodesMap["Viewer"] = [](model::Node *modelNode) {
        return new logic::ViewerNode((model::ViewerNode *) modelNode);
    };
    nodesMap["Delay"] = [](model::Node *modelNode) {
        return new logic::DelayNode((model::DelayNode *) modelNode);
    };
    nodesMap["Assertion"] = [](model::Node *modelNode) {
        return new logic::AssertionNode((model::AssertionNode *) modelNode);
    };
    nodesMap["External"] = [](model::Node *modelNode) {
        return new logic::ExternalNode((model::ExternalNode *) modelNode);
    };

    return nodesMap[node->getType()](node);
}

void view::SceneGraph::drawBackground(QPainter *painter, const QRectF &rect)
{
    if(!_drawBackground) return;

    switch (getBackgroundType()) {
    case SOLID:
        painter->fillRect(rect, QColor("#A8A8A8"));
        break;
    case DOTS:
        drawDotsBackground(painter, rect);
        break;
    case CROSSES:
        drawCrossBackground(painter, rect);
        break;
    case GRID:
        drawGridBackground(painter, rect);
        break;
    case QUADS:
        drawQuadsBackground(painter, rect);
        break;
    }


    {/*
        auto nodes = getNodes();
        if(!nodes.isEmpty())
        {
            auto rect = computeBoundingRect(nodes, 100);
            int left = rect.x();
            int right = left + rect.width();
            int top = rect.y();
            int bottom = top + rect.height();

        // points
        int _cellSize = 10;
        const int firstLeft = left - ((int)left % _cellSize);
        const int firstTop = top - ((int)top % _cellSize);
        int count = (1 + (right - firstLeft) / _cellSize) * (1 + (bottom - firstTop) / _cellSize);
        QVector<QPointF> points(count);

        int index = 0;
        for (int i = firstLeft; i < right; i += _cellSize) {
            for (int j = firstTop; j < bottom; j += _cellSize) {
                points[index++] = QPointF(i, j);
            }
        }

        QPen pen(QBrush(QColor("#AA0000")), 4.0f);
        //pen.setStyle(Qt::PenStyle::DashLine);
        painter->setPen(pen);
        painter->drawPoints(points.data(), points.size());
    }*/

}
}

void view::SceneGraph::drawForeground(QPainter *painter, const QRectF &rect)
{
    customUpdate();
}

void view::SceneGraph::drawDotsBackground(QPainter *painter, const QRectF &rect) const
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
    for (int i = firstLeft; i < right; i += _cellSize) {
        for (int j = firstTop; j < bottom; j += _cellSize) {
            points[index++] = QPointF(i, j);
        }
    }

    QPen pen(QBrush(QColor("#E4E3E4")), 4.0f);
    //painter->fillRect(rect, backgroundBrush());
    //pen.setStyle(Qt::PenStyle::DashLine);
    painter->setPen(pen);
    painter->drawPoints(points.data(), points.size());
}

void view::SceneGraph::drawGridBackground(QPainter *painter, const QRectF &rect) const
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

        for (int i = firstLeft; i < right; i += _cellSize) {
            _lines.push_back(QLine(i, top, i, bottom));
        }

        for (int i = firstTop; i < bottom; i += _cellSize) {
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

        for (int i = firstLeft; i < right; i += blockSizePixels) {
            _lines.push_back(QLine(i, top, i, bottom));
        }

        for (int i = firstTop; i < bottom; i += blockSizePixels) {
            _lines.push_back(QLine(left, i, right, i));
        }

        QPen pen(QBrush(_darkGrid), 1.0f);
        //painter->fillRect(rect, backgroundBrush());
        painter->setPen(pen);
        painter->drawLines(_lines);
    }
}

void view::SceneGraph::drawCrossBackground(QPainter *painter, const QRectF &rect) const
{
    const int left = floor(rect.left());
    const int right = ceil(rect.right());
    const int top = floor(rect.top());
    const int bottom = ceil(rect.bottom());

    // points
    const int firstLeft = left - (left % _cellSize);
    const int firstTop = top - (top % _cellSize);
    int count = (1 + (right - firstLeft) / _cellSize) * (1 + (bottom - firstTop) / _cellSize);
    //QVector<QPointF> points(count);
    QVector<QLine> lines(count * 2);

    float halfLen = 5;

    int index = 0;
    for (int i = firstLeft; i < right; i += _cellSize) {
        for (int j = firstTop; j < bottom; j += _cellSize) {
            lines[index++] = QLine(i - halfLen, j, i + halfLen, j);
            lines[index++] = QLine(i, j - halfLen, i, j + halfLen);
        }
    }

    QPen pen(QBrush(QColor("#E4E3E4")), 2.0f);
    //pen.setStyle(Qt::PenStyle::DashLine);
    //painter->fillRect(rect, backgroundBrush());
    painter->setPen(pen);
    painter->drawLines(lines.data(), lines.size());
}

void view::SceneGraph::drawQuadsBackground(QPainter *painter, const QRectF &rect) const
{
    const int left = floor(rect.left());
    const int right = ceil(rect.right());
    const int top = floor(rect.top());
    const int bottom = ceil(rect.bottom());
    painter->fillRect(rect, backgroundBrush());
    // cells
    {
        QList<QRect> _rectList;

        const int firstLeft = left - (left % _cellSize);
        const int firstTop = top - (top % _cellSize);

        for (int i = firstLeft; i < right; i += _cellSize*2) {
            for (int j = firstTop; j < bottom; j += _cellSize) {
                _rectList << QRect(i, j, _cellSize, _cellSize);
            }
        }

        QBrush brush(_lightGrid);
        QPen pen(QBrush(_lightGrid), 1.0f);
        //pen.setStyle(Qt::PenStyle::DashLine);
        painter->setPen(pen);
        painter->drawRects(_rectList.toVector());
    }
}

void view::SceneGraph::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    auto menu = _interactionsHandler->createContextMenu(event->scenePos());
    if (!menu->actions().isEmpty()) {
        menu->exec(event->screenPos());
        menu->deleteLater();
    }
    QGraphicsScene::contextMenuEvent(event);
}

void view::SceneGraph::setupUi()
{
    setItemIndexMethod(ItemIndexMethod::BspTreeIndex);
    _cellSize = 80; // in pixels
    _blockSize = 4; // in cells

    //_background = QColor(140, 140, 140);
    _background = QColor("#F4F3F4");
    //_background = QColor("#A0A0A0");
    _lightGrid = QColor(200, 200, 200);
    _darkGrid = QColor(160, 160, 160);
    //*/
    /*
    _background = colors::whiteGrey;
    _lightGrid = colors::lightestGrey;
    _darkGrid = colors::lightGrey;
    //*/

    const int gridSize = 1 << 14;
    const int hgridSize = gridSize >> 1;

    setSceneRect(QRectF(-hgridSize, -hgridSize, gridSize, gridSize));
    setBackgroundBrush(QBrush(_background));

    _connectionEdge = new ConnectionEdge();
    addItem(_connectionEdge);

    connect(_modelGraph, &model::Graph::preparingStartup, [=]() { clearNodes(); });

    _miniMap = new MiniMap();
    addItem(_miniMap);
}

void view::SceneGraph::createEdge()
{
    if (_originSlot && _destinationSlot) {
        auto modelEdge = _modelGraph->connectSlots(dynamic_cast<model::OutputSlot *>(
                                                       _originSlot->getModelSlot()),
                                                   dynamic_cast<model::InputSlot *>(
                                                       _destinationSlot->getModelSlot()));

        if (modelEdge != nullptr) {
            addItem(new Edge(this, modelEdge));
        }
    }
}

void view::SceneGraph::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    return;
    QGraphicsScene::dragEnterEvent(event);
}

void view::SceneGraph::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(getModelGraph()->isRunning())
    {
        event->ignore();
    }
    else
    {
        event->acceptProposedAction();
    }
    //return;
    //QGraphicsScene::dragMoveEvent(event);
}

void view::SceneGraph::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(getModelGraph()->isRunning())
    {
        event->ignore();
    }
    else
    {
        event->acceptProposedAction();
    }
    //return;
    //QGraphicsScene::dragLeaveEvent(event);
}

void view::SceneGraph::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(getModelGraph()->isRunning())
    {
        event->ignore();
    }
    else
    {
        event->acceptProposedAction();
    }
    //return;
    //QGraphicsScene::dropEvent(event);
}

void view::SceneGraph::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        auto slot = dynamic_cast<Slot *>(itemAt(event->scenePos(), QTransform()));
        if (slot != nullptr && !slot->isInput()) {
            _originSlot = slot;
            _connectionEdge->setOrigin(_originSlot);
            _connectionEdge->setDestination(event->scenePos());
        }
    }

    _cursorPosition = event->scenePos();

    QGraphicsScene::mousePressEvent(event);
}

void view::SceneGraph::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //bring to front
    if (event->button()) {
        bringToFront(event->scenePos());
    }

    _cursorPosition = event->scenePos();

    if (_originSlot != nullptr) {
        auto slot = dynamic_cast<Slot *>(itemAt(event->scenePos(), QTransform()));
        if (slot != nullptr) {
            bool eligibleStatus = slot->acceptConnection(_originSlot);
            if (eligibleStatus) {
                _destinationSlot = slot;
                _connectionEdge->setEligible();
            } else {
                _destinationSlot = nullptr;
                _connectionEdge->setNotEligible();
            }
        } else {
            _destinationSlot = nullptr;
            _connectionEdge->setNoCandidateAvaible();
        }
        _connectionEdge->setDestination(_cursorPosition);

        _connectionEdge->update();
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void view::SceneGraph::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    createEdge();
    _destinationSlot = nullptr;
    _originSlot = nullptr;
    _connectionEdge->setOrigin(_originSlot);
    update();
    QGraphicsScene::mouseReleaseEvent(event);
}

void view::SceneGraph::bringToFront(QPointF pos) const
{
    auto node = getNodeAt(pos);

    if (node != nullptr) {
        bringToFront(node);
    }
}

void view::SceneGraph::bringToFront(Node *node) const
{
    auto collidingItemsList = collidingItems(node);
    for (auto collidingItem : collidingItemsList) {
        auto collidingNode = dynamic_cast<Node *>(collidingItem);

        if (collidingNode == nullptr)
            continue;
        if (collidingItem == node)
            continue;

        collidingItem->stackBefore(node);
    }
}

#include <model/PersistenceHandler.h>
#include <QUuid>

view::Node *view::SceneGraph::cloneNode(Node *originalNode)
{
    model::PersistenceHandler handler;
    auto modelNode = getModelGraph()->createNodeFromJSValue(
        originalNode->getModelNode()->saveToJSValue(&handler));
    modelNode->setIdentifier(QUuid::createUuid().toString());
    auto node = createGeometryForModel(modelNode);
    addItem(node);
    return node;
}

view::InteractionsHandler *view::SceneGraph::getInteractionsHandler() const
{
    return _interactionsHandler;
}

void view::SceneGraph::deleteSelectedItems()
{
    auto items = selectedItems();

    // process edges
    for(auto item : items)
    {
        auto edge = dynamic_cast<view::Edge*>(item);
        if(edge != nullptr)
        {
            _interactionsHandler->deleteEdge(edge);
        }
    }

    // process nodes
    for(auto item : items)
    {
        auto node = dynamic_cast<view::Node*>(item);
        if(node != nullptr)
        {
            _interactionsHandler->deleteNode(node);
        }
    }

    update();
}

void view::SceneGraph::duplicateSelectedItems()
{
    qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
}

void view::SceneGraph::renameSelectedNode()
{
    qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
}

QRectF view::SceneGraph::computeBoundingRect(const QList<Node *> &nodes, qreal padding) const
{
    if(nodes.isEmpty()) return QRectF();
    auto firstNode = nodes.at(0);
    auto firstNodePos = firstNode->pos();
    qreal left = firstNodePos.x();
    qreal right = firstNodePos.x() + firstNode->width();
    qreal top = firstNode->y();
    qreal bottom = top + firstNode->height();

    for(auto node : nodes)
    {
        left = std::min(left, node->pos().x());
        right = std::max(right, node->pos().x() + node->width());

        top = std::min(top, node->pos().y());
        bottom = std::max(bottom, node->pos().y() + node->height());
    }

    return QRect(left-padding, top-padding, (right-left)+2*padding, (bottom-top)+2*padding);
}

void view::SceneGraph::setBackgroundType(view::SceneGraph::BackgroundType bgType)
{
    _backgroundType = bgType;
}

int view::SceneGraph::getBackgroundType() const
{
    return _backgroundType;
}

void view::SceneGraph::setEdgeType(view::SceneGraph::EdgeType type)
{
    _edgeType = type;
}

int view::SceneGraph::getEdgeType() const
{
    return _edgeType;
}

view::MiniMap *view::SceneGraph::getMiniMap() const
{
    return _miniMap;
}

void view::SceneGraph::customUpdate()
{
    auto sgw = dynamic_cast<SceneGraphWidget*>(views().first());
    auto customRect = sgw->mapToScene(sgw->viewport()->rect()).boundingRect();
    getMiniMap()->setParentViewport(customRect);
}

QImage view::SceneGraph::takeScreenShotSvg(QString path, QRectF rect)
{
    QRect outputRect(0, 0, rect.width(), rect.height());
    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(rect.width(), rect.height()));
    generator.setViewBox(QRectF(0, 0, rect.width(), rect.width()));
    //generator.setResolution(100);


    QBrush tempBrush(QColor(0, 255, 0, 255));
    auto prevBrush = backgroundBrush();
    setBackgroundBrush(tempBrush);
    _drawBackground = false;

    QPainter painter;
    painter.begin(&generator);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
    painter.setRenderHint(QPainter::RenderHint::LosslessImageRendering);
    painter.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
    render(&painter, outputRect, rect);
    painter.end();
    setBackgroundBrush(prevBrush);
    _drawBackground = true;
    return QImage();
}

#include <QPixmap>

QImage view::SceneGraph::takeScreenShot(QRectF rect, qreal multiplier)
{
    bool miniMapVisibility = getMiniMap()->isVisible();
    getMiniMap()->setVisible(false);
    QBrush tempBrush(QColor(0, 255, 0, 255));
    auto prevBrush = backgroundBrush();
    setBackgroundBrush(tempBrush);
    _drawBackground = false;
    QRect outputRect(0, 0, rect.width()*multiplier, rect.height()*multiplier);
    QImage img(outputRect.width(), outputRect.height(), QImage::Format::Format_RGBA8888);
    img.fill(qRgba(0, 0, 0, 0));
    QPainter painter(&img);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
    painter.setRenderHint(QPainter::RenderHint::LosslessImageRendering);
    painter.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
    render(&painter, outputRect, rect);
    setBackgroundBrush(prevBrush);
    _drawBackground = true;
    getMiniMap()->setVisible(miniMapVisibility);
    return img;
}

QImage view::SceneGraph::takeScreenShot(qreal padding, qreal multiplier)
{
    return takeScreenShot(computeBoundingRect(getNodes(), padding), multiplier);
}

void view::SceneGraph::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);

    // Support for double click focus
    auto node = dynamic_cast<view::Node*>(item);
    if(node != nullptr)
    {
        connect(node, &view::Node::doubleClicked, this, [=]()
                {
                    auto nodes = getNodes();
                    for(auto node : nodes)
                    {
                        //if(node->isSelected())
                        //nodes.removeAll(node);
                    }
                    clearSelection();
                    update();
                    qreal multiplier = 2.0;
                    /*for(auto node : nodes)
                    {
                        QList<Node*> tempList;
                        tempList << node;
                        auto rect = computeBoundingRect(tempList, 20);
                        //takeScreenShotSvg(QString("./%1-%2-%3.svg").arg(getModelGraph()->getName().replace(" ", "_"), node->getModelNode()->getType(), node->getModelNode()->getName()), rect);
                        QString path = QString("./%1-%2-%3.png")
                                           .arg(getModelGraph()->getName().replace(" ", "_"),
                                                node->getModelNode()->getType(),
                                                node->getModelNode()->getName());
                        takeScreenShot(rect, multiplier).save(path);
                        //takeScreenShotSvg(path, rect);
                    }*/

                    takeScreenShot(computeBoundingRect(nodes, 20), multiplier).save(QString("./%1.png")
                                                                                        .arg(getModelGraph()->getName().replace(" ", "_")));
/*
                    takeScreenShotSvg(QString("./%1.svg")
                                          .arg(getModelGraph()->getName().replace(" ", "_")), computeBoundingRect(nodes, 20));
                    */
                    //takeScreenShotSvg(computeBoundingRect(getNodes(), 20), 3);
                    //takeScreenShot().save(QString("./%1.png").arg(getModelGraph()->getName().replace(" ", "_")));
                    //emit nodeDoubleClicked(node);
                });
    }
}



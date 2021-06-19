#include "SceneGraphWidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QTimer>
#include <math.h>
#include "Node.h"

#include <model/Graph.h>
#include <model/Environment.h>
#include <QAction>
#include <QMimeData>

#include <QDataStream>
#include <model/EndpointEntry.h>

#include "InteractionsHandler.h"
#include <QPropertyAnimation>
#include <QJSValueIterator>
#include "../NodeSearchDialog.h"
#include <QTimer>

SceneGraphWidget::SceneGraphWidget(QWidget* parent, view::SceneGraph* sceneGraph) : QGraphicsView(parent), _sceneGraph(sceneGraph)
{
    initUi();
    setScene(_sceneGraph);
    setObjectName(sceneGraph->getModelGraph()->getIdentifier());
    _rubberBand = new QRubberBand(QRubberBand::Shape::Line, this);
    viewport()->grabGesture(Qt::GestureType::PanGesture);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value)
            {
                _sceneGraph->customUpdate();
        }, Qt::ConnectionType::DirectConnection);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, [=](int value)
            {
                _sceneGraph->customUpdate();
            }, Qt::ConnectionType::DirectConnection);
}

SceneGraphWidget::~SceneGraphWidget()
{

}

void SceneGraphWidget::initUi()
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //TODO: test other flags
    //setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::SmartViewportUpdate);

    setRenderHint(QPainter::RenderHint::Antialiasing, true);
    setRenderHint(QPainter::RenderHint::LosslessImageRendering, false);
    setRenderHint(QPainter::RenderHint::TextAntialiasing, true);
    setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);
    setRenderHint(QPainter::RenderHint::VerticalSubpixelPositioning, false);

    setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);

    _zoomInFactor = 1.5f;
    _zoomStep = 1;

    _minZoomLevel = 1;
    _maxZoomLevel = 7;
    _defaultZoomLevel = 2;

    _zoomLevel = _defaultZoomLevel;

    setAcceptDrops(true);   
}

view::SceneGraph* SceneGraphWidget::getSceneGraph() const
{
    return _sceneGraph;
}

void SceneGraphWidget::setupViewport(QWidget *widget)
{
    QGraphicsView::setupViewport(widget);
    _sceneGraph->customUpdate();
}

#include <QGesture>

bool SceneGraphWidget::event(QEvent *event)
{
    if(event->type() == QEvent::Type::Gesture)
    {
        auto gesture = (QGesture*)event;
        if(gesture->gestureType() == Qt::GestureType::PanGesture)
        {
            auto panGesture = dynamic_cast<QPanGesture*>(gesture);
            qDebug() << panGesture;
        }
    }
    return QGraphicsView::event(event);
}

void SceneGraphWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        mouseMiddleButtonPressed(event);
    }
    //else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void SceneGraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    setDragMode(QGraphicsView::DragMode::NoDrag);


    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        mouseMiddleButtonReleased(event);
    }
    //else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void SceneGraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (_sceneGraph == nullptr) return;
    if (dragMode() == DragMode::ScrollHandDrag)
    {
        auto p = (event->pos());

        auto delta = p - _prevPos;
        _prevPos = p;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        _sceneGraph->customUpdate();
    }
    QGraphicsView::mouseMoveEvent(event);
}


void SceneGraphWidget::wheelEvent(QWheelEvent* event)
{
    if (_sceneGraph == nullptr) return;

    // always zoom in/out when ctrl is pressed
    bool ctrlPressed = Qt::KeyboardModifier::ControlModifier & event->modifiers();

    auto itemUnderCursor = _sceneGraph->itemAt(mapToScene(event->position().toPoint()), QTransform());

    if (itemUnderCursor != nullptr && !ctrlPressed)
    {
        auto widget = dynamic_cast<QGraphicsProxyWidget*>(itemUnderCursor);

        if (widget != nullptr)
        {
            auto dx = horizontalScrollBar()->value();
            auto dy = verticalScrollBar()->value();

            QGraphicsView::wheelEvent(event);

            horizontalScrollBar()->setValue(dx);
            verticalScrollBar()->setValue(dy);
        }
    }
    else
    {
        performZoom(event);
    }

    _sceneGraph->customUpdate();
}

void SceneGraphWidget::mouseMiddleButtonPressed(QMouseEvent* event)
{
    _prevPos = (event->pos());
    setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
}

void SceneGraphWidget::mouseMiddleButtonReleased(QMouseEvent*)
{
    setDragMode(QGraphicsView::DragMode::NoDrag);
}

void SceneGraphWidget::performZoom(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        setZoomLevel(_zoomLevel - _zoomStep);
    }
    else
    {
        setZoomLevel(_zoomLevel + _zoomStep);
    }
}

void SceneGraphWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-EndpointEntry"))
    {
        event->acceptProposedAction();
    }
}

void SceneGraphWidget::dropEvent(QDropEvent* event)
{    
    auto mime = event->mimeData();
    auto byteArray = mime->data("application/x-EndpointEntry");
    QDataStream inputStream(&byteArray, QIODevice::ReadOnly);

    unsigned int count = 0;
    inputStream >> count;
    for (unsigned int i = 0; i < count; i++)
    {
        quint64 ptr = 0;
        inputStream >> ptr;
        auto entry = dynamic_cast<model::EndpointEntry*>(reinterpret_cast<QObject*>(ptr));
        if (entry != nullptr)
        {
            auto node = _sceneGraph->getInteractionsHandler()->createEndpointNode(entry);
            node->setPos(mapToScene(event->position().toPoint()));
        }
    }

    event->acceptProposedAction();
}

QPointF SceneGraphWidget::getCenter() const
{
    return mapToScene(viewport()->rect()).boundingRect().center();;
}

void SceneGraphWidget::setCenter(const QPointF& p)
{
    centerOn(p);
    _sceneGraph->customUpdate();
}

void SceneGraphWidget::setCenterAnimated(const QPointF &p, bool resetZoom)
{
    auto propAnimation = new QPropertyAnimation();
    propAnimation->setTargetObject(this);
    propAnimation->setDuration(200);
    propAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::InOutCubic));
    propAnimation->setStartValue(getCenter());
    propAnimation->setEndValue(p);
    propAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    if(resetZoom)
        setZoomLevel(2);

    connect(propAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& v)
            {
                centerOn(v.toPointF());
                _sceneGraph->customUpdate();
            });
}

void SceneGraphWidget::setCenterAnimated(view::Node *nodeGr, bool resetZoom)
{
    setCenterAnimated(nodeGr->pos() + QPointF(nodeGr->width() * 0.5f, nodeGr->height() * 0.5f), resetZoom);
}

float SceneGraphWidget::getZoomLevel() const
{
    return _zoomLevel;
}

void SceneGraphWidget::setZoomLevel(float level)
{
    if (level > _maxZoomLevel) level = _maxZoomLevel;
    else if (level < _minZoomLevel) level = _minZoomLevel;

    float factor = powf(_zoomInFactor, _zoomLevel - level);
    scale(factor, factor);

    _zoomLevel = level;
}

QJSValue SceneGraphWidget::saveToJSValue(model::PersistenceHandler* handler) const
{
    auto sceneValue = handler->createJsValue();

    sceneValue.setProperty("center", handler->createJsValue(getCenter()));
    sceneValue.setProperty("zoom", handler->createJsValue(getZoomLevel()));

    auto nodesList = getSceneGraph()->getNodes();
    auto nodesValue = handler->createJsValue();

    std::for_each(nodesList.begin(), nodesList.end(), [=, &nodesValue](view::Node* node)
                  {
                      auto nodeValue = handler->createJsValue();
                      nodeValue.setProperty("position", handler->createJsValue(node->scenePos()));
                      nodeValue.setProperty("size", handler->createJsValue(QPointF(node->width(), node->height())));
                      nodesValue.setProperty(node->getModelNode()->getIdentifier(), nodeValue);
                  });

    sceneValue.setProperty("nodes", nodesValue);

    return sceneValue;
}

bool SceneGraphWidget::loadFromJSValue(const QJSValue& v)
{
    auto valueToPoint = [](const QJSValue& v)
    {
        return QPointF(v.property("x").toNumber(), v.property("y").toNumber());
    };

    auto center = valueToPoint(v.property("center"));
    setCenter(center);
    float zoom = v.property("zoom").toNumber();
    setZoomLevel(zoom);

    auto nodesValue = v.property("nodes");
    QJSValueIterator it(nodesValue);
    while (it.hasNext())
    {
        it.next();
        auto nodeValue = nodesValue.property(it.name());
        QString nodeId = it.name();
        auto nodePosValue = nodeValue.property("position");
        auto nodeSizeValue = nodeValue.property("size");

        auto modelNode = getSceneGraph()->getModelGraph()->findChild<model::Node*>(nodeId);
        auto node = getSceneGraph()->findbyModel(modelNode);
        if (node != nullptr)
        {
            node->setPos(valueToPoint(nodePosValue));
            auto sz = valueToPoint(nodeSizeValue);
            node->setSize(sz.x(), sz.y());
            node->update();
        }
    }

    return true;
}

void SceneGraphWidget::findNodeDialog() const
{
    auto dlg = new view::NodeSearchDialog(const_cast<SceneGraphWidget*>(this));
    dlg->exec();
}


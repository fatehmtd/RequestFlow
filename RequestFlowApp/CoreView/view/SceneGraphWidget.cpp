#include "SceneGraphWidget.h"
#include "Node.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>
#include <math.h>

#include <QOpenGLWidget>

#include <QAction>
#include <QMimeData>
#include <model/Environment.h>
#include <model/Graph.h>

#include <QDataStream>
#include <model/EndpointEntry.h>

#include "../NodeSearchDialog.h"
#include "InteractionsHandler.h"
#include <QJSValueIterator>
#include <QPropertyAnimation>
#include <QTimer>

#include <QNativeGestureEvent>

SceneGraphWidget::SceneGraphWidget(QWidget* parent,
    view::SceneGraph* sceneGraph)
    : QGraphicsView(parent)
    , model::PersistableEntity(this)
    , _sceneGraph(sceneGraph)
{
    initUi();
    setScene(_sceneGraph);
    setObjectName(sceneGraph->getModelGraph()->getIdentifier());
    createShortcuts();
    _rubberBand = new QRubberBand(QRubberBand::Shape::Line, this);

    connect(
        verticalScrollBar(), &QScrollBar::valueChanged, this,
        [=](int value) { _sceneGraph->customUpdate(); },
        Qt::ConnectionType::DirectConnection);

    connect(
        horizontalScrollBar(), &QScrollBar::valueChanged, this,
        [=](int value) { _sceneGraph->customUpdate(); },
        Qt::ConnectionType::DirectConnection);

    connect(_sceneGraph, &view::SceneGraph::nodeDoubleClicked, this,
        [=](view::Node* node) { setCenterAnimated(node, true); });
}

SceneGraphWidget::~SceneGraphWidget() { }

void SceneGraphWidget::initUi()
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // TODO: test other flags
    setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::BoundingRectViewportUpdate);
    //setOptimizationFlag(QGraphicsView::OptimizationFlag::IndirectPainting);
    //setOptimizationFlag(QGraphicsView::OptimizationFlag::DontAdjustForAntialiasing);

    setRenderHint(QPainter::RenderHint::Antialiasing, true);
    setRenderHint(QPainter::RenderHint::LosslessImageRendering, true);
    setRenderHint(QPainter::RenderHint::TextAntialiasing, true);
    setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);
    // setRenderHint(QPainter::RenderHint::VerticalSubpixelPositioning, false);

    setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);

    //setViewport(new QOpenGLWidget());

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

void SceneGraphWidget::setupViewport(QWidget* widget)
{
    QGraphicsView::setupViewport(widget);
    _sceneGraph->customUpdate();
}

bool SceneGraphWidget::event(QEvent* ev)
{
    switch (ev->type()) {
    case QEvent::NativeGesture: {
        auto nge = dynamic_cast<QNativeGestureEvent*>(ev);
        if (nativeGestureEvent(nge))
            return true;
    } break;
    default:
        break;
    }
    return QGraphicsView::event(ev);
}

void SceneGraphWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    if (event->button() == Qt::MouseButton::MiddleButton) {
        mouseMiddleButtonPressed(event);
    }
    // else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void SceneGraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    setDragMode(QGraphicsView::DragMode::NoDrag);

    if (event->button() == Qt::MouseButton::MiddleButton) {
        mouseMiddleButtonReleased(event);
    }
    // else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void SceneGraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (_sceneGraph == nullptr)
        return;
    if (dragMode() == DragMode::ScrollHandDrag) {
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
    if (_sceneGraph == nullptr)
        return;

    // always zoom in/out when ctrl is pressed
    bool ctrlPressed = Qt::KeyboardModifier::ControlModifier & event->modifiers();

    auto itemUnderCursor = _sceneGraph->itemAt(
        mapToScene(event->position().toPoint()), QTransform());

    if (itemUnderCursor != nullptr) {
        //qDebug() << __FUNCTION__ << itemUnderCursor;
    }
    if (ctrlPressed) {

        /*
        if (itemUnderCursor != nullptr && !ctrlPressed) {
            auto widget = dynamic_cast<QGraphicsProxyWidget*>(itemUnderCursor);

            if (widget != nullptr) {
                auto dx = horizontalScrollBar()->value();
                auto dy = verticalScrollBar()->value();

                QGraphicsView::wheelEvent(event);

                horizontalScrollBar()->setValue(dx);
                verticalScrollBar()->setValue(dy);
            }
        } else {
            performZoom(event);
        }//*/

        performZoom(event);
    }

    else {
        QGraphicsView::wheelEvent(event);
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

bool SceneGraphWidget::nativeGestureEvent(QNativeGestureEvent* nativeGestureEvent)
{
    if (nativeGestureEvent != nullptr) {
        if (nativeGestureEvent->gestureType() == Qt::ZoomNativeGesture) {
            auto zvalue = nativeGestureEvent->value();
            auto lvl = zvalue < 0 ? _zoomStep : -_zoomStep;
            setZoomLevel(getZoomLevel() + lvl);
            return true;
        }
    }
    return false;
}

void SceneGraphWidget::performZoom(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        setZoomLevel(_zoomLevel - _zoomStep);
    } else {
        setZoomLevel(_zoomLevel + _zoomStep);
    }
}

void SceneGraphWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-EndpointEntry")) {
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
    for (unsigned int i = 0; i < count; i++) {
        quint64 ptr = 0;
        inputStream >> ptr;
        auto entry = dynamic_cast<model::EndpointEntry*>(reinterpret_cast<QObject*>(ptr));
        if (entry != nullptr) {
            auto node = _sceneGraph->getInteractionsHandler()->createEndpointNode(entry);
            node->setPos(mapToScene(event->pos()));
        }
    }

    event->acceptProposedAction();
}

void SceneGraphWidget::createShortcuts()
{
    auto deleteAction = new QAction(QIcon(), "delete", this);
    deleteAction->setShortcutContext(
        Qt::ShortcutContext::WidgetWithChildrenShortcut);
    deleteAction->setShortcut(QKeySequence(QKeySequence::StandardKey::Delete));
    connect(deleteAction, &QAction::triggered, this,
        [=]() { _sceneGraph->deleteSelectedItems(); });
    addAction(deleteAction);

    auto duplicateAction = new QAction(QIcon(), "duplicate", this);
    duplicateAction->setShortcutContext(
        Qt::ShortcutContext::WidgetWithChildrenShortcut);
    duplicateAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(duplicateAction, &QAction::triggered, this,
        [=]() { _sceneGraph->duplicateSelectedItems(); });
    addAction(duplicateAction);

    auto renameAction = new QAction(QIcon(), "rename", this);
    renameAction->setShortcutContext(
        Qt::ShortcutContext::WidgetWithChildrenShortcut);
    renameAction->setShortcut(QKeySequence(Qt::Key::Key_F2));
    connect(renameAction, &QAction::triggered, this,
        [=]() { _sceneGraph->renameSelectedNode(); });
    addAction(renameAction);
}

QPointF SceneGraphWidget::getCenter() const
{
    return mapToScene(viewport()->rect()).boundingRect().center();
}

void SceneGraphWidget::setCenter(const QPointF& p)
{
    centerOn(p);
    _sceneGraph->customUpdate();
}

void SceneGraphWidget::setCenterAnimated(const QPointF& p, bool resetZoom)
{
    auto propAnimation = new QPropertyAnimation();
    propAnimation->setTargetObject(this);
    propAnimation->setDuration(400);
    // propAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::InOutCubic));
    propAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::OutCubic));
    propAnimation->setStartValue(getCenter());
    propAnimation->setEndValue(p);
    propAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    if (resetZoom)
        setZoomLevel(2);

    connect(propAnimation, &QPropertyAnimation::valueChanged, this,
        [=](const QVariant& v) {
            centerOn(v.toPointF());
            _sceneGraph->customUpdate();
        });
}

void SceneGraphWidget::setCenterAnimated(view::Node* nodeGr, bool resetZoom)
{
    setCenterAnimated(
        nodeGr->pos() + QPointF(nodeGr->width() * 0.5f, nodeGr->height() * 0.5f),
        resetZoom);
}

float SceneGraphWidget::getZoomLevel() const { return _zoomLevel; }

void SceneGraphWidget::setZoomLevel(float level)
{
    if (level > _maxZoomLevel)
        level = _maxZoomLevel;
    else if (level < _minZoomLevel)
        level = _minZoomLevel;

    float factor = powf(_zoomInFactor, _zoomLevel - level);
    scale(factor, factor);

    _zoomLevel = level;
}

QJSValue
SceneGraphWidget::saveToJSValue(model::PersistenceHandler* handler) const
{
    auto sceneValue = handler->createJsValue();

    sceneValue.setProperty("center", handler->createJsValue(getCenter()));
    sceneValue.setProperty("zoom", handler->createJsValue(getZoomLevel()));

    auto nodesList = getSceneGraph()->getNodes();

    saveChildren(sceneValue, handler, "nodes", nodesList);

    // Old code for persisting nodes geometries
    /*
     auto nodesValue = handler->createJsValue();
      std::for_each(nodesList.begin(), nodesList.end(), [=,
     &nodesValue](view::Node* node)
                    {
                        auto nodeValue = handler->createJsValue();
                        nodeValue.setProperty("position",
     handler->createJsValue(node->scenePos())); nodeValue.setProperty("size",
     handler->createJsValue(QPointF(node->width(), node->height())));
                        nodesValue.setProperty(node->getModelNode()->getIdentifier(),
     nodeValue);
                    });

      sceneValue.setProperty("nodes", nodesValue);
  */
    return sceneValue;
}

bool SceneGraphWidget::loadFromJSValue(const QJSValue& v)
{
    auto valueToPoint = [](const QJSValue& v) {
        return QPointF(v.property("x").toNumber(), v.property("y").toNumber());
    };

    auto center = valueToPoint(v.property("center"));
    setCenter(center);
    float zoom = v.property("zoom").toNumber();
    setZoomLevel(zoom);

    loadChildren(v, "nodes", [=](const QJSValue& value) {
        auto nodeId = value.property("identifier").toString();
        auto modelNode = getSceneGraph()->getModelGraph()->findChild<model::Node*>(nodeId);
        if (modelNode != nullptr) {
            auto node = getSceneGraph()->findbyModel(modelNode);
            node->loadFromJSValue(value);
        }
    });

    // Old code for loading nodes geometries
    /*
      auto nodesValue = v.property("nodes");
      QJSValueIterator it(nodesValue);
      while (it.hasNext())
      {
          it.next();
          auto nodeValue = nodesValue.property(it.name());
          QString nodeId = it.name();
          auto nodePosValue = nodeValue.property("position");
          auto nodeSizeValue = nodeValue.property("size");

          auto modelNode =
     getSceneGraph()->getModelGraph()->findChild<model::Node*>(nodeId); auto
     node = getSceneGraph()->findbyModel(modelNode); if (node != nullptr)
          {
              node->setPos(valueToPoint(nodePosValue));
              auto sz = valueToPoint(nodeSizeValue);
              node->setSize(sz.x(), sz.y());
              node->update();
          }
      }*/

    return true;
}

void SceneGraphWidget::findNodeDialog() const
{
    auto dlg = new view::NodeSearchDialog(const_cast<SceneGraphWidget*>(this));
    dlg->exec();
}

void SceneGraphWidget::centerOnScene()
{
    auto childrenList = _sceneGraph->getNodes();

    if (childrenList.isEmpty())
        return;

    auto node = childrenList.first();
    auto p = node->scenePos();

    QRectF rect = childrenList.first()->boundingRect().adjusted(p.x(), p.y(), p.x(), p.y());
    std::for_each(childrenList.begin(), childrenList.end(), [&rect](view::Node* node) {
        auto p = node->scenePos();
        rect = rect.united(node->boundingRect().adjusted(p.x(), p.y(), p.x(), p.y()));
    });

    setCenterAnimated(rect.center());
}

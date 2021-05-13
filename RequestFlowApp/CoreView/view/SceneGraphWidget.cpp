#include "SceneGraphWidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QOpenGLWidget>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QTimer>
#include <math.h>
#include "Node.h" 

#include <model/Graph.h>
#include <model/Environment.h>

SceneGraphWidget::SceneGraphWidget(QWidget* parent, view::SceneGraph* sceneGraph) : QGraphicsView(parent), _sceneGraph(sceneGraph)
{
	initUi();
	setScene(_sceneGraph);
	setObjectName(sceneGraph->getModelGraph()->getIdentifier());
	_rubberBand = new QRubberBand(QRubberBand::Shape::Rectangle, this);
}

SceneGraphWidget::~SceneGraphWidget()
{

}

view::SceneGraph* SceneGraphWidget::getSceneGraph() const
{
	return _sceneGraph;
}

#include <QTimer>

void SceneGraphWidget::initUi()
{
	//setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::SmartViewportUpdate);

    setRenderHint(QPainter::RenderHint::Antialiasing, true);
    setRenderHint(QPainter::RenderHint::LosslessImageRendering, false);
    setRenderHint(QPainter::RenderHint::TextAntialiasing, true);
    setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);

	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);

    /*
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    setOptimizationFlags(QGraphicsView::OptimizationFlag::DontClipPainter);
    setCacheMode(QGraphicsView::CacheModeFlag::CacheNone);*/

	_zoomInFactor = 1.5f;
	_zoomStep = 1;

	_minZoomLevel = 1;
    _maxZoomLevel = 5;
    _defaultZoomLevel = 2;

	_zoomLevel = _defaultZoomLevel;

    //setOGLBackend();
	setAcceptDrops(true);
}

void SceneGraphWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MouseButton::LeftButton)
	{
		setDragMode(QGraphicsView::RubberBandDrag);
	}

	// TODO: use this as the center of the view
	//qDebug() << mapToScene(viewport()->rect()).boundingRect().center();
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

		auto items = _sceneGraph->items(viewport()->rect());

		for (auto item : items)
		{
			auto node = dynamic_cast<QGraphicsProxyWidget*>(item);
			if (node == nullptr) continue;
			node->widget()->repaint();
		}
	}
	QGraphicsView::mouseMoveEvent(event);
}


void SceneGraphWidget::wheelEvent(QWheelEvent* event)
{
	if (_sceneGraph == nullptr) return;

	// always zoom in/out when ctrl is pressed
	bool ctrlPressed = Qt::KeyboardModifier::ControlModifier & event->modifiers();

	auto itemUnderCursor = _sceneGraph->itemAt(mapToScene(event->pos()), QTransform());

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

	auto items = _sceneGraph->items(viewport()->rect());

	for (auto item : items)
	{
		auto node = dynamic_cast<QGraphicsProxyWidget*>(item);
		if (node == nullptr) continue;
		node->widget()->repaint();
	}
}

void SceneGraphWidget::mouseMiddleButtonPressed(QMouseEvent* event)
{
	_prevPos = (event->pos());
	setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
}

void SceneGraphWidget::mouseMiddleButtonReleased(QMouseEvent* event)
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

void SceneGraphWidget::setOGLBackend()
{
	QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(8);
	//surfaceFormat.setVersion(4, 2);
	//surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
    surfaceFormat.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
    surfaceFormat.setSwapInterval(1);

	//QSurfaceFormat::setDefaultFormat(surfaceFormat);
    surfaceFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::TripleBuffer);

	auto glWidget = new QOpenGLWidget();
    glWidget->setFormat(surfaceFormat);
	setViewport(glWidget);
	//*/
}

#include <QMimeData>

void SceneGraphWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("application/x-EndpointEntry"))
	{
		event->acceptProposedAction();
	}
}

#include <QDataStream>
#include <model/EndpointEntry.h>

#include "InteractionsHandler.h"

void SceneGraphWidget::dropEvent(QDropEvent* event)
{
	auto mime = event->mimeData();
	auto byteArray = mime->data("application/x-EndpointEntry");
	QDataStream in(&byteArray, QIODevice::ReadOnly);

	int count = 0;
	in >> count;
	for (int i = 0; i < count; i++)
	{
        quint64 ptr = 0;
		in >> ptr;
		auto entry = dynamic_cast<model::EndpointEntry*>(reinterpret_cast<QObject*>(ptr));
		if (entry != nullptr)
		{
			auto node = _sceneGraph->getInteractionsHandler()->createEndpointNode(entry);			
			node->setPos(mapToScene(event->pos()));
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

	for (auto node : nodesList)
	{
		auto nodeValue = handler->createJsValue();
		auto pos = node->scenePos();
		nodeValue.setProperty("position", handler->createJsValue(node->scenePos()));
		nodeValue.setProperty("size", handler->createJsValue(QPointF(node->width(), node->height())));
		nodesValue.setProperty(node->getModelNode()->getIdentifier(), nodeValue);
	}

	sceneValue.setProperty("nodes", nodesValue);

	return sceneValue;
}

#include <QJSValueIterator>

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


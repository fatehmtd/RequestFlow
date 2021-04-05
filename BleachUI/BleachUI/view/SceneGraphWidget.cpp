#include "SceneGraphWidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QOpenGLWidget>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QApplication>

#include "Node.h" 

#include <model/Graph.h>
#include <model/Environment.h>

SceneGraphWidget::SceneGraphWidget(QWidget* parent) : QGraphicsView(parent)
{
	initUi();
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

	setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
	setRenderHint(QPainter::RenderHint::Antialiasing, true);
	setRenderHint(QPainter::RenderHint::TextAntialiasing, true);
	setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);

	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);

	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
	setCacheMode(QGraphicsView::CacheModeFlag::CacheNone);
	//*/
	/*
	_sceneGraph = new view::SceneGraph(new model::Graph, this);
	auto env = new model::Environment(_sceneGraph->getModelGraph());
	env->setName("Default environment");
	env->getEntries().insert("baseUrl", "https://jsonplaceholder.typicode.com");
	_sceneGraph->getModelGraph()->setActiveEnvironment(env);	
	setScene(_sceneGraph);
	*/

	_zoomInFactor = 1.5f;
	_zoomStep = 1;
	_zoomLevel = 2;
	_minZoomLevel = 1;
	_maxZoomLevel = 5;

	//setOGLBackend();	
}

void SceneGraphWidget::mousePressEvent(QMouseEvent* event)
{
	qDebug() << mapToScene(viewport()->rect()).boundingRect().center();
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
		if (_zoomLevel > _minZoomLevel)
		{
			_zoomLevel -= _zoomStep;
			scale(_zoomInFactor, _zoomInFactor);
		}
	}
	else
	{
		if (_zoomLevel < _maxZoomLevel)
		{
			_zoomLevel += _zoomStep;
			const float factor = 1.0f / _zoomInFactor;
			scale(factor, factor);
		}
	}
}

void SceneGraphWidget::setOGLBackend()
{
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setSamples(8);
	//surfaceFormat.setVersion(4, 2);
	//surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
	//surfaceFormat.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
	//surfaceFormat.setSwapInterval(1);

	//QSurfaceFormat::setDefaultFormat(surfaceFormat);
	//surfaceFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::SingleBuffer);

	auto glWidget = new QOpenGLWidget();
	glWidget->setFormat(surfaceFormat);
	setViewport(glWidget);
	//*/
}

void SceneGraphWidget::paintEvent(QPaintEvent* paintEvent)
{
	auto newEvent = new QPaintEvent(paintEvent->region().boundingRect());
	QGraphicsView::paintEvent(newEvent);
	delete newEvent;
}

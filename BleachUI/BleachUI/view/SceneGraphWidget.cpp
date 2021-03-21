#include "SceneGraphWidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QOpenGLWidget>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QApplication>

#include <model/Graph.h>

SceneGraphWidget::SceneGraphWidget(QWidget* parent) : QGraphicsView(parent)
{
	initUi();
}

SceneGraphWidget::~SceneGraphWidget()
{

}

#include <QTimer>

void SceneGraphWidget::initUi()
{
	//setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
	setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
	setRenderHint(QPainter::RenderHint::Antialiasing, false);
	setRenderHint(QPainter::RenderHint::TextAntialiasing, false);
	setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, false);

	/*
	setRenderHints(
		QPainter::RenderHint::Antialiasing |
		QPainter::RenderHint::TextAntialiasing |
		QPainter::RenderHint::SmoothPixmapTransform
	);
	//*/
	
	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
	/*
	setOptimizationFlags(QGraphicsView::DontClipPainter);
	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
	setCacheMode(QGraphicsView::CacheModeFlag::CacheNone);
	//*/

	_sceneGraph = new view::SceneGraph(new model::Graph, this);
	setScene(_sceneGraph);

	_zoomInFactor = 1.25f;
	_zoomStep = 1;
	_zoomLevel = 4;
	_minZoomLevel = 1;
	_maxZoomLevel = 10;

	/*
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setSamples(8);
	surfaceFormat.setVersion(4, 2);
	//surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
	//surfaceFormat.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
	surfaceFormat.setSwapInterval(1);

	QSurfaceFormat::setDefaultFormat(surfaceFormat);
	//surfaceFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::SingleBuffer);

	auto glWidget = new QOpenGLWidget();
	//glWidget->setFormat(surfaceFormat);
	setViewport(glWidget);
	//*/
}

void SceneGraphWidget::mousePressEvent(QMouseEvent* event)
{
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
	if (dragMode() == DragMode::ScrollHandDrag)
	{
		auto p = (event->pos());

		auto delta = p - _prevPos;
		_prevPos = p;

		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
		verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
	}
	QGraphicsView::mouseMoveEvent(event);
}

void SceneGraphWidget::wheelEvent(QWheelEvent* event)
{
	// always zoom in/out when ctrl is pressed
	bool ctrlPressed = Qt::KeyboardModifier::ControlModifier& event->modifiers();

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
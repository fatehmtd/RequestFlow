#include "SceneGraphWidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QOpenGLWidget>

SceneGraphWidget::SceneGraphWidget(QWidget* parent) : QGraphicsView(parent)
{
	initUi();
}

SceneGraphWidget::~SceneGraphWidget()
{

}

void SceneGraphWidget::initUi()
{
	//setFocusPolicy(Qt::FocusPolicy::ClickFocus);
 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
	
	setRenderHints(
		//QPainter::RenderHint::Antialiasing | 
		//QPainter::RenderHint::HighQualityAntialiasing | 
		QPainter::RenderHint::TextAntialiasing |
		QPainter::RenderHint::SmoothPixmapTransform
	);
	//*/
	//setCacheMode(QGraphicsView::CacheModeFlag::CacheNone);
	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);

	setOptimizationFlags(QGraphicsView::DontClipPainter);
	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
	setCacheMode(QGraphicsView::CacheBackground);
	
	_sceneGraph = new view::SceneGraph(this);
	setScene(_sceneGraph);

	_zoomInFactor = 1.25f;
	_zoomStep = 1;
	_zoomLevel = 5;
	_minZoomLevel = 1;
	_maxZoomLevel = 10;

	/*
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setSamples(8);
	surfaceFormat.setSwapInterval(60);
	surfaceFormat.setRedBufferSize(16);
	surfaceFormat.setGreenBufferSize(16);
	surfaceFormat.setBlueBufferSize(16);
	surfaceFormat.setAlphaBufferSize(16);
	auto glWidget = new QOpenGLWidget();
	glWidget->setFormat(surfaceFormat);
	setViewport(glWidget);
	//*/
}

void SceneGraphWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MouseButton::MiddleButton)
	{
		mouseMiddleButtonPressed(event);
	}
	else
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
	else
	{
		QGraphicsView::mouseReleaseEvent(event);
	}
}

void SceneGraphWidget::wheelEvent(QWheelEvent* event)
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
	// TODO: disable zooming whilst hovering over a widget
	//QGraphicsView::wheelEvent(event);
}

void SceneGraphWidget::mouseMiddleButtonPressed(QMouseEvent* event)
{
	auto releaseEvent = QMouseEvent(QEvent::MouseButtonRelease, event->localPos(), Qt::MouseButton::MiddleButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);
	QGraphicsView::mouseReleaseEvent(&releaseEvent);

	setDragMode(QGraphicsView::DragMode::ScrollHandDrag);

	auto pressEvent = QMouseEvent(event->type(), event->localPos(), Qt::MouseButton::LeftButton, event->buttons() | Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);
	QGraphicsView::mousePressEvent(&pressEvent);
}

void SceneGraphWidget::mouseMiddleButtonReleased(QMouseEvent* event)
{
	auto pressEvent = QMouseEvent(event->type(), event->localPos(), Qt::MouseButton::LeftButton, event->buttons() & ~Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);
	QGraphicsView::mousePressEvent(&pressEvent);

	setDragMode(QGraphicsView::DragMode::NoDrag);
}

void SceneGraphWidget::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
}

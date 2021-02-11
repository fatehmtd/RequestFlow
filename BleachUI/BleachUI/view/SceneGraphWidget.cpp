#include "SceneGraphWidget.h"
#include <QVBoxLayout>
#include <QDebug>

SceneGraphWidget::SceneGraphWidget(QWidget* parent) : QGraphicsView(parent)
{
	initUi();
}

SceneGraphWidget::~SceneGraphWidget()
{

}

void SceneGraphWidget::initUi()
{
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
 	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
	setRenderHints(QPainter::RenderHint::Antialiasing | 
		QPainter::RenderHint::HighQualityAntialiasing | 
		QPainter::RenderHint::TextAntialiasing |
		QPainter::RenderHint::SmoothPixmapTransform);

	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);

	_sceneGraph = new view::SceneGraph(this);
	setScene(_sceneGraph);

	_zoomInFactor = 1.125f;
	_zoomStep = 1;
	_zoomLevel = 3;
	_minZoomLevel = 1;
	_maxZoomLevel = 10;
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

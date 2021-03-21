#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include "SceneGraph.h"

class SceneGraphWidget : public QGraphicsView
{
public:
	SceneGraphWidget(QWidget* parent = nullptr);
	~SceneGraphWidget();
protected:
	void initUi();

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;

	virtual void mouseMiddleButtonPressed(QMouseEvent* event);
	virtual void mouseMiddleButtonReleased(QMouseEvent* event);

	void performZoom(QWheelEvent* event);

protected:
	view::SceneGraph* _sceneGraph = nullptr;

	bool _isDragging = false;
	QPointF _prevPos;
	QPointF _position;

	// Zooming
	float _zoomInFactor;
	float _zoomStep, _zoomLevel;
	float _minZoomLevel, _maxZoomLevel;
};


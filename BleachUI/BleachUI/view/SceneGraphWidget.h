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

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	void mouseMiddleButtonPressed(QMouseEvent* event);
	void mouseMiddleButtonReleased(QMouseEvent* event);

protected:
	view::SceneGraph* _sceneGraph = nullptr;

	// Zooming
	float _zoomInFactor;
	float _zoomStep, _zoomLevel;
	float _minZoomLevel, _maxZoomLevel;
};


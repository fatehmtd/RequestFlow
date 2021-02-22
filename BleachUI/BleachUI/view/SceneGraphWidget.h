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
	virtual void wheelEvent(QWheelEvent* event) override;

	virtual void mouseMiddleButtonPressed(QMouseEvent* event);
	virtual void mouseMiddleButtonReleased(QMouseEvent* event);

	virtual void scrollContentsBy(int dx, int dy) override;

protected:
	view::SceneGraph* _sceneGraph = nullptr;

	// Zooming
	float _zoomInFactor;
	float _zoomStep, _zoomLevel;
	float _minZoomLevel, _maxZoomLevel;
};


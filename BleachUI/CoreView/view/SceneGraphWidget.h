#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include "SceneGraph.h"
#include <QTimer>
#include "SceneGraph.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QRubberBand>

class SceneGraphWidget : public QGraphicsView
{
public:
	SceneGraphWidget(QWidget* parent, view::SceneGraph* sceneGraph);
	~SceneGraphWidget();

	view::SceneGraph* getSceneGraph() const;

	QPointF getCenter() const;
	void setCenter(const QPointF& p);

	float getZoomLevel() const;
	void setZoomLevel(float level);

	QJSValue saveToJSValue(model::PersistenceHandler* handler) const;
	bool loadFromJSValue(const QJSValue& v);

protected:
	void initUi();

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;

	virtual void mouseMiddleButtonPressed(QMouseEvent* event);
	virtual void mouseMiddleButtonReleased(QMouseEvent* event);

	void performZoom(QWheelEvent* event);
	void setOGLBackend();

	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
protected:
	view::SceneGraph* _sceneGraph = nullptr;

	bool _isDragging = false;
	QPointF _prevPos;
	QPointF _position;

	int _timer = 0;

	// Zooming
	float _zoomInFactor;
	float _zoomStep, _zoomLevel;
	float _minZoomLevel, _maxZoomLevel, _defaultZoomLevel;
	QRubberBand* _rubberBand = nullptr;
	QPoint _selectionOrigin;
};
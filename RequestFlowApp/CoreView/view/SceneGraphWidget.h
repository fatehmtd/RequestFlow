#pragma once
#include "coreview_global.h"

#include "SceneGraph.h"
#include <model/PersistableEntity.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRubberBand>
#include <QTimer>

class COREVIEW_EXPORT SceneGraphWidget : public QGraphicsView, public model::PersistableEntity {
public:
    SceneGraphWidget(QWidget* parent, view::SceneGraph* sceneGraph);
    ~SceneGraphWidget();

    view::SceneGraph* getSceneGraph() const;

    QPointF getCenter() const;
    void setCenter(const QPointF& p);

    void setCenterAnimated(const QPointF& p, bool resetZoom = true);
    void setCenterAnimated(view::Node* node, bool resetZoom = true);

    void centerOnScene();

    float getZoomLevel() const;
    void setZoomLevel(float level);

    QJSValue saveToJSValue(model::PersistenceHandler* handler) const override;
    bool loadFromJSValue(const QJSValue& v) override;

    void findNodeDialog() const;

protected:
    void initUi();

    void setupViewport(QWidget* widget) override;

    virtual bool event(QEvent* ev) override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    virtual void mouseMiddleButtonPressed(QMouseEvent* event);
    virtual void mouseMiddleButtonReleased(QMouseEvent* event);

    virtual bool nativeGestureEvent(QNativeGestureEvent* event);

    void performZoom(QWheelEvent* event);

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    void createShortcuts();

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

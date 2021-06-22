#pragma once
#include "coreview_global.h"

#include <QGraphicsSvgItem>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsObject>
#include <QGraphicsWidget>
#include <QGraphicsPixmapItem>
#include <QIcon>
#include <QPixmap>
#include <QObject>
#include <QJSValue>
#include <QJSEngine>
#include <model/Node.h>
#include "ContentWidget.h"
#include "Colors.h"

#include "SceneGraph.h"

namespace modelr
{
	class PersistenceHandler;
}

namespace view
{
	class Slot;
	class SceneGraph;

	class COREVIEW_EXPORT Node : public QGraphicsObject
	{
		Q_OBJECT
	public:
		Node(model::Node* modelNode, QString nodeType);
		~Node();

		int width() const;
		int height() const;

		int getSlotIndex(const Slot* slot) const;
		ContentWidget* getContentWidget() const;

		model::Node* getModelNode() const;

		template <class T>
		T getModelNode() const
		{
			return dynamic_cast<T>(getModelNode());
		}

		Slot* getSlot(const QString& name) const;

		// Inherited via QGraphicsItem
		virtual QRectF boundingRect() const override;

		void setTitle(const QString& txt);
		QString getTitle() const;

		void setSize(int w, int h);

		float getSlotHeight() const;
		float getHeaderHeight() const;
		float getSlotsSectionHeight() const;
		float getContentHeight() const;

		void setMinSize(QSize size);
        void setMinSize(int w, int h);
		QSize getMinSize() const;

        void setMaxSize(QSize size);
        void setMaxSize(int w, int h);
        QSize getMaxSize() const;

        void setSvgIcon(QString path);

		virtual void clearUI();

		bool isResizable() const;
		void setResizable(bool status);

		view::SceneGraph* getSceneGraph() const;

		QString getNodeType() const;

        QColor getBackgroundColor() const;

	private slots:
		void onGraphStarted();
        void onGraphFinished();

    signals:
        void doubleClicked();

	protected:
		enum Handle
		{
			LEFT = 1,
			RIGHT = 2,
			TOP = 4,
			BOTTOM = 8,
			TOP_LEFT = 5,
			TOP_RIGHT = 6,
			BOTTOM_LEFT = 9,
			BOTTOM_RIGHT = 10
		};

		virtual void setupUi();
		virtual void setupContentWidget();
		virtual void setupUIForModel();
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event)	 override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)	 override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)	 override;

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

		void doResize(QGraphicsSceneMouseEvent* event);
	private:
		int computeGripCorner(const QPointF& cursorPos);
		void handleResize(const QPointF& pos);

        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	protected:
		model::Node* _node = nullptr;
		QGraphicsTextItem* _title = nullptr;
        QGraphicsSvgItem* _icon = nullptr;
		ContentWidget* _contentWidget = nullptr;
		QSize _size;
		int _padding;
		int _edgeSize;
		QPointF _mouseShift;
		QColor _bgColor;
		bool _mouseHover = false;

		// resizing
		QPointF _anchorPoint;
		int _cursorResizeMode = false;
		bool _resizeEligible = false;
		bool _isResizing = false;
		QPointF _topLeftCorner, _bottomRightCorner;
		bool _firstTimeResize = true;
		QSize _minSize;
        QSize _maxSize;
		bool _isResizable = true;
		QPixmap _pixmap;
		bool _painted = false;
	private:
		QString _nodeType;
	};
}


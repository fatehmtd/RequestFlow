#pragma once

#include <QGraphicsScene>

namespace view
{
	class Node;
	class Link;
	class Slot;

	class SceneGraph : public QGraphicsScene
	{
	public:
		SceneGraph(QObject* parent = nullptr);
	protected:
		QColor _background;
		QColor _lightGrid, _darkGrid;
		ushort _cellSize, _blockSize;
		float _zoomLevel;

	protected:
		virtual void drawBackground(QPainter* painter, const QRectF& rect) override;

		void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

		virtual void setupUi();
	};
}
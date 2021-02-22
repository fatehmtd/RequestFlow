#include "Node.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "Colors.h"
#include "Slot.h"
#include <QGraphicsProxyWidget>

#include <model/Node.h>
#include <model/Slot.h>

view::Node::Node(model::Node* modelNode) : _node(modelNode)
{
	_bgColor = colors::light::purple;
	setupUi();
	setupUIForModel();
	setupContentWidget();
}

view::Node::~Node()
{
}

int view::Node::width() const
{
	return _width;
}

int view::Node::height() const
{
	return _height;
}

int view::Node::getSlotIndex(const Slot* slot) const
{
	const auto allChildren = childItems();
	int index = 0;
	for (auto child : allChildren)
	{
		auto childSlot = dynamic_cast<Slot*>(child);
		if (childSlot == nullptr) continue;
		if (childSlot->isInput() == slot->isInput())
		{
			if (childSlot == slot) break;
			++index;
		}
	}
	return index;
}

view::ContentWidget* view::Node::getContentWidget() const
{
	return _contentWidget;
}

model::Node* view::Node::getModelNode() const
{
	return _node;
}

view::Slot* view::Node::getSlot(const QString& name) const
{
	return findChild<Slot*>(name);
}

void view::Node::setupUi()
{
	_width = 256 + rand() % 32;
	_height = 128 + rand() % 64;
	_edgeSize = 5;

	setAcceptHoverEvents(true);

	setFlag(GraphicsItemFlag::ItemIsSelectable);
	setFlag(GraphicsItemFlag::ItemIsMovable);

	_title = new QGraphicsTextItem(this);
	_title->setPlainText(QString("Node name").arg(rand() % 100));
	_title->setDefaultTextColor(Qt::white);
}

void view::Node::setupContentWidget()
{
	////////////////////////////
	auto proxyWidget = new QGraphicsProxyWidget(this);
	_contentWidget = new ContentWidget(nullptr);
	proxyWidget->setWidget(_contentWidget);

	int top = 60;
	int bottom = _height - (top + 20);
	_contentWidget->setGeometry(_edgeSize, top, _width - 2 * _edgeSize, bottom);
}

void view::Node::setupUIForModel()
{
	if (_node != nullptr)
	{
		auto inputSlots = _node->getInputSlots();
		for (auto slot : inputSlots.values())
		{
			auto inputSlot = new view::Slot(this, slot);
		}

		auto outputSlots = _node->getOutputSlots();
		for (auto slot : outputSlots.values())
		{
			auto outputSlot = new view::Slot(this, slot);
		}

		_title->setPlainText(_node->objectName());
	}
}

QRectF view::Node::boundingRect() const
{
	auto rect = QRectF(0, 0, _width, _height);
	return rect;
}

void view::Node::setTitle(const QString& txt)
{
	_title->setPlainText(txt);
}

QString view::Node::getTitle() const
{
	return _title->toPlainText();
}

void view::Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto rect = boundingRect();
	int w = rect.width(), h = rect.height();

	// Background
	{
		QPainterPath backgroundPath;
		//auto bgColor = colors::light::purple;
		auto bgColor = QColor("#F0F0F0");
		QPen pen(bgColor);

		QBrush backgroundBrush(bgColor);
		backgroundPath.setFillRule(Qt::FillRule::WindingFill);
		backgroundPath.addRoundedRect(0, 0, w, h, _edgeSize, _edgeSize);
		painter->setPen(pen);
		painter->setBrush(backgroundBrush);
		painter->drawPath(backgroundPath.simplified());
	}

	// Title background
	{
		QPainterPath backgroundPath;
		//auto bgColor = colors::light::yellow;
		auto bgColor = _bgColor;
		QPen pen(bgColor);

		QBrush backgroundBrush(bgColor);
		backgroundPath.setFillRule(Qt::FillRule::WindingFill);
		backgroundPath.addRoundedRect(0, 0, w, 20, _edgeSize, _edgeSize);
		painter->setPen(pen);
		painter->setBrush(backgroundBrush);
		painter->drawPath(backgroundPath.simplified());
	}

	// Outline
	{
		QPainterPath outlinePath;
		outlinePath.addRoundedRect(0, 0, w, h, _edgeSize, _edgeSize);
		QPen pen(isSelected() ? QColor("#FFFFA637") : QColor("#7F000000"), isSelected() ? 2.0f : 1.0f);
		painter->setPen(pen);
		painter->setBrush(Qt::BrushStyle::NoBrush);
		painter->drawPath(outlinePath.simplified());
	}

	// Outline Hover
	if (_mouseHover)
	{
		QPainterPath outlinePath;
		float outlineEdgeSize = _edgeSize * 1.5f;
		float halfEdgeSize = 0.5f * outlineEdgeSize;
		outlinePath.addRoundedRect(-halfEdgeSize, -halfEdgeSize, w + outlineEdgeSize, h + outlineEdgeSize, outlineEdgeSize, outlineEdgeSize);
		QPen pen(QColor("#FFFFA637"), _edgeSize * 0.5f, Qt::PenStyle::DashDotDotLine);
		painter->setPen(pen);
		painter->setBrush(Qt::BrushStyle::NoBrush);
		painter->drawPath(outlinePath.simplified());
	}
}

void view::Node::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsObject::mouseMoveEvent(event);
}

void view::Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsObject::mousePressEvent(event);
}

void view::Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsObject::mouseReleaseEvent(event);
}

void view::Node::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHover = true;
	QGraphicsObject::hoverEnterEvent(event);
}

void view::Node::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	// check if the cursor is on the border and engage resizing
	/*if (!_resizeMode)
	{
		int corner = computeGripCorner(event->scenePos());

		switch (corner)
		{
		case Handle::LEFT:
		case Handle::RIGHT:
			setCursor(Qt::CursorShape::SizeHorCursor);
			break;
		case Handle::TOP:
		case Handle::BOTTOM:
			setCursor(Qt::CursorShape::SizeVerCursor);
			break;
		case Handle::TOP_LEFT:
		case Handle::BOTTOM_RIGHT:
			setCursor(Qt::CursorShape::SizeFDiagCursor);
			break;
		case Handle::BOTTOM_LEFT:
		case Handle::TOP_RIGHT:
			setCursor(Qt::CursorShape::SizeBDiagCursor);
			break;
		default:
			setCursor(Qt::CursorShape::OpenHandCursor);
			break;
		}
	}
	*/
	QGraphicsObject::hoverMoveEvent(event);
}

void view::Node::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHover = false;
	QGraphicsObject::hoverLeaveEvent(event);
}

int view::Node::computeGripCorner(const QPointF& p)
{
	auto relativePos = p - scenePos();
	float thresh = _edgeSize * 2;

	int corner = 0;

	if (relativePos.x() <= thresh)
	{
		corner |= Handle::LEFT;
	}
	else if ((_width - relativePos.x()) <= thresh)
	{
		corner |= Handle::RIGHT;
	}

	if (relativePos.y() <= thresh)
	{
		corner |= Handle::TOP;
	}
	else if ((_height - relativePos.y()) <= thresh)
	{
		corner |= Handle::BOTTOM;
	}

	qDebug() << relativePos << corner;

	return corner;
}

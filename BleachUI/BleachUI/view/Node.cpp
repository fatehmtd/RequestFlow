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
	// delete slots
	//auto children = findChildren<Slot*>();
	//qDeleteAll(children);
	//delete _node;
}

int view::Node::width() const
{
	return _size.width();
}

int view::Node::height() const
{
	return _size.height();
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
	_size.setWidth(320);
	_size.setHeight(160);
	
	_minSize.setWidth(200);
	_minSize.setHeight(200);

	_edgeSize = 5;

	setAcceptHoverEvents(true);

	setFlag(GraphicsItemFlag::ItemIsSelectable);
	setFlag(GraphicsItemFlag::ItemIsMovable);
	setFlag(GraphicsItemFlag::ItemSendsGeometryChanges);
	setCacheMode(CacheMode::DeviceCoordinateCache);

	QFont font;
	font.setBold(true);
	font.setPointSizeF(10);
	_title = new QGraphicsTextItem(this);
	_title->setFont(font);
	//_title->setPlainText(QString("Node name").arg(rand() % 100));
	_title->setDefaultTextColor(Qt::white);
	_title->setPos(18, 5);
}

void view::Node::setupContentWidget()
{
	////////////////////////////
	auto proxyWidget = new QGraphicsProxyWidget(this);
	_contentWidget = new ContentWidget(nullptr);
	proxyWidget->setWidget(_contentWidget);

	int top = getHeaderHeight() + getSlotsSectionHeight();
	int bottom = _size.height() - (top + 20);
	_contentWidget->setGeometry(_edgeSize, top, _size.width() - 2 * _edgeSize, bottom);

	setSize(_size.width(), _size.height());
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
	auto rect = QRectF(0, 0, _size.width(), _size.height());
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

void view::Node::setSize(int w, int h)
{
	prepareGeometryChange();

	auto spacing = _edgeSize * 2;

	float headerSizeY = getHeaderHeight() + getSlotsSectionHeight();

	if (_contentWidget != nullptr)
	{
		_contentWidget->adjustSize();
		
		auto minSize = _contentWidget->minimumSizeHint();

		float tempH = std::max(h, (int)headerSizeY + 2*spacing + minSize.height());
		float tempW = std::max(w, 2 * spacing + minSize.width());

		// manual fixed min width, height
		tempH = std::max(tempH, (float)_minSize.height());
		tempW = std::max(tempW, (float)_minSize.width());
		
		int top = headerSizeY - spacing;
		int bottom = tempH - (top + spacing);

		h = tempH;
		w = tempW;

		_contentWidget->setGeometry(_edgeSize, top, tempW - 2 * _edgeSize, bottom);
	}
	else
	{
		h = std::max((int)headerSizeY, h);
		w = std::max(_minSize.width(), w);
	}

	_size.setWidth(w);
	_size.setHeight(h);

	_bottomRightCorner = _topLeftCorner + QPointF(w, h);
}

float view::Node::getSlotHeight() const
{
	return 25.0f;
}

float view::Node::getHeaderHeight() const
{
	return 45.0f;
}

float view::Node::getSlotsSectionHeight() const
{
	int numOutputSlots = 0, numInputSlots = 0;
	const auto allChildren = childItems();
	int index = 0;
	for (auto child : allChildren)
	{
		auto childSlot = dynamic_cast<Slot*>(child);
		if (childSlot == nullptr) continue;
		if (childSlot->isInput())
			numInputSlots++;
		else
			numOutputSlots++;
	}
	return getSlotHeight() * std::max(numInputSlots, numOutputSlots);
}

float view::Node::getContentHeight() const
{
	if (_contentWidget != nullptr)
	{
		return _contentWidget->height();
	}
	return 0.0f;
}

void view::Node::setMinSize(QSize size)
{
	_minSize = size;
}

QSize view::Node::getMinSize() const
{
	return _minSize;
}

void view::Node::clearUI()
{

}

bool view::Node::isResizable() const
{
	return _isResizable;
}

void view::Node::setResizable(bool status)
{
	_isResizable = status;
}

view::SceneGraph* view::Node::getSceneGraph() const
{
	return dynamic_cast<SceneGraph*>(scene());
}

#include <QStyleOptionGraphicsItem>

void view::Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setClipRect(option->exposedRect);
	// hack to resize at startup
	if (_firstTimeResize)
	{
		setSize(1, 1);
		_firstTimeResize = false;
	}

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
		painter->drawPath(backgroundPath);
	}

	// Title background
	{
		QPainterPath backgroundPath;
		//auto bgColor = colors::light::yellow;
		auto bgColor = _bgColor;
		QPen pen(bgColor);

		float headerHeight = getHeaderHeight();
		QBrush backgroundBrush(bgColor);
		backgroundPath.setFillRule(Qt::FillRule::WindingFill);
		backgroundPath.addRoundedRect(0, 0, w, headerHeight, _edgeSize, _edgeSize);
		backgroundPath.addRoundedRect(0, 10, w, headerHeight, 0, 0);
		painter->setPen(pen);
		painter->setBrush(backgroundBrush);
		painter->drawPath(backgroundPath);
	}

	// Outline
	{
		QPainterPath outlinePath;
		outlinePath.addRoundedRect(0, 0, w, h, _edgeSize, _edgeSize);
		QPen pen(isSelected() ? colors::orange : colors::outlineGrey, isSelected() ? 2.0f : 1.0f);
		painter->setPen(pen);
		painter->setBrush(Qt::BrushStyle::NoBrush);
		painter->drawPath(outlinePath);
	}

	// Outline for resizing
	if (_isResizing)
	{/*
		auto p0 = _topLeftCorner - scenePos();
		auto p1 = _bottomRightCorner - _topLeftCorner;

		QPainterPath outlinePath;
		outlinePath.addRoundedRect(p0.x(), p0.y(), p1.x(), p1.y(), _edgeSize, _edgeSize);
		QPen pen(QColor("#00FF00"), 5);
		painter->setPen(pen);
		painter->setBrush(Qt::BrushStyle::NoBrush);
		painter->drawPath(outlinePath.simplified());
		//*/
	}
	else
	{
		// Outline Hover
		if (_mouseHover)
		{
			QPainterPath outlinePath;
			float outlineEdgeSize = _edgeSize * 1.2f;
			float halfEdgeSize = 0.5f * outlineEdgeSize;
			outlinePath.addRoundedRect(halfEdgeSize, halfEdgeSize, w - halfEdgeSize * 2.0, h - halfEdgeSize * 2.0, _edgeSize, _edgeSize);
			QPen pen(QColor("#FFFFA637"), _edgeSize * 1.5, Qt::PenStyle::DashDotDotLine);
			painter->setPen(pen);
			painter->setBrush(Qt::BrushStyle::NoBrush);
			painter->drawPath(outlinePath);
		}

	}
}

void view::Node::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (_isResizing)
	{
		doResize(event);
	}
	else
	{
		QGraphicsObject::mouseMoveEvent(event);
	}
}

void view::Node::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (_resizeEligible)
	{
		_isResizing = true;
		_anchorPoint = event->scenePos();
	}
	else
	{
		QGraphicsObject::mousePressEvent(event);
	}	
}

void view::Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if(_isResizing)
	{
		_isResizing = false;
		update();
	}

	QGraphicsObject::mouseReleaseEvent(event);
}

void view::Node::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHover = true;	
	handleResize(event->scenePos());
	QGraphicsObject::hoverEnterEvent(event);
}

void view::Node::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	handleResize(event->scenePos());
	QGraphicsObject::hoverMoveEvent(event);
}

void view::Node::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	_mouseHover = false;
	QGraphicsObject::hoverLeaveEvent(event);
}

QVariant view::Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
	switch (change)
	{
	case QGraphicsItem::ItemPositionChange:
	{
		auto p = value.toPointF();
		auto delta = p - _topLeftCorner;
		_topLeftCorner = p;
		_bottomRightCorner += delta;
	}
		break;
	case QGraphicsItem::ItemMatrixChange:
		break;
	case QGraphicsItem::ItemVisibleChange:
		break;
	case QGraphicsItem::ItemEnabledChange:
		break;
	case QGraphicsItem::ItemSelectedChange:
		break;
	case QGraphicsItem::ItemParentChange:
		break;
	case QGraphicsItem::ItemChildAddedChange:
		break;
	case QGraphicsItem::ItemChildRemovedChange:
		break;
	case QGraphicsItem::ItemTransformChange:
		break;
	case QGraphicsItem::ItemPositionHasChanged:
		break;
	case QGraphicsItem::ItemTransformHasChanged:
		break;
	case QGraphicsItem::ItemSceneChange:
		break;
	case QGraphicsItem::ItemVisibleHasChanged:
		break;
	case QGraphicsItem::ItemEnabledHasChanged:
		break;
	case QGraphicsItem::ItemSelectedHasChanged:
		break;
	case QGraphicsItem::ItemParentHasChanged:
		break;
	case QGraphicsItem::ItemSceneHasChanged:
		break;
	case QGraphicsItem::ItemCursorChange:
		break;
	case QGraphicsItem::ItemCursorHasChanged:
		break;
	case QGraphicsItem::ItemToolTipChange:
		break;
	case QGraphicsItem::ItemToolTipHasChanged:
		break;
	case QGraphicsItem::ItemFlagsChange:
		break;
	case QGraphicsItem::ItemFlagsHaveChanged:
		break;
	case QGraphicsItem::ItemZValueChange:
		break;
	case QGraphicsItem::ItemZValueHasChanged:
		break;
	case QGraphicsItem::ItemOpacityChange:
		break;
	case QGraphicsItem::ItemOpacityHasChanged:
		break;
	case QGraphicsItem::ItemScenePositionHasChanged:
		break;
	case QGraphicsItem::ItemRotationChange:
		break;
	case QGraphicsItem::ItemRotationHasChanged:
		break;
	case QGraphicsItem::ItemScaleChange:
		break;
	case QGraphicsItem::ItemScaleHasChanged:
		break;
	case QGraphicsItem::ItemTransformOriginPointChange:
		break;
	case QGraphicsItem::ItemTransformOriginPointHasChanged:
		break;
	default:
		break;
	}
	return QGraphicsObject::itemChange(change, value);
}

void view::Node::doResize(QGraphicsSceneMouseEvent* event)
{
	auto p = event->scenePos();
	float x = 0, y = 0, w = width(), h = height();

	// handle the size change
	if (_cursorResizeMode & Handle::RIGHT)
	{
		_bottomRightCorner.setX(p.x());
		w = _bottomRightCorner.x() - _topLeftCorner.x();
	}

	if (_cursorResizeMode & Handle::BOTTOM)
	{
		_bottomRightCorner.setY(p.y());
		h = _bottomRightCorner.y() - _topLeftCorner.y();
	}

	if (_cursorResizeMode & Handle::LEFT)
	{
		_topLeftCorner.setX(p.x());
		x = _topLeftCorner.x();
	}

	if (_cursorResizeMode & Handle::TOP)
	{
		_topLeftCorner.setY(p.y());
		y = _topLeftCorner.y();
	}

	if (x != 0 || y != 0)
	{
		setPos(_topLeftCorner);
		w = _bottomRightCorner.x() - _topLeftCorner.x();
		h = _bottomRightCorner.y() - _topLeftCorner.y();
	}

	if (w != width() || h != height())
	{
		setSize(w, h);
	}

	update();
}

int view::Node::computeGripCorner(const QPointF& p)
{
	auto relativePos = p - scenePos();
	float thresh = 5;

	int corner = 0;

	if (relativePos.x() <= thresh)
	{
		corner |= Handle::LEFT;
	}
	else if ((width() - relativePos.x()) <= thresh)
	{
		corner |= Handle::RIGHT;
	}

	if (relativePos.y() <= thresh)
	{
		corner |= Handle::TOP;
	}
	else if ((height() - relativePos.y()) <= thresh)
	{
		corner |= Handle::BOTTOM;
	}

	return corner;
}

void view::Node::handleResize(const QPointF& pos)
{
	if (!isResizable()) return;

	_cursorResizeMode = computeGripCorner(pos);

	_resizeEligible = _cursorResizeMode != 0;

	switch (_cursorResizeMode)
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
		setCursor(Qt::CursorShape::ArrowCursor);
		break;
	}

	update();
}

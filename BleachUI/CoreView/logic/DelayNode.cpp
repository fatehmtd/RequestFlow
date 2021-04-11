#include "DelayNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

logic::DelayNode::DelayNode(model::DelayNode* modelNode) : view::Node(modelNode, "Delay")
{
	setupUi();
	setTitle("Delay");
	_ui.spinBox->setValue(modelNode->getDelay());
	connect(_ui.spinBox, SIGNAL(valueChanged(int)), modelNode, SLOT(setDelay(int)));
}

void logic::DelayNode::clearUI()
{
	//_editor->clear();
}

QJSValue logic::DelayNode::toJSValue(QJSEngine& engine) const
{
	auto value = Node::toJSValue(engine);
	value.setProperty("_delay", _ui.spinBox->value());
	return value;
}

void logic::DelayNode::fromJSValue(const QJSValue& jsValue)
{
	Node::fromJSValue(jsValue);
	_ui.spinBox->setValue(jsValue.property("_delay").toInt());
}

void logic::DelayNode::setupUi()
{
	auto widget = new QWidget();
	_ui.setupUi(widget);

	getContentWidget()->layout()->addWidget(widget);

	setResizable(false);

	setMinSize(QSize(100, 50));
	setSize(100, 150);

	_bgColor = view::colors::yellow;	
}
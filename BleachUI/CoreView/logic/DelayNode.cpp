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

/*
*/

model::DelayNode::DelayNode(model::Graph* graph) : model::Node(graph, "Delay")
{
	_timer.setInterval(1000);
	_timer.setSingleShot(true);
	connect(&_timer, &QTimer::timeout, this, &DelayNode::onTimeout);

	connect(this, &Node::ready, this, [=]()
		{
			qDebug() << "ready";
			_timer.start();
		});
}

void model::DelayNode::createModel()
{
	_inputSlot = addInputSlot("input", Slot::DataType::CUSTOM);
	_outputSlot = addOutputSlot("output", Slot::DataType::CUSTOM);
}

void model::DelayNode::setDelay(int duration)
{
	_timer.setInterval(duration);
}

unsigned int model::DelayNode::getDelay() const
{
	return _timer.interval();
}

void model::DelayNode::onTimeout()
{
	_outputSlot->setData(_inputSlot->getData());
	evaluate();
}

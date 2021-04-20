#include "DelayNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

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
	addInputSlot("input", Slot::DataType::CUSTOM);
	addOutputSlot("output", Slot::DataType::CUSTOM);
}

void model::DelayNode::setDelay(int duration)
{
	_timer.setInterval(duration);
}

int model::DelayNode::getDelay() const
{
	return _timer.interval();
}

void model::DelayNode::onTimeout()
{
	getOutputSlots().values()[0]->setData(getInputSlots().values()[0]->getData());
	evaluate();
}
